#include "MusicDecodecThread.h"
#include <QDebug>
#include <QTime>

MusicDecodecThread::MusicDecodecThread(QObject *parent)
    :QThread(parent)
{
    av_register_all();
    avfilter_register_all();
    m_AvFrame = av_frame_alloc();
    g_AudioPlayThread->start();
}

MusicDecodecThread::~MusicDecodecThread()
{

}

bool MusicDecodecThread::openAudioFile(QString fileName)
{
    QMutexLocker locker(&m_Mutex);
    if (m_AVFormatContext)
        avformat_close_input(&m_AVFormatContext);

    // 打开文件
    int result = avformat_open_input(&m_AVFormatContext, fileName.toLocal8Bit().data(), nullptr, nullptr);
    if (result != 0 || m_AVFormatContext == nullptr)
        return false;

    // 查找流信息，把它存入AVFormatContext中
    if (avformat_find_stream_info(m_AVFormatContext, nullptr) < 0)
        return false;

    int streamsCount = m_AVFormatContext->nb_streams;

    // 读取详细信息
    AVDictionaryEntry *tag = nullptr;
    while (tag = av_dict_get(m_AVFormatContext->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))
    {
        QString keyString = QString(tag->key);
        QString valueString = QString(tag->value);
        m_InfoMap.insert(keyString, valueString);
    }

    // 查找音频流索引
    for (int i=0; i<streamsCount; ++i)
    {
        if (m_AVFormatContext->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC)
        {
            AVPacket pkt = m_AVFormatContext->streams[i]->attached_pic;
            m_InfoImage = QImage::fromData((uchar*)pkt.data, pkt.size);
        }
        if (m_AVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_AudioIndex = i;
            continue;
        }
    }

    if (m_AudioIndex == -1)
        return false;

    // 获取总时间
    m_TotalTime = m_AVFormatContext->duration / AV_TIME_BASE * 1000;

    // 查找解码器
    m_AudioCodec = m_AVFormatContext->streams[m_AudioIndex]->codec;
    AVCodec *codec = avcodec_find_decoder(m_AudioCodec->codec_id);
    if (codec == nullptr)
        return false;

    // 打开音频解码器
    if (avcodec_open2(m_AudioCodec, codec, nullptr) != 0)
        return false;

    int rate = m_AudioCodec->sample_rate;
    int channel = m_AudioCodec->channels;
    m_AudioCodec->channel_layout = av_get_default_channel_layout(m_AudioCodec->channels);
    g_AudioPlayThread->cleanAllAudioBuffer();
    g_AudioPlayThread->setCurrentSampleInfo(rate, 16, channel);

    return true;
}

void MusicDecodecThread::run(void)
{
    QTime time;
    int count = 0;
    while (!this->isInterruptionRequested())
    {
        QMutexLocker locker(&m_Mutex);

        AVPacket pkt;
        int result = av_read_frame(m_AVFormatContext, &pkt);
        if (result != 0)
        {
            QThread::msleep(10);
            continue;
        }

        if (pkt.stream_index != m_AudioIndex)
            continue;

        // 解码视频帧， 发送视频包
        if (avcodec_send_packet(m_AudioCodec, &pkt))
            continue;

        // 解码视频帧，接收视频解码帧
        if (avcodec_receive_frame(m_AudioCodec, m_AvFrame))
            continue;

        SwrContext *m_SWRtx = swr_alloc();
        swr_alloc_set_opts(m_SWRtx, m_AudioCodec->channel_layout, AV_SAMPLE_FMT_S16, \
                           m_AudioCodec->sample_rate, m_AudioCodec->channels, m_AudioCodec->sample_fmt, \
                           m_AudioCodec->sample_rate, 0, 0);
        swr_init(m_SWRtx);

        uint8_t *array[1];
        uint8_t arrays[10000] = {0};
        array[0] = arrays;
        int len = swr_convert(m_SWRtx, array, 10000, (const uint8_t **)m_AvFrame->data, m_AvFrame->nb_samples);

        g_AudioPlayThread->addAudioBuffer((char*)arrays, m_AvFrame->linesize[0] * 2);

        swr_free(&m_SWRtx);

        av_packet_unref(&pkt);
    }
}

QMap<QString, QString> MusicDecodecThread::getInfoMap(void)
{
    QMutexLocker locker(&m_Mutex);
    return m_InfoMap;
}

QImage MusicDecodecThread::getMusicIcon(void)
{
    QMutexLocker locker(&m_Mutex);
    return m_InfoImage;
}

int MusicDecodecThread::getTotalTime(void)
{
    return m_TotalTime;
}

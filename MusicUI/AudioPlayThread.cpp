#include "AudioPlayThread.h"
#include <QMutexLocker>
#include <QDebug>

AudioPlayThread::AudioPlayThread(QObject *parent)
    :QThread(parent)
{
    m_PCMDataBuffer.clear();
}

AudioPlayThread::~AudioPlayThread()
{

}

AudioPlayThread *AudioPlayThread::getInstance(void)
{
    static AudioPlayThread instance;
    return &instance;
}

void AudioPlayThread::setCurrentBuffer(QByteArray buffer)
{
    QMutexLocker locker(&m_Mutex);

    m_PCMDataBuffer.clear();
    m_PCMDataBuffer = buffer;
    m_IsPlaying = true;
}

void AudioPlayThread::setCurrentSampleInfo(int sampleRate, int sampleSize, int channelCount)
{
    QMutexLocker locker(&m_Mutex);
    //this->requestInterruption();

    // Format
    QAudioFormat nFormat;
    nFormat.setSampleRate(sampleRate);
    nFormat.setSampleSize(sampleSize);
    nFormat.setChannelCount(channelCount);
    nFormat.setCodec("audio/pcm");
    nFormat.setByteOrder(QAudioFormat::LittleEndian);
    nFormat.setSampleType(QAudioFormat::UnSignedInt);

    if (m_OutPut != nullptr)
        delete m_OutPut;
    m_OutPut = new QAudioOutput(nFormat);
    m_AudioIo = m_OutPut->start();
    //this->start();
}

void AudioPlayThread::run(void)
{
    while (!this->isInterruptionRequested())
    {
        if (!m_IsPlaying)
        {
            continue;
            QThread::msleep(10);
        }

        QMutexLocker locker(&m_Mutex);

        if (m_PCMDataBuffer.count() <= 0 || m_CurrentPlayIndex >= m_PCMDataBuffer.count())
        {
            QThread::msleep(10);
            continue;
        }

        if (m_OutPut->bytesFree() >= m_OutPut->periodSize())
        {
            char *writeData = new char[m_OutPut->periodSize()];

            // 获取将要播放的数据
            int size = m_OutPut->periodSize();
            size = qMin(size, m_PCMDataBuffer.size() - m_CurrentPlayIndex);
            memcpy(writeData, &m_PCMDataBuffer.data()[m_CurrentPlayIndex], size);

            // 写入音频数据
            m_AudioIo->write(writeData, size);
            m_CurrentPlayIndex += size;

            emit updatePlayStatus();
            QThread::msleep(10);
        }
    }
}

// 添加数据
void AudioPlayThread::addAudioBuffer(char* pData, int len)
{
    QMutexLocker locker(&m_Mutex);

    m_PCMDataBuffer.append(pData, len);
    m_IsPlaying = true;
}

void AudioPlayThread::cleanAllAudioBuffer(void)
{
    QMutexLocker locker(&m_Mutex);
    m_CurrentPlayIndex = 0;
    m_PCMDataBuffer.clear();
    m_IsPlaying = false;
}

void AudioPlayThread::playMusic(bool status)
{
    m_IsPlaying = status;
}

bool AudioPlayThread::getPlayMusicStatus(void)
{
    return m_IsPlaying;
}

void AudioPlayThread::setCurrentVolumn(qreal volumn)
{
    if (m_OutPut)
        m_OutPut->setVolume(volumn);
}

qreal AudioPlayThread::getCurrentVolumn(void)
{
    if (!m_OutPut)
        return 0;

    return m_OutPut->volume();
}

// 获取当前的所在位置大小
int AudioPlayThread::getCurrentBuffIndex(void)
{
    return m_CurrentPlayIndex;
}

int AudioPlayThread::getCurrentTime(void)
{
    QMutexLocker locker(&m_Mutex);

    qreal sec = m_CurrentPlayIndex * 1.0 / 4 * (1 * 1.0 / m_OutPut->format().sampleRate());
    return sec * 1000;
}

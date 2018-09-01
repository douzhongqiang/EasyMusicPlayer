#ifndef MUSCI_DECODEC_THREAD_H
#define MUSCI_DECODEC_THREAD_H
#include <QThread>
#include <QObject>
#include <QMap>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include "AudioPlayThread.h"
extern "C"{
    #include <stdio.h>
    #include <stdlib.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/frame.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include <libavfilter/avfiltergraph.h>
    #include <libavfilter/buffersrc.h>
    #include <libavfilter/buffersink.h>
    #include <libavutil/opt.h>
    #include <libavutil/error.h>
}

class MusicDecodecThread : public QThread
{
    Q_OBJECT

public:
    MusicDecodecThread(QObject *parent = nullptr);
    ~MusicDecodecThread();

    // 打开文件
    bool openAudioFile(QString fileName);

    void run(void) override;

    // 获取信息列表中的内容
    QMap<QString, QString> getInfoMap(void);
    // 获取音乐的头像
    QImage getMusicIcon(void);
    // 获取音乐的总时长
    int getTotalTime(void);

private:
    AVFormatContext *m_AVFormatContext = nullptr;
    AVCodecContext *m_AudioCodec = nullptr;
    AVFrame *m_AvFrame;

    int m_AudioIndex = -1;
    int m_TotalTime = 0;

    QMap<QString, QString> m_InfoMap;
    QImage m_InfoImage;

    QMutex m_Mutex;
};

#endif

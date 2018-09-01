#ifndef AUDIO_PLAY_THREAD_H
#define AUDIO_PLAY_THREAD_H

#include <QThread>
#include <QObject>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QMutex>
#include <QByteArray>
#define g_AudioPlayThread AudioPlayThread::getInstance()
class AudioPlayThread : public QThread
{
    Q_OBJECT

public:
    static AudioPlayThread *getInstance(void);

public:
    AudioPlayThread(QObject *parent = nullptr);
    ~AudioPlayThread();

    // ----------- 添加数据相关 ----------------------------------------
    // 设置当前的PCM Buffer
    void setCurrentBuffer(QByteArray buffer);
    // 添加数据
    void addAudioBuffer(char* pData, int len);
    // 清空当前的数据
    void cleanAllAudioBuffer(void);
    // ------------- End ----------------------------------------------

    // 设置当前的采样率、采样位数、通道数目
    void setCurrentSampleInfo(int sampleRate, int sampleSize, int channelCount);

    virtual void run(void) override;

    // 获取当前的所在位置大小
    int getCurrentBuffIndex(void);
    // 获取当前的时间
    int getCurrentTime(void);

    // 切换播放状态
    void playMusic(bool status);
    // 获取当前的播放状态
    bool getPlayMusicStatus(void);
    // 设置音量
    void setCurrentVolumn(qreal volumn);
    // 获取当前音量
    qreal getCurrentVolumn(void);

private:
    QAudioOutput *m_OutPut = nullptr;
    QIODevice *m_AudioIo = nullptr;

    QByteArray m_PCMDataBuffer;
    int m_CurrentPlayIndex = 0;

    QMutex m_Mutex;
    // 播放状态
    bool m_IsPlaying = true;

signals:
    void updatePlayStatus(void);
};

#endif

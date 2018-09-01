#include "MusicWidget.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QTime>
#include "AudioPlayThread.h"

MusciWidget::MusciWidget(QWidget *parent)
    :UIBaseWindow(parent)
{
    m_MusicDecodecThread = new MusicDecodecThread(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addSpacing(30);

    QHBoxLayout *layout = new QHBoxLayout;
    QSplitter *spliter = new QSplitter;
    mainLayout->addWidget(spliter);

    m_MusicListWidget = new MusicListWidget;
    spliter->addWidget(m_MusicListWidget);
    m_MusicListWidget->setMinimumWidth(300);
    QObject::connect(m_MusicListWidget, SIGNAL(openMusicUrl(QString)), this, SLOT(onOpenMusicUrl(QString)));

    m_MusicControlWidget = new MusicBottomControlWidget;
    m_MusicControlWidget->setFixedHeight(80);
    mainLayout->addWidget(m_MusicControlWidget);
    QObject::connect(m_MusicControlWidget, SIGNAL(loadMusicFile(QString)), this, SLOT(addPlayUrlList(QString)));
    QObject::connect(m_MusicControlWidget, SIGNAL(previousAudio()), this, SLOT(onRecvPreviousMusic()));
    QObject::connect(m_MusicControlWidget, SIGNAL(nextAudio()), this, SLOT(onRecvNextMusic()));

    QLabel *label = new QLabel;
    spliter->addWidget(label);
    spliter->setStretchFactor(0, 1);
    spliter->setStretchFactor(1, 2);
    m_RightWidget = label;

    // 连接信号和槽
    QObject::connect(g_AudioPlayThread, &AudioPlayThread::updatePlayStatus, \
                     this, &MusciWidget::onRecvUpdateAudioPlayStatus);
}

MusciWidget::~MusciWidget()
{

}

void MusciWidget::addPlayUrlList(QString musicURL)
{
    // 打开音频文件
    m_MusicDecodecThread->openAudioFile(musicURL);
    if (!m_MusicDecodecThread->isRunning())
        m_MusicDecodecThread->start();

    // 添加音乐信息
    QMap<QString, QString> infoMap = m_MusicDecodecThread->getInfoMap();
    MusicListWidget::MusicItemInfo info ;
    QMap<QString, QString>::iterator itor;
    if ((itor = infoMap.find("title")) != infoMap.end())
        info.titleName = itor.value();
    if ((itor = infoMap.find("album")) != infoMap.end())
        info.albumName = itor.value();
    if ((itor = infoMap.find("artist")) != infoMap.end())
        info.artistName = itor.value();
    info.musicItemIconInfo = m_MusicDecodecThread->getMusicIcon();
    info.musicUrl = musicURL;
    m_MusicListWidget->addItemInfo(info);

    // 设置图片
    m_RightWidget->setPixmap(QPixmap::fromImage(m_MusicDecodecThread->getMusicIcon()));

    // 设置总音乐时长
    int totalTime = m_MusicDecodecThread->getTotalTime();
    QString timeStr = time2String(totalTime);
    m_MusicControlWidget->setTotalTime(timeStr);
}

// 上一曲
void MusciWidget::onRecvPreviousMusic(void)
{
    m_MusicListWidget->changePreviousMusic();
    QString str = m_MusicListWidget->getCurrentPlayMusic();
    if (!str.isEmpty())
        onOpenMusicUrl(str);
}

// 下一曲
void MusciWidget::onRecvNextMusic(void)
{
    m_MusicListWidget->changeNextMusic();
    QString str = m_MusicListWidget->getCurrentPlayMusic();
    if (!str.isEmpty())
        onOpenMusicUrl(str);
}

void MusciWidget::onRecvUpdateAudioPlayStatus(void)
{
    // 设置当前播放的时间
    int currentTime = g_AudioPlayThread->getCurrentTime();
    QString timeStr = time2String(currentTime);
    m_MusicControlWidget->setCurrentPlayTime(timeStr);

    // 设置进度条当前进度
    int totalTime = m_MusicDecodecThread->getTotalTime();
    m_MusicControlWidget->setPlaySliderValue(currentTime * 1.0 / totalTime * 100);
    if (currentTime * 1.0 / totalTime * 100 >= 100)
        onRecvNextMusic();
}

void MusciWidget::onOpenMusicUrl(QString fileName)
{
    // 打开音频文件
    m_MusicDecodecThread->openAudioFile(fileName);
    if (!m_MusicDecodecThread->isRunning())
        m_MusicDecodecThread->start();

    // 设置图片
    m_RightWidget->setPixmap(QPixmap::fromImage(m_MusicDecodecThread->getMusicIcon()));

    // 设置总音乐时长
    int totalTime = m_MusicDecodecThread->getTotalTime();
    QString timeStr = time2String(totalTime);
    m_MusicControlWidget->setTotalTime(timeStr);
}

QString MusciWidget::time2String(int times)
{
    // 设置总音乐时长
    int totalTime = times / 1000;
    int min = totalTime / 60;
    int sec = totalTime % 60;

    QTime time;
    time.setHMS(0, min, sec);
    return time.toString("mm:ss");
}

#ifndef MUSIC_WIDGET_H
#define MUSIC_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include "UIBase/UIBaseWindow.h"
#include "MusicDecodecThread.h"
#include "MusicLisWidget.h"
#include "MusicBottomControl.h"
class MusciWidget : public UIBaseWindow
{
    Q_OBJECT

public:
    MusciWidget(QWidget *parent = nullptr);
    ~MusciWidget();

private:
    MusicDecodecThread *m_MusicDecodecThread = nullptr;

    //QTextEdit *m_TextEdit = nullptr;

    MusicListWidget *m_MusicListWidget = nullptr;               // 左侧音乐列表
    QLabel *m_RightWidget = nullptr;                            // 右侧内容显示列表
    MusicBottomControlWidget *m_MusicControlWidget = nullptr;   // 底部控制窗口

    // 时间转成字符串
    QString time2String(int time);

private slots:
    // 添加文件到列表
    void addPlayUrlList(QString musicURL);
    // 上一曲
    void onRecvPreviousMusic(void);
    // 下一曲
    void onRecvNextMusic(void);
    // 更新当前音频信息
    void onRecvUpdateAudioPlayStatus(void);
    // 打开文件
    void onOpenMusicUrl(QString);
};

#endif

#ifndef MUSIC_BOTTOM_CONTROL_H
#define MUSIC_BOTTOM_CONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include "UIBase/UIGlobalTool.h"

class MusicBottomControlWidget : public QWidget
{
    Q_OBJECT

public:
    MusicBottomControlWidget(QWidget *parent = nullptr);
    ~MusicBottomControlWidget();

    // 设置总时长
    void setTotalTime(QString str);
    // 设置当前播放的时间
    void setCurrentPlayTime(QString str);
    // 设置播放进度条的进度
    void setPlaySliderValue(int value);

private:
    QPushButton *m_LoadFileButton = nullptr;        // 加载文件
    QPushButton *m_ChangePlayMode = nullptr;        // 切换播放样式
    QPushButton *m_ChangePlayPreview = nullptr;     // 切换预览主界面

    QPushButton *m_PreviousButton = nullptr;        // 上一曲
    QPushButton *m_NextButton = nullptr;            // 下一曲
    QPushButton *m_PlayButton = nullptr;            // 播放/暂停
    QSlider *m_PlayStatusSlider = nullptr;          // 播放进度条
    QLabel *m_CurrentTimeLabel = nullptr;           // 当前播放的时间
    QLabel *m_TotalTimeLabel = nullptr;             // 总时间

    QPushButton *m_VolumnButton = nullptr;          // 声音控制
    QSlider *m_VolumnSlider = nullptr;              // 音量进度条

    // 初始化UI
    void initUI(void);

private slots:
    void onLoadMusicFile(void);                     // 添加文件到列表
    void onClickedPreviousButton(void);             // 上一曲
    void onClickedNextButton(void);                 // 下一曲
    void onClickedPlayButton(void);                 // 点击播放/暂停按钮
    void onClickedVolumnButton(void);               // 静音按钮

    // 更改音量
    void onChangedVolumnSlider(int);

signals:
    void loadMusicFile(QString);            // 添加文件到音乐列表
    void previousAudio(void);               // 上一曲
    void nextAudio(void);                   // 下一曲

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif

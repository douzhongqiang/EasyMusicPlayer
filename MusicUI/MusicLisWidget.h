#ifndef MISIC_LIST_WIDGET_H
#define MISIC_LIST_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

class MusicListItemWidget;
class MusicListWidget : public QWidget
{
    Q_OBJECT

public:
    struct MusicItemInfo
    {
        QImage musicItemIconInfo;       // 音乐图片
        QString artistName;             // 艺术家名
        QString albumName;              // 专辑名称
        QString titleName;              // 音乐名
        QString musicUrl;               // 音乐路径
    };
public:
    MusicListWidget(QWidget *parent = nullptr);
    ~MusicListWidget();

    // 向列表中添加内容
    void addItemInfo(MusicItemInfo info);
    // 获取当前播放的音乐
    QString getCurrentPlayMusic(void);
    // 切换到上一曲
    void changePreviousMusic(void);
    // 切换到下一曲
    void changeNextMusic(void);

private:
    QList<MusicListItemWidget*> m_MusicItemList;

    // UI相关
    QVBoxLayout *m_MainLayout = nullptr;
    // 初始化界面
    void initUI(void);
    // 清除所有的播放标志
    void cleanAllPlayFlag(void);
    // 更新当前播放的标志
    void updateCurrentPlayFlag(void);

    // 当前播放的音乐索引
    int m_CurrentIndex = -1;

signals:
    void openMusicUrl(QString);

private slots:
    void onRecvChangeCurrentAudio(QString);
};

// ----------------------------------------------------------------------
class MusicListItemWidget : public QWidget
{
    Q_OBJECT

public:
    MusicListItemWidget(QWidget *parent = nullptr);
    ~MusicListItemWidget();

    // 设置当前的信息
    void setMusicItemInfo(MusicListWidget::MusicItemInfo info);
    // 获取当前的信息
    MusicListWidget::MusicItemInfo getMusicItemInfo(void);
    // 根据当前信息，生成界面
    void generateWidget(void);
    // 设置当前为正在播放
    void setCurrentPlay(bool isPlay);

private:
    MusicListWidget::MusicItemInfo m_MusicInfo;
    QWidget *m_MainWidget = nullptr;
    QVBoxLayout *m_MainLayout = nullptr;
    bool m_IsCurrentPlay = false;
    QLabel *m_CurrentPlayLabel = nullptr;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent * event) override;

signals:
    void currentClicked(QString);
};

#endif

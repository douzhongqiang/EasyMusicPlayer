#include "MusicLisWidget.h"
#include "UIBase/UIGlobalTool.h"
#include <QLabel>

MusicListItemWidget::MusicListItemWidget(QWidget *parent)
    :QWidget(parent)
{

}

MusicListItemWidget::~MusicListItemWidget()
{

}

void MusicListItemWidget::setMusicItemInfo(MusicListWidget::MusicItemInfo info)
{
    m_MusicInfo = info;
    generateWidget();
}

MusicListWidget::MusicItemInfo MusicListItemWidget::getMusicItemInfo(void)
{
    return m_MusicInfo;
}

void MusicListItemWidget::generateWidget(void)
{
    MusicListWidget::MusicItemInfo info = m_MusicInfo;
    if (m_MainWidget)
        delete m_MainWidget;
    m_MainWidget = new QWidget;
    if (m_MainLayout == nullptr)
    {
        m_MainLayout = new QVBoxLayout(this);
        m_MainLayout->setSpacing(0);
        m_MainLayout->setMargin(2);
    }
    m_MainLayout->addWidget(m_MainWidget);
    this->setMinimumHeight(50);

    m_MainWidget->setProperty("outsetWidget", "outsetWidget");
    g_GlobalTool->addShadowEffect(m_MainWidget);

    QHBoxLayout *layout = new QHBoxLayout(m_MainWidget);
    layout->setSpacing(5);
    layout->setMargin(2);

    // 歌曲图片
    QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(50, 50);
    if (!info.musicItemIconInfo.isNull())
        imageLabel->setPixmap(QPixmap::fromImage(info.musicItemIconInfo.scaled(50, 50)));
    layout->addWidget(imageLabel);

    // 当前播放
    QLabel *playSound = new QLabel;
    playSound->setFixedSize(20, 20);
    QImage image = QImage("./icons/Music/speaker.png");
    playSound->setPixmap(QPixmap::fromImage(image.scaled(20, 20)));
    m_CurrentPlayLabel = playSound;
    m_CurrentPlayLabel->setVisible(m_IsCurrentPlay);
    layout->addWidget(m_CurrentPlayLabel);

    QWidget *rightWidget = new QWidget;
    layout->addWidget(rightWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setMargin(0);
    rightLayout->setSpacing(0);

    // 歌曲名称
    QLabel *musicTitleTag = new QLabel(info.titleName);
    rightLayout->addWidget(musicTitleTag);
    // 专辑名和歌手
    QString infoString = "专辑：%1  歌手：%2";
    infoString = infoString.arg(info.albumName).arg(info.artistName);
    QLabel *albumAndAuthorTag = new QLabel(infoString);
    rightLayout->addWidget(albumAndAuthorTag);
}

void MusicListItemWidget::setCurrentPlay(bool isPlay)
{
    m_IsCurrentPlay = isPlay;
    m_CurrentPlayLabel->setVisible(isPlay);
}

void MusicListItemWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
    emit currentClicked(m_MusicInfo.musicUrl);
    setCurrentPlay(true);
    QWidget::mouseDoubleClickEvent(event);
}

// -------------------------------------------------------------------------
MusicListWidget::MusicListWidget(QWidget *parent)
    :QWidget(parent)
{
    initUI();
}

MusicListWidget::~MusicListWidget()
{

}

void MusicListWidget::addItemInfo(MusicItemInfo info)
{
    MusicListItemWidget *wdt = new MusicListItemWidget;
    QObject::connect(wdt, SIGNAL(currentClicked(QString)), this, SLOT(onRecvChangeCurrentAudio(QString)));

    wdt->setMusicItemInfo(info);

    if (m_MusicItemList.count() <= 0)
    {
        m_MainLayout->insertWidget(m_MusicItemList.count(), wdt);
        m_MainLayout->addStretch();
    }
    else
        m_MainLayout->insertWidget(m_MusicItemList.count(), wdt);
    m_MusicItemList.push_back(wdt);
    m_CurrentIndex = m_MusicItemList.count() - 1;
    cleanAllPlayFlag();
    wdt->setCurrentPlay(true);
}

QString MusicListWidget::getCurrentPlayMusic(void)
{
    if (m_CurrentIndex >= m_MusicItemList.count() || m_CurrentIndex < 0)
        return "";

    MusicListWidget::MusicItemInfo info = m_MusicItemList.at(m_CurrentIndex)->getMusicItemInfo();
    return info.musicUrl;
}

void MusicListWidget::changePreviousMusic(void)
{
    if (m_CurrentIndex == 0)
        m_CurrentIndex = m_MusicItemList.count() - 1;
    else
        m_CurrentIndex--;
    updateCurrentPlayFlag();
}

void MusicListWidget::changeNextMusic(void)
{
    if (m_MusicItemList.count() <= m_CurrentIndex)
        m_CurrentIndex = 0;
    else
        m_CurrentIndex++;
    updateCurrentPlayFlag();
}

void MusicListWidget::initUI(void)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(2);
    mainLayout->setSpacing(5);

    m_MainLayout = mainLayout;
}

void MusicListWidget::cleanAllPlayFlag(void)
{
    for (int i=0; i<m_MusicItemList.count(); ++i)
    {
        MusicListItemWidget *w = m_MusicItemList.at(i);
        w->setCurrentPlay(false);
    }
}

void MusicListWidget::updateCurrentPlayFlag(void)
{
    cleanAllPlayFlag();
    for (int i=0; i<m_MusicItemList.count(); ++i)
    {
        if (i == m_CurrentIndex)
        {
            MusicListItemWidget *w = m_MusicItemList.at(i);
            w->setCurrentPlay(true);
            break;
        }
    }
}

void MusicListWidget::onRecvChangeCurrentAudio(QString str)
{
    emit openMusicUrl(str);

    MusicListItemWidget *w = dynamic_cast<MusicListItemWidget*>(sender());
    if (w == nullptr)
        return;

    cleanAllPlayFlag();
    w->setCurrentPlay(true);
    for (int i=0; i<m_MusicItemList.count();++i)
    {
        if (m_MusicItemList.at(i) == w)
        {
            m_CurrentIndex = i;
            break;
        }
    }
}


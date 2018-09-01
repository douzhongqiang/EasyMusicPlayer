#include "MusicBottomControl.h"
#include "UIBase/UIGlobalTool.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QApplication>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include "AudioPlayThread.h"

MusicBottomControlWidget::MusicBottomControlWidget(QWidget *parent)
    :QWidget(parent)
{
    initUI();
}

MusicBottomControlWidget::~MusicBottomControlWidget()
{

}

void MusicBottomControlWidget::initUI(void)
{
    this->setProperty("controlAreaWidget", "controlAreaWidget");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(5);
    layout->setMargin(2);

    // 播放控制按钮
    m_PreviousButton = new QPushButton;
    m_PreviousButton->setFixedSize(40, 40);
    layout->addWidget(m_PreviousButton);
    g_GlobalTool->addShadowEffect(m_PreviousButton);
    QObject::connect(m_PreviousButton, SIGNAL(clicked()), this, SIGNAL(previousAudio()));
    m_PreviousButton->setIcon(QIcon("./Icons/Music/previous.png"));

    m_PlayButton = new QPushButton;
    m_PlayButton->setFixedSize(50, 50);
    layout->addWidget(m_PlayButton);
    QObject::connect(m_PlayButton, SIGNAL(clicked()), this, SLOT(onClickedPlayButton()));
    g_GlobalTool->addShadowEffect(m_PlayButton);
    m_PlayButton->setIcon(QPixmap::fromImage(QImage("./Icons/Music/play.png")));

    m_NextButton = new QPushButton;
    m_NextButton->setFixedSize(40, 40);
    layout->addWidget(m_NextButton);
    g_GlobalTool->addShadowEffect(m_NextButton);
    QObject::connect(m_NextButton, SIGNAL(clicked()), this, SIGNAL(nextAudio()));
    m_NextButton->setIcon(QIcon("./Icons/Music/next.png"));

    // 进度控制
    m_CurrentTimeLabel = new QLabel;
    m_PlayStatusSlider = new QSlider;
    m_PlayStatusSlider->setOrientation(Qt::Horizontal);
    m_PlayStatusSlider->setMinimum(0);
    m_PlayStatusSlider->setMaximum(100);
    m_TotalTimeLabel = new QLabel;
    layout->addWidget(m_CurrentTimeLabel);
    layout->addWidget(m_PlayStatusSlider);
    layout->addWidget(m_TotalTimeLabel);

    // 声音进度条
    m_VolumnButton = new QPushButton;
    m_VolumnButton->setFixedSize(30, 30);
    g_GlobalTool->addShadowEffect(m_VolumnButton);
    m_VolumnButton->setIcon(QIcon("./Icons/Music/speaker.png"));
    QObject::connect(m_VolumnButton, SIGNAL(clicked()), this, SLOT(onClickedVolumnButton()));

    m_VolumnSlider = new QSlider;
    m_VolumnSlider->setOrientation(Qt::Horizontal);
    m_VolumnSlider->setMinimum(0);
    m_VolumnSlider->setMaximum(100);
    m_VolumnSlider->setFixedWidth(150);
    layout->addWidget(m_VolumnButton);
    layout->addWidget(m_VolumnSlider);
    m_VolumnSlider->setValue(100);
    QObject::connect(m_VolumnSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangedVolumnSlider(int)));

    // 加载文件按钮
    m_ChangePlayMode = new QPushButton;
    m_ChangePlayMode->setFixedSize(30, 30);
    layout->addWidget(m_ChangePlayMode);
    g_GlobalTool->addShadowEffect(m_ChangePlayMode);

    m_ChangePlayPreview = new QPushButton;
    m_ChangePlayPreview->setFixedSize(30, 30);
    layout->addWidget(m_ChangePlayPreview);
    g_GlobalTool->addShadowEffect(m_ChangePlayPreview);

    m_LoadFileButton = new QPushButton();
    m_LoadFileButton->setFixedSize(30, 30);
    g_GlobalTool->addShadowEffect(m_LoadFileButton);
    layout->addWidget(m_LoadFileButton);
    m_LoadFileButton->setIcon(QIcon(QPixmap::fromImage(QImage("./Icons/Music/add.png"))));
    QObject::connect(m_LoadFileButton, SIGNAL(clicked(bool)), this, SLOT(onLoadMusicFile()));
}

void MusicBottomControlWidget::onLoadMusicFile(void)
{
    QString openedFileName = QFileDialog::getOpenFileName(this, "Open File", "./");
    if (!openedFileName.isEmpty())
        emit loadMusicFile(openedFileName);
}

void MusicBottomControlWidget::onClickedPreviousButton(void)
{
    // 上一曲
}

void MusicBottomControlWidget::onClickedNextButton(void)
{
    // 下一曲
}

void MusicBottomControlWidget::onClickedPlayButton(void)
{
    bool playStatus = g_AudioPlayThread->getPlayMusicStatus();
    g_AudioPlayThread->playMusic(!playStatus);

    if (!playStatus)
        m_PlayButton->setIcon(QPixmap::fromImage(QImage("./Icons/Music/pause.png")));
    else
        m_PlayButton->setIcon(QPixmap::fromImage(QImage("./Icons/Music/play.png")));
}

void MusicBottomControlWidget::onClickedVolumnButton(void)
{
    // 静音按钮
    int volumn = g_AudioPlayThread->getCurrentVolumn();

    if (volumn > 0)
        g_AudioPlayThread->setCurrentVolumn(0);
    else
        g_AudioPlayThread->setCurrentVolumn(m_VolumnSlider->value() * 1.0 / 100);
}

void MusicBottomControlWidget::onChangedVolumnSlider(int value)
{
    g_AudioPlayThread->setCurrentVolumn(value * 1.0 / 100);
}

void MusicBottomControlWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void MusicBottomControlWidget::setTotalTime(QString str)
{
    m_TotalTimeLabel->setText(str);
}

void MusicBottomControlWidget::setCurrentPlayTime(QString str)
{
    m_CurrentTimeLabel->setText(str);
}

void MusicBottomControlWidget::setPlaySliderValue(int value)
{
    m_PlayStatusSlider->setValue(value);
}

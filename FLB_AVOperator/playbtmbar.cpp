#include "playbtmbar.h"
#include "uicomponents.h"
#include "avoperator.h"
#include "timeobservers.h"
#include "volumewidget.h"
#include "speedcmbbox.h"

#include <QDebug>
#include <QPainter>

PlayBtmBar::PlayBtmBar(FAVPlayer* player, QWidget* ps, QWidget* p) :QWidget(p), m_pPlayer(player), m_pScreenWidget(ps)
{
	ui.setupUi(this);

	this->setMinimumHeight(75);

	this->setLayout(ui.verticalLayout);
	ui.verticalLayout->setStretch(0, 1);
	ui.verticalLayout->setStretch(1, 5);

	this->initSlider();

	this->initTimeStamp();

	this->initBtmBtns();
	//this->setStyleSheet("border: 2px solid red;");

	this->initLayout();


	this->setMouseTracking(true);
}

PlayBtmBar::~PlayBtmBar()
{
	qDebug() << "底部进度条析构！";
}

void PlayBtmBar::initBtmBtns()
{
	m_iconPlay = QIcon(":/FLB_AVOperator/icons/play.png");
	m_iconPause = QIcon(":/FLB_AVOperator/icons/pause.png");
	m_pBtnPlayPause = new SquareButton(this);
	m_pBtnPlayPause->setIcon(m_iconPlay);
	connect(m_pBtnPlayPause, &QPushButton::clicked, this, &PlayBtmBar::onPlayPauseClicked);

	m_pBtnPlayLast = new SquareButton(this);
	m_pBtnPlayLast->setIcon(QIcon(":/FLB_AVOperator/icons/lastAV.png"));
	connect(m_pBtnPlayLast, &QPushButton::clicked, this, &PlayBtmBar::lastClicked);

	m_pBtnPlayNext = new SquareButton(this);
	m_pBtnPlayNext->setIcon(QIcon(":/FLB_AVOperator/icons/nextAV.png"));
	connect(m_pBtnPlayNext, &QPushButton::clicked, this, &PlayBtmBar::nextClicked);

	m_pBtnForward = new SquareButton(this);
	m_pBtnForward->setIcon(QIcon(":/FLB_AVOperator/icons/forward.png"));
	connect(m_pBtnForward, &QPushButton::clicked, m_pPlayer, &FAVPlayer::seekForward);

	m_pBtnBackward = new SquareButton(this);
	m_pBtnBackward->setIcon(QIcon(":/FLB_AVOperator/icons/backward.png"));
	connect(m_pBtnBackward, &QPushButton::clicked, m_pPlayer, &FAVPlayer::seekBackward);

	m_pBtnStop = new SquareButton(this);
	m_pBtnStop->setIcon(QIcon(":/FLB_AVOperator/icons/stop.png"));
	connect(m_pBtnStop, &QPushButton::clicked, m_pPlayer, &FAVPlayer::stop);

	m_iconSilence = QIcon(":/FLB_AVOperator/icons/silence.png");
	m_iconVolume = QIcon(":/FLB_AVOperator/icons/volume.png");
	m_pBtnVolume = new SquareButton(this);
	m_pBtnVolume->setIcon(m_iconVolume);
	connect(m_pBtnVolume, &QPushButton::clicked, this, &PlayBtmBar::onVolClicked);

	m_pVolWidget = new VolumeWidget(m_pPlayer, this->parentWidget());
	m_pVolWidget->hide();
	connect(m_pVolWidget, &VolumeWidget::volChanged, this, &PlayBtmBar::onVolChanged);

	m_pCmbBoxSpeed = new SpeedCombobox(m_pPlayer, this);

	m_iconFull = QIcon(":/FLB_AVOperator/icons/full.png");
	m_iconNotFull = QIcon(":/FLB_AVOperator/icons/notfull.png");
	m_pBtnFull = new SquareButton(this);
	m_pBtnFull->setIcon(m_iconFull);
	connect(m_pBtnFull, &QPushButton::clicked, this, &PlayBtmBar::onFullScreen);

	//m_pBtnStop = new SquareButton(this);
	this->setStyleSheet("background-color: #0D0D0D;color: white;");
	//this->setAttribute(Qt::WA_OpaquePaintEvent);
	m_pVolWidget->setStyleSheet("background-color: #050505;color: white;");
	m_pVolWidget->setRelatedWidget(m_pBtnVolume);
	//m_pVolWidget->setAttribute(Qt::WA_OpaquePaintEvent);
}

void PlayBtmBar::initSlider()
{
	m_pSliderController = new SliderController(m_pPlayer, ui.m_pPlaySlider);
	connect(this, &PlayBtmBar::playClicked, m_pPlayer, &FAVPlayer::playCont);
	connect(this, &PlayBtmBar::pauseClicked, m_pPlayer, &FAVPlayer::pause);
	connect(m_pSliderController, &SliderController::seekProp, m_pPlayer, &FAVPlayer::seekProp);
}

void PlayBtmBar::initTimeStamp()
{
	m_pLabelTimestamp = new QLabel(this);
	m_pLabelTimeObserver = new LabelTimeStampObserver(m_pPlayer, m_pLabelTimestamp);

	m_pLabelTimestamp->setFixedWidth(125);
	m_pLabelTimestamp->setFont(QFont("Arial", 14));
}

void PlayBtmBar::initLayout()
{
	ui.horizontalLayout->setContentsMargins(0, 0, 0, 0);

	ui.horizontalLayout->addWidget(m_pBtnPlayLast, 1);
	ui.horizontalLayout->addWidget(m_pBtnPlayPause, 1);
	ui.horizontalLayout->addWidget(m_pBtnPlayNext, 1);

	ui.horizontalLayout->addWidget(m_pBtnBackward, 1);
	ui.horizontalLayout->addWidget(m_pBtnForward, 1);

	ui.horizontalLayout->addWidget(m_pBtnStop, 1);

	ui.horizontalLayout->addWidget(m_pLabelTimestamp);

	ui.horizontalLayout->addWidget(m_pBtnVolume);

	ui.horizontalLayout->addWidget(m_pCmbBoxSpeed);

	ui.horizontalLayout->addWidget(m_pBtnFull, 1);

	//ui.horizontalLayout->addStretch(5);
}

bool PlayBtmBar::isSliderPressed()const
{
	return m_pSliderController->isPressed();
}

bool PlayBtmBar::isMouseEntered()const
{
	return m_isEntered || !m_pVolWidget->isHidden() || m_pCmbBoxSpeed->isPopupShown();
}

void PlayBtmBar::setPlayState(bool isPlay)
{
	if (m_isPlay == isPlay)
		return;

	m_isPlay = isPlay;

	if (m_isPlay)
		m_pBtnPlayPause->setIcon(m_iconPause);
	else
		m_pBtnPlayPause->setIcon(m_iconPlay);
}

void PlayBtmBar::onEventType(QEvent::Type type)
{
	if (type == QEvent::Enter)
		m_isEntered = true;
	else if (type == QEvent::Leave)
		m_isEntered = false;
}

void PlayBtmBar::onFullScreen()
{
	if (m_pScreenWidget == nullptr)
		return;

	if (!m_pScreenWidget->isFullScreen())
	{
		//m_pScreenWidget->showFullScreen();
		m_pBtnFull->setIcon(m_iconNotFull);
	}
	else
	{
		//m_pScreenWidget->showNormal();
		m_pBtnFull->setIcon(m_iconFull);
	}

	emit fullClicked();
}

void PlayBtmBar::onPlayPauseClicked()
{
	bool play = m_isPlay;
	if (m_isPlay)
		emit pauseClicked();
	else
		emit playClicked();

	this->setPlayState(!play);
}

void PlayBtmBar::onVolClicked()
{
	if (!m_pVolWidget->isHidden())
	{
		m_pVolWidget->hide();
		return;
	}
	int x = m_pBtnVolume->x() - m_pVolWidget->width() / 2 + m_pBtnVolume->width() / 2;
	int y = this->y() - m_pVolWidget->height() + m_pBtnVolume->y();

	m_pVolWidget->move(x, y);
	m_pVolWidget->show();

}

void PlayBtmBar::onVolChanged(int v)
{

	if (v == 0)
	{
		m_pBtnVolume->setIcon(m_iconSilence);
	}
	else if (m_preVol == 0)
	{
		m_pBtnVolume->setIcon(m_iconVolume);
	}

	m_preVol = v;
}

void PlayBtmBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setBrush(Qt::black); // 设置刷子的颜色与样式表中的颜色一致
	painter.setPen(Qt::NoPen); // 不绘制边框
	painter.drawRect(this->rect()); // 绘制整个部件的矩形
	QWidget::paintEvent(event); // 调用基类的paintEvent以确保其他绘制正常进行
}


#include "playbtmbar.h"
#include "uicomponents.h"
#include "avoperator.h"
#include "timeobservers.h"

#include <QDebug>


PlayBtmBar::PlayBtmBar(FAVPlayer* player, QWidget* p) :QWidget(p), m_pPlayer(player)
{
	ui.setupUi(this);

	this->setMinimumHeight(75);

	this->setLayout(ui.verticalLayout);
	ui.verticalLayout->setStretch(0, 1);
	ui.verticalLayout->setStretch(1, 5);

	this->initSlider();

	this->initTimeStamp();

	this->initBtmWidget();
	//this->setStyleSheet("border: 2px solid red;");

	this->initLayout();


	this->setMouseTracking(true);
}

PlayBtmBar::~PlayBtmBar()
{
	qDebug() << "底部进度条析构！";
}

void PlayBtmBar::initBtmWidget()
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

	//m_pBtnStop = new SquareButton(this);
	this->setStyleSheet("background-color: #0D0D0D;color: white;");
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

	ui.horizontalLayout->addWidget(m_pLabelTimestamp);

	ui.horizontalLayout->addStretch(5);
}

bool PlayBtmBar::isSliderPressed()const
{
	return m_pSliderController->isPressed();
}

bool PlayBtmBar::isMouseEntered()const
{
	return m_isEntered;
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

void PlayBtmBar::onPlayPauseClicked()
{
	bool play = m_isPlay;
	if (m_isPlay)
		emit pauseClicked();
	else
		emit playClicked();

	this->setPlayState(!play);
}




#include "aveditwidget.h"
#include "videoopenglplayer.h"
#include "avoperator.h"
#include "streamwidgets.h"
#include "tablestreams.h"
#include "tableresource.h"
#include "streamutils.h"
#include "dlgaveditoutput.h"

#include <QMessageBox>

AVEditWidget::AVEditWidget(QWidget* p) : QWidget(p)
{
	ui.setupUi(this);

	this->initWidget();
}

void AVEditWidget::initWidget()
{
	ui.m_pHLayoutUpper->addWidget(ui.m_pTabWidgetSrc);

	m_pVideoPlayer = new VideoOpenGLPlayer(this);
	m_pVideoPlayer->setMinimumWidth(300);
	ui.m_pHLayoutUpper->addWidget(m_pVideoPlayer);
	m_pVideoPlayer->show();
	m_pPlayer = m_pVideoPlayer->getPlayer();
	connect(m_pPlayer, &FAVPlayer::playStateChanged, this, &AVEditWidget::onPlayStateChanged);

	m_pParamBox = new QGroupBox("参数设置", this);
	m_pParamBox->setMinimumWidth(200);
	ui.m_pHLayoutUpper->addWidget(m_pParamBox);

	this->setLayout(ui.m_pVertLayout);
	ui.m_pWidgetUpper->setLayout(ui.m_pHLayoutUpper);

	m_pTableStreams = new TableStreams(m_pPlayer, this);
	ui.m_pScrollAreaAVStream->setWidget(m_pTableStreams);

	/*m_pTableResourceVideo = new TableResource(std::make_unique<ResourceFactoryFileVideo>());
	m_pTableResourceVideo->installTableStream(m_pTableStreams);

	ui.m_pTabVideo->setLayout(new QHBoxLayout(ui.m_pTabVideo));
	ui.m_pTabVideo->layout()->addWidget(m_pTableResourceVideo);
	ui.m_pTabVideo->layout()->setContentsMargins(0, 0, 0, 0);*/
	this->addTableResource(std::make_unique<ResourceFactoryFileVideo>());
	this->addTableResource(std::make_unique<ResourceFactoryFileAudio>());

	this->initBtmButton();
	this->setStyleSheet("QWidget{ color: white; } QWidget:disabled {color: rgba(255, 255, 255, 128);}"
		"QMenu::item:selected { background-color: blue; }"
		"QMenu::item:disabled { background-color: rgba(0,0,0,0); }");
}

void AVEditWidget::initBtmButton()
{
	QWidget* pBtmWidget = new QWidget(this);
	QHBoxLayout* pLayout = new QHBoxLayout(pBtmWidget);
	pBtmWidget->setLayout(pLayout);

	pLayout->addSpacing(500);
	m_pBtnPlay = new QPushButton(pBtmWidget);
	m_pBtnPlay->setText("播放");
	pLayout->addWidget(m_pBtnPlay, 10);
	connect(m_pBtnPlay, &QPushButton::clicked, this, &AVEditWidget::onPlayClick);

	m_pBtnExport = new QPushButton(pBtmWidget);
	m_pBtnExport->setText("导出");
	pLayout->addWidget(m_pBtnExport, 10);
	connect(m_pBtnExport, &QPushButton::clicked, this, &AVEditWidget::onExport);

	pBtmWidget->setMaximumHeight(100);
	ui.m_pVertLayout->addWidget(pBtmWidget);
}

void AVEditWidget::addTableResource(std::unique_ptr<IAVResourceFactory> upResrcFactory)
{
	if (!upResrcFactory)
		return;

	QString title = upResrcFactory->title();
	TableResource* pTableResource = new TableResource(std::move(upResrcFactory));
	pTableResource->installTableStream(m_pTableStreams);

	ui.m_pTabWidgetSrc->addTab(pTableResource, title);
	//ui.m_pTabVideo->setLayout(new QHBoxLayout(ui.m_pTabVideo));
	//ui.m_pTabVideo->layout()->addWidget(pTableResource);
	//ui.m_pTabVideo->layout()->setContentsMargins(0, 0, 0, 0);
}

void AVEditWidget::onPlayClick()
{
	if (m_pPlayer->state() == PlayState::Play)
	{
		m_pPlayer->pause();
		//m_pBtnPlay->setText("播放");
	}
	else if (m_pPlayer->state() == PlayState::Pause)
	{
		m_pPlayer->playCont();
		//m_pBtnPlay->setText("暂停");
	}
}

void AVEditWidget::onExport()
{
	auto spManager = m_pTableStreams->getStreamManager();
	if (!spManager || !spManager->hasStream())
	{
		QMessageBox::information(this, "视频导出", "没有音视频流");
		return;
	}

	DlgAVEditOutput dlgExport(spManager->clone());
	dlgExport.exec();
}

void AVEditWidget::onPlayStateChanged(PlayState state)
{
	if (state == PlayState::Play)
		m_pBtnPlay->setText("暂停");
	else
		m_pBtnPlay->setText("播放");
}

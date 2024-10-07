#include "flb_avoperator.h"
#include "videoopenglplayer.h"
#include "avoperator.h"
#include "playlistwidget.h"
#include "playtable.h"
#include "playbtmbar.h"
#include "avfilemanager.h"
#include "aveditwidget.h"

#include <QDebug>
#include <QDateTime>
#include <QDockWidget>
#include <QMessageBox>
#include <QTabWidget>


FLB_AVOperator* FLB_AVOperator::s_pMainWindow = nullptr;
FLB_AVOperator* FLB_AVOperator::getWindowPtr()
{
	return s_pMainWindow;
}

FLB_AVOperator::FLB_AVOperator(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->initWidgets();

	this->setStyleSheet(
		"QWidget {"
		"   background-color: #2E2E2E; /* 深灰色 */"
		"}"
		"QTabBar::tab { "
		"background: black; "
		"color: white; "
		"padding: 10px; "
		"}"
		"QTabBar::tab:selected { "
		"background: #1d1d1d; "
		"color: white; "
		"}"
		"QTabBar::tab:hover { "
		"background: gray; "
		"}"
	);

	s_pMainWindow = this;
}


FLB_AVOperator::~FLB_AVOperator()
{
	s_pMainWindow = nullptr;
	auto player = m_pVideoWidget->getPlayer();
	player->stop();
}

void FLB_AVOperator::initWidgets()
{
	m_pVideoWidget = new VideoOpenGLPlayer(this);

	this->setDockPlayList();

	m_pEditWidget = new AVEditWidget(this);

	m_pBtnPlayer = new QPushButton("视频播放", this);
	m_pBtnPlayer->setStyleSheet("QPushButton { background-color: black; color: white; } "
		"QPushButton:hover { background-color: lightblue; }"
		"QPushButton:disabled{ "
		"background-color: black;"  // 禁用状态背景颜色
		"color: gray;"              // 禁用状态字体颜色
		"}");
	m_pBtnEditor = new QPushButton("音视频编辑", this);
	m_pBtnEditor->setStyleSheet(m_pBtnPlayer->styleSheet());
	ui.mainToolBar->addWidget(m_pBtnPlayer);
	ui.mainToolBar->addWidget(m_pBtnEditor);
	connect(m_pBtnPlayer, &QPushButton::clicked, this, &FLB_AVOperator::onPlayerShow);
	connect(m_pBtnEditor, &QPushButton::clicked, this, &FLB_AVOperator::onEditorShow);

	this->onPlayerShow();
}

void FLB_AVOperator::onPlayerShow()
{
	m_pEditWidget->hide();
	m_pEditWidget->setParent(nullptr);
	m_pVideoWidget->show();
	this->setCentralWidget(m_pVideoWidget);
	m_pEditWidget->setParent(this);

	m_pDockPlay->show();
	m_pBtnPlayer->setEnabled(false);
	m_pBtnEditor->setEnabled(true);
}

void FLB_AVOperator::onEditorShow()
{
	m_pVideoWidget->hide();
	m_pVideoWidget->setParent(nullptr);
	m_pEditWidget->show();
	this->setCentralWidget(m_pEditWidget);
	m_pVideoWidget->setParent(this);

	m_pDockPlay->hide();
	m_pBtnPlayer->setEnabled(true);
	m_pBtnEditor->setEnabled(false);
}

void FLB_AVOperator::setDockPlayList()
{
	m_pDockPlay = new QDockWidget("音乐播放列表", this);
	m_pDockPlay->setFixedWidth(200);
	m_pDockPlay->setFeatures(QDockWidget::NoDockWidgetFeatures);

	m_pDockPlay->setAllowedAreas(Qt::RightDockWidgetArea);
	this->addDockWidget(Qt::RightDockWidgetArea, m_pDockPlay);


	m_pTabPlayList = new QTabWidget(m_pDockPlay);
	m_pDockPlay->setWidget(m_pTabPlayList);
	m_pDockPlay->setStyleSheet(R"(
        QDockWidget {
            background-color: #1D1D1D; /* Dock widget background color */
			color: white;
        }
        
    )");

	/*m_pTabPlayList->setStyleSheet(
		"QTabBar::tab { "
		"background: black; "
		"color: white; "
		"padding: 10px; "
		"}"
		"QTabBar::tab:selected { "
		"background: #1d1d1d; "
		"color: white; "
		"}"
		"QTabBar::tab:hover { "
		"background: gray; "
		"}"
	);*/
	m_pTabPlayList->setFocusPolicy(Qt::NoFocus);

	m_pLocalPlayList = new PlayListWidget(&AVFileManager::getInstance(), this);
	m_pTabPlayList->addTab(m_pLocalPlayList, "本地");

	m_pOnlinePlayList = new PlayListWidget(&AVUrlManager::getInstance(), this);
	m_pTabPlayList->addTab(m_pOnlinePlayList, "在线");

	auto playTable = m_pLocalPlayList->getPlayTable(), playTableOnline = m_pOnlinePlayList->getPlayTable();
	connect(playTable, &PathPlayTable::playAVF, this, &FLB_AVOperator::openFile);
	connect(playTableOnline, &PathPlayTable::playAVF, this, &FLB_AVOperator::openFile);

	auto playBtm = m_pVideoWidget->getPlayBtmWidget();
	connect(playBtm, &PlayBtmBar::lastClicked, playTable, &PathPlayTable::lastAVF);
	connect(playBtm, &PlayBtmBar::nextClicked, playTable, &PathPlayTable::nextAVF);
	connect(playBtm, &PlayBtmBar::lastClicked, playTableOnline, &PathPlayTable::lastAVF);
	connect(playBtm, &PlayBtmBar::nextClicked, playTableOnline, &PathPlayTable::nextAVF);

	connect(playBtm, &PlayBtmBar::fullClicked, this, &FLB_AVOperator::onFullClicked);
}

void FLB_AVOperator::openFile(QString fp)
{
	auto player = m_pVideoWidget->getPlayer();
	if (player == nullptr || !player->openFile(fp))
	{
		QMessageBox::critical(m_pLocalPlayList, "文件打开提示", "文件播放失败！");
		return;
	}

	if (player->state() != PlayState::Stop)
	{
		player->playCont();
	}
}

//void FLB_AVOperator::openUrl(QString url)
//{
//	auto player = m_pVideoWidget->getPlayer();
//	if (player == nullptr || !player->openUrl(url))
//	{
//		QMessageBox::critical(m_pLocalPlayList, "文件打开提示", "文件播放失败！");
//		return;
//	}
//
//	if (player->state() != PlayState::Stop)
//	{
//		player->playCont();
//	}
//}

void FLB_AVOperator::onFullClicked()
{
	if (!m_pVideoWidget->isFullScreen())
	{
		m_pVideoWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		m_pVideoWidget->showFullScreen();
	}
	else
	{
		m_pVideoWidget->setWindowFlags(m_pVideoWidget->windowFlags() & ~Qt::Window);
		this->setCentralWidget(m_pVideoWidget);
	}
}


#include "flb_avoperator.h"
#include "videoopenglplayer.h"
#include "avoperator.h"
#include "playlistwidget.h"
#include "playtable.h"
#include "playbtmbar.h"
#include "avfilemanager.h"

#include <QDebug>
#include <QDateTime>
#include <QDockWidget>
#include <QMessageBox>
#include <QTabWidget>

FLB_AVOperator::FLB_AVOperator(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pVideoWidget = new VideoOpenGLPlayer(this);
	this->setCentralWidget(m_pVideoWidget);

	this->setDockPlayList();

	this->setStyleSheet(R"(
            QMainWindow {
                background-color: #2E2E2E; /* 深灰色 */
            })");
}


FLB_AVOperator::~FLB_AVOperator()
{
	auto player = m_pVideoWidget->getPlayer();
	player->stop();
}

void FLB_AVOperator::setDockPlayList()
{
	auto pDock = new QDockWidget("音乐播放列表", this);
	pDock->setFixedWidth(200);
	pDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

	pDock->setAllowedAreas(Qt::RightDockWidgetArea);
	this->addDockWidget(Qt::RightDockWidgetArea, pDock);


	m_pTabPlayList = new QTabWidget(pDock);
	pDock->setWidget(m_pTabPlayList);
	pDock->setStyleSheet(R"(
        QDockWidget {
            background-color: #1D1D1D; /* Dock widget background color */
			color: white;
        }
        
    )");

	m_pTabPlayList->setStyleSheet(
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


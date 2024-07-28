#include "flb_avoperator.h"
#include "videoopenglplayer.h"
#include "avoperator.h"
#include "playlistwidget.h"
#include "playtable.h"
#include "playbtmbar.h"

#include <QDebug>
#include <QDateTime>
#include <QDockWidget>
#include <QMessageBox>

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

	/*auto player = m_pVideoWidget->getPlayer();
	player->openFile("D:\\Users\\My\\source\\repos\\git\\mingjunhao\\week08\\homework\\KMusicPlayer\\KMusicPlayer\\local files\\Stars_Align.mp4");
	if (player->state() != PlayState::Stop)
	{
		player->playCont();
	}
	else
	{
		qDebug() << "open Failed!";
	}*/
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

	m_pPlayList = new PlayListWidget(this);
	pDock->setWidget(m_pPlayList);

	auto playTable = m_pPlayList->getPlayTable();
	connect(playTable, &LocalPlayTable::playAVF, this, &FLB_AVOperator::openFile);

	pDock->setStyleSheet(R"(
        QDockWidget {
            background-color: #1D1D1D; /* Dock widget background color */
			color: white;
        }
        
    )");

	auto playBtm = m_pVideoWidget->getPlayBtmWidget();
	connect(playBtm, &PlayBtmBar::lastClicked, playTable, &LocalPlayTable::lastAVF);
	connect(playBtm, &PlayBtmBar::nextClicked, playTable, &LocalPlayTable::nextAVF);
	connect(playBtm, &PlayBtmBar::fullClicked, this, &FLB_AVOperator::onFullClicked);
}

void FLB_AVOperator::openFile(QString fp)
{
	auto player = m_pVideoWidget->getPlayer();
	if (player == nullptr || !player->openFile(fp))
	{
		QMessageBox::critical(m_pPlayList, "文件打开提示", "文件播放失败！");
		return;
	}

	if (player->state() != PlayState::Stop)
	{
		player->playCont();
	}
}


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


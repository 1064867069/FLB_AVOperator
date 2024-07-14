#include "flb_avoperator.h"
#include "videoopenglplayer.h"
#include "avoperator.h"
#include <QDebug>

FLB_AVOperator::FLB_AVOperator(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pVideoWidget = new VideoOpenGLPlayer(this);
	this->setCentralWidget(m_pVideoWidget);

	auto player = m_pVideoWidget->getPlayer();
	player->openFile("D:\\Users\\My\\source\\repos\\git\\mingjunhao\\week08\\homework\\KMusicPlayer\\KMusicPlayer\\local files\\Stars_Align.mp4");
	if (player->state() != PlayState::Stop)
	{
		player->pause(false);
	}
	else
	{
		qDebug() << "open Failed!";
	}
}


FLB_AVOperator::~FLB_AVOperator()
{
	auto player = m_pVideoWidget->getPlayer();
	player->stop();
}

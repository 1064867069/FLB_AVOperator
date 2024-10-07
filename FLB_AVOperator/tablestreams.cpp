#include "tablestreams.h"
#include "streamutils.h"
#include "streamwidgets.h"
#include "avoperator.h"
#include "streamtimewidget.h"

#include <QHeaderView>
#include <QDebug>
#include <QPushButton>

TableStreams::TableStreams(FAVPlayer* pPlayer, QWidget* p) :QTableWidget(p),
m_spStreamManager(StreamManager::createManager()), m_pPlayer(pPlayer)
{
	m_pSlider = new StreamTimeSlider(m_pPlayer, this);
	m_pSlider->setFixedWidth(m_pad);
	m_pSliderController = new StreamSliderController(m_pPlayer, m_pSlider);
	connect(this, &TableStreams::durationSecondChanged, m_spStreamManager.get(), &StreamManager::onEndChanged);
	connect(m_spStreamManager.get(), &StreamManager::hasStreamGotton, this, &TableStreams::onStreamFirstHas);
	connect(m_pSlider, &StreamTimeSlider::seekProp, m_pPlayer, &FAVPlayer::seekProp);
	connect(m_pPlayer, &FAVPlayer::playStateChanged, m_spStreamManager.get(), &StreamManager::onPlayStateChanged);

	m_pStreamBlockTimeWidget = new StreamTimeWidget(this);

	this->setColumnCount(1);
	this->horizontalHeader()->hide();
	this->verticalHeader()->hide();
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	this->setStyleSheet(
		"QTableWidget::item:selected {"
		"    background-color: transparent;"  // 设置选中时背景颜色为透明
		"    border: none;"                   // 去除边框
		"}"
	);
}

TableStreams::~TableStreams()
{
	emit m_spStreamManager->abandoned();
}

bool TableStreams::addStreams(const QList<StreamPtr>& streams)
{
	if (streams.isEmpty())
		return false;

	static constexpr int ls_perHeight = 100;
	for (int i = 0; i < streams.size(); ++i)
	{
		if (!m_spStreamManager->addStream(streams[i]))
		{
			for (int j = 0; j < i; ++j)
				m_spStreamManager->removeStream(streams[j]);
			return false;
		}
	}

	int rowCnt = this->rowCount();
	this->setRowCount(rowCnt + streams.size());
	for (int i = 0; i < streams.size(); ++i)
	{
		streams[i]->bindStreamManager(m_spStreamManager);

		AVStreamItemWidget* pItemWidget = new AVStreamItemWidget(m_spStreamManager, this);
		pItemWidget->setFixedHeight(ls_perHeight);
		connect(pItemWidget, &AVStreamItemWidget::backEndChanged, this, &TableStreams::onBackEndChanged);
		connect(pItemWidget, &AVStreamItemWidget::blockTimeNeedSet, this, &TableStreams::onBlockTimeSet);

		AVStreamBlock* pBlock = new AVStreamBlock(streams[i], m_spStreamManager, this);
		pBlock->setPad(m_pad);
		pItemWidget->addBlock(pBlock);
		this->horizontalHeader()->setFixedWidth(std::max(this->width(), pItemWidget->width()));
		pItemWidget->show();
		this->setRowHeight(rowCnt + i, ls_perHeight);
		this->setCellWidget(rowCnt + i, 0, pItemWidget);
	}

	if (m_pPlayer->getDuration() > 0)
	{
		m_pPlayer->seekProp(m_pPlayer->getCurSecond() / m_pPlayer->getDuration());
	}

	this->setFixedHeight(this->rowCount() * ls_perHeight + 3);
	return true;
}

std::shared_ptr<StreamManager> TableStreams::getStreamManager()const
{
	return m_spStreamManager;
}

void TableStreams::resizeEvent(QResizeEvent* event)
{
	QTableWidget::resizeEvent(event);
	m_pSlider->modifyPos();
}

void TableStreams::onBackEndChanged(double end)
{
	double secEnd = m_spStreamManager->getSecEnd();
	if (secEnd == end)
		return;

	auto item = dynamic_cast<AVStreamItemWidget*>(sender());
	if (item == nullptr)
		return;

	if (end > secEnd)
	{
		m_pWidgetMaxLen = item;

		int width = m_spStreamManager->sizePerSecond() * end + m_pad;
		m_pSlider->setEndPos(width - m_pad);
		this->setFixedWidth(width);
		this->setColumnWidth(0, width);
		emit durationSecondChanged(end);
	}
	else if (m_pWidgetMaxLen == item)
	{
		for (int i = 0; i < this->rowCount(); ++i)
		{
			auto pItemWidget = dynamic_cast<AVStreamItemWidget*>(this->cellWidget(i, 0));
			if (pItemWidget != nullptr)
			{
				if (pItemWidget->endTime() > m_pWidgetMaxLen->endTime())
				{
					qDebug() << "结尾流更改！";
					m_pWidgetMaxLen = pItemWidget;

				}
			}
		}

		int width = m_spStreamManager->sizePerSecond() * end + m_pad;
		m_pSlider->setEndPos(m_spStreamManager->sizePerSecond() * end);
		emit durationSecondChanged(end);
	}
}

void TableStreams::onStreamFirstHas()
{
	if (!m_bManagerBind && m_pPlayer)
	{
		m_pPlayer->bindStreamManager(m_spStreamManager);
		m_bManagerBind = true;
	}
}

void TableStreams::onBlockTimeSet(AVStreamBlock* pBlock)
{
	if (pBlock)
	{
		m_pStreamBlockTimeWidget->showOpStreamBlock(pBlock);
		m_pStreamBlockTimeWidget->setFocus();
	}
}

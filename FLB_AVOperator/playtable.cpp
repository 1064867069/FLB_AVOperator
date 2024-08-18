#include "playtable.h"
#include "avfilemanager.h"
#include "str_utils.h"

#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

PlayTableBase* PlayTableBase::s_pCurPlayTable = nullptr;

PlayTableBase::PlayTableBase(QWidget* p) :QTableWidget(p)
{
	this->setColumnCount(1);
	this->horizontalHeader()->hide();
	this->verticalHeader()->hide();
}

void PlayTableBase::play_cancel()
{
	m_played = false;
}

void PlayTableBase::resizeEvent(QResizeEvent* event)
{
	QTableWidget::resizeEvent(event);
	this->setColumnWidth(0, this->width() - 2);
}

void PlayTableBase::addItem(int i, const QString& fp)
{
	auto fn = NsStr::filePath2Name(fp);
	QTableWidgetItem* item = new QTableWidgetItem(fn);

	item->setFlags(item->flags() & ~Qt::ItemIsEditable);

	this->setItem(i, 0, item);
}

PathPlayTable::PathPlayTable(IAVPathManager* pthManager, QWidget* p) :PlayTableBase(p), m_pPthManager(pthManager)
{
	this->initAVList();
	this->setFocusPolicy(Qt::NoFocus);

	m_pMenu = new QMenu(this);

	m_pActionPlay = m_pMenu->addAction("播放");
	connect(m_pActionPlay, &QAction::triggered, this, &PathPlayTable::onPlayTriggered);

	m_pActionDelFile = m_pMenu->addAction("移除");
	connect(m_pActionDelFile, &QAction::triggered, this, &PathPlayTable::delFile);
	//connect(this, &QTableWidget::customContextMenuRequested, this, &LocalPlayTable::onCustomContextMenu);

	connect(m_pPthManager, &IAVPathManager::newfp, this, &PathPlayTable::addNewFile);
}

void PathPlayTable::delFile()
{
	auto fileList = m_pPthManager->getPaths();
	if (m_selectRow >= 0 && m_selectRow < this->rowCount())
	{
		if (fileList[m_selectRow] == m_curPlayPath && s_pCurPlayTable == this)
		{
			QMessageBox::critical(this, "文件删除", "不允许删除正在播放的文件");
			return;
		}

		if (m_pPthManager->delFile(m_selectRow))
		{
			for (int i = m_selectRow; i < this->rowCount() - 1; ++i)
			{
				this->item(i, 0)->setText(this->item(i + 1, 0)->text());
			}
			this->setRowCount(this->rowCount() - 1);
			if (m_curPlayIndex > m_selectRow)
				--m_curPlayIndex;
		}
		m_selectRow = -1;
	}
}


void PathPlayTable::onCustomContextMenu(const QPoint& pos)
{
	QModelIndex index = this->indexAt(pos);//根据位置查找单元格

	if (index.isValid())
	{
		m_selectRow = index.row();
		m_pMenu->exec(mapToGlobal(pos));
	}

}

void PathPlayTable::addNewFile(int i)
{
	auto& paths = m_pPthManager->getPaths();
	auto& names = m_pPthManager->getNames();

	if (i < paths.size() && this->rowCount() == i)
	{
		this->setRowCount(i + 1);
		this->addItem(i, names[i]);
	}
}

void PathPlayTable::lastAVF()
{
	if (s_pCurPlayTable == this && m_played && m_curPlayIndex >= 0)
	{
		auto preplay = m_curPlayIndex;
		m_curPlayIndex = (m_curPlayIndex + this->rowCount() - 1) % this->rowCount();
		m_curPlayPath = m_pPthManager->getPaths()[m_curPlayIndex];

		this->setItemSelected(this->item(m_curPlayIndex, 0), true);
		this->setItemSelected(this->item(preplay, 0), false);
		emit playAVF(m_curPlayPath);
	}
}

void PathPlayTable::nextAVF()
{
	if (s_pCurPlayTable == this && m_played && m_curPlayIndex >= 0)
	{
		auto preplay = m_curPlayIndex;
		m_curPlayIndex = (m_curPlayIndex + this->rowCount() + 1) % this->rowCount();
		m_curPlayPath = m_pPthManager->getPaths()[m_curPlayIndex];

		this->setItemSelected(this->item(m_curPlayIndex, 0), true);
		this->setItemSelected(this->item(preplay, 0), false);
		emit playAVF(m_curPlayPath);
	}
}

void PathPlayTable::mouseDoubleClickEvent(QMouseEvent* event)
{
	PlayTableBase::mouseDoubleClickEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		QPoint p = event->pos();//双击位置
		QModelIndex index = this->indexAt(p);//根据位置查找单元格
		if (index.isValid())
		{
			this->playRowItem(index.row());
		}
	}

}

void PathPlayTable::onPlayTriggered()
{
	if (m_selectRow >= 0)
		this->playRowItem(m_selectRow);
}

void PathPlayTable::playRowItem(int r)
{
	m_curPlayIndex = r;

	m_curPlayPath = m_pPthManager->getPaths()[m_curPlayIndex];
	emit playAVF(m_curPlayPath);

	m_played = true;
	emit played();

	s_pCurPlayTable = this;
}


void PathPlayTable::mouseReleaseEvent(QMouseEvent* event)
{
	PlayTableBase::mouseReleaseEvent(event);
	if (event->button() == Qt::RightButton)
		this->onCustomContextMenu(event->pos());
}

void PathPlayTable::initAVList()
{
	auto& paths = m_pPthManager->getPaths();
	auto& names = m_pPthManager->getNames();

	this->setRowCount(paths.size());

	for (int i = 0; i < paths.size(); ++i)
	{
		this->addItem(i, names[i]);

		auto item = this->item(i, 0);
		item->setToolTip(paths[i]);
	}
}

#include "playtable.h"
#include "avfilemanager.h"
#include "str_utils.h"

#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

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
	item->setToolTip(fp);

	this->setItem(i, 0, item);
}

LocalPlayTable::LocalPlayTable(QWidget* p) :PlayTableBase(p)
{
	this->initAVList();

	m_pMenu = new QMenu(this);

	m_pActionPlay = m_pMenu->addAction("播放");
	connect(m_pActionPlay, &QAction::triggered, this, &LocalPlayTable::onPlayTriggered);

	m_pActionDelFile = m_pMenu->addAction("移除");
	connect(m_pActionDelFile, &QAction::triggered, this, &LocalPlayTable::delFile);
	//connect(this, &QTableWidget::customContextMenuRequested, this, &LocalPlayTable::onCustomContextMenu);

	auto& fm = AVFileManager::getInstance();
	connect(&fm, &AVFileManager::newfp, this, &LocalPlayTable::addNewFile);
}

void LocalPlayTable::delFile()
{
	auto fileList = AVFileManager::getInstance().getFileNamePaths();
	if (m_selectRow >= 0 && m_selectRow < this->rowCount())
	{
		if (fileList[m_selectRow] == m_curPlayPath)
		{
			QMessageBox::critical(this, "文件删除", "不允许删除正在播放的文件");
			return;
		}

		auto& fmanager = AVFileManager::getInstance();
		if (fmanager.delFile(m_selectRow))
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


void LocalPlayTable::onCustomContextMenu(const QPoint& pos)
{
	QModelIndex index = this->indexAt(pos);//根据位置查找单元格

	if (index.isValid())
	{
		m_selectRow = index.row();
		m_pMenu->exec(mapToGlobal(pos));
	}

}

void LocalPlayTable::addNewFile(int i)
{
	auto& fmanager = AVFileManager::getInstance();
	auto& paths = fmanager.getFileNamePaths();

	if (i < paths.size() && this->rowCount() == i)
	{
		int pos = paths[i].lastIndexOf("/");
		if (pos < 0)
			return;

		this->setRowCount(i + 1);
		this->addItem(i, paths[i]);
	}
}

void LocalPlayTable::lastAVF()
{
	if (m_played && m_curPlayIndex >= 0)
	{
		auto preplay = m_curPlayIndex;
		m_curPlayIndex = (m_curPlayIndex + this->rowCount() - 1) % this->rowCount();
		m_curPlayPath = AVFileManager::getInstance().getFileNamePaths()[m_curPlayIndex];

		this->setItemSelected(this->item(m_curPlayIndex, 0), true);
		this->setItemSelected(this->item(preplay, 0), false);
		emit playAVF(m_curPlayPath);
	}
}

void LocalPlayTable::nextAVF()
{
	if (m_played && m_curPlayIndex >= 0)
	{
		auto preplay = m_curPlayIndex;
		m_curPlayIndex = (m_curPlayIndex + this->rowCount() + 1) % this->rowCount();
		m_curPlayPath = AVFileManager::getInstance().getFileNamePaths()[m_curPlayIndex];

		this->setItemSelected(this->item(m_curPlayIndex, 0), true);
		this->setItemSelected(this->item(preplay, 0), false);
		emit playAVF(m_curPlayPath);
	}
}

void LocalPlayTable::mouseDoubleClickEvent(QMouseEvent* event)
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

void LocalPlayTable::onPlayTriggered()
{
	if (m_selectRow >= 0)
		this->playRowItem(m_selectRow);
}

void LocalPlayTable::playRowItem(int r)
{
	m_curPlayIndex = r;

	m_curPlayPath = AVFileManager::getInstance().getFileNamePaths()[m_curPlayIndex];
	emit playAVF(m_curPlayPath);

	m_played = true;
	emit played();
}


void LocalPlayTable::mouseReleaseEvent(QMouseEvent* event)
{
	PlayTableBase::mouseReleaseEvent(event);
	if (event->button() == Qt::RightButton)
		this->onCustomContextMenu(event->pos());
}

void LocalPlayTable::initAVList()
{
	auto& fmanager = AVFileManager::getInstance();
	auto& paths = fmanager.getFileNamePaths();

	this->setRowCount(paths.size());

	for (int i = 0; i < paths.size(); ++i)
	{
		auto fn = NsStr::filePath2Name(paths[i]);
		this->addItem(i, fn);

		auto item = this->item(i, 0);
		item->setToolTip(paths[i]);
	}
}

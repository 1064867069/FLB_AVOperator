#include "tableresource.h"
#include "str_utils.h"
#include "tablestreams.h"

#include <QMenu>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>

TableResource::TableResource(std::unique_ptr<IAVResourceFactory>&& srcFactory, QWidget* p)
	:QTableWidget(p), m_upAVRsrcFactory(std::move(srcFactory))
{
	this->initMenu();

	this->setColumnCount(1);
	this->horizontalHeader()->setVisible(false);
	this->verticalHeader()->setVisible(false);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QWidget::customContextMenuRequested, this, &TableResource::onCustomContextMenu);
}

void TableResource::initMenu()
{
	m_pMenu = new QMenu(this);

	m_pActionImport = new QAction("导入", this);
	m_pActionRemove = new QAction("移除", this);
	m_pActionAddNewStream = new QAction("新增音视频流", this);
	m_pMenu->addActions({ m_pActionImport, m_pActionRemove, m_pActionAddNewStream });

	connect(m_pActionImport, &QAction::triggered, this, &TableResource::importFile);
	connect(m_pActionRemove, &QAction::triggered, this, &TableResource::removeResource);
	connect(m_pActionAddNewStream, &QAction::triggered, this, &TableResource::addNewStream);
}

void TableResource::installTableStream(TableStreams* pTableStream)
{
	if (pTableStream)
	{
		if (m_pTableStream)
		{
			this->disconnect(m_pTableStream);
			this->onTableStreamChangeDestroyed();
		}

		m_pTableStream = pTableStream;
		connect(m_pTableStream, &QObject::destroyed, this, &TableResource::onTableStreamChangeDestroyed);
	}
}

void TableResource::onTableStreamChangeDestroyed()
{
	m_pTableStream = nullptr;
}

void TableResource::addNewItem(const QString& filePath)
{
	ResourceItem* pItem = ResourceItem::createNewItem(filePath);
	if (pItem != nullptr)
	{
		this->setRowCount(this->rowCount() + 1);
		this->setItem(this->rowCount() - 1, 0, pItem);
	}
}

void TableResource::onCustomContextMenu(const QPoint& pos)
{
	QModelIndex index = this->indexAt(pos);//根据位置查找单元格

	//m_pActionImport->setEnabled(!index.isValid());
	m_pActionRemove->setEnabled(index.isValid());
	m_pActionAddNewStream->setEnabled(index.isValid());

	m_pMenu->exec(this->viewport()->mapToGlobal(pos));
}

void TableResource::importFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "",
		QString("%1(%2);").arg(m_upAVRsrcFactory->title()).arg(m_upAVRsrcFactory->fileFilter()));

	this->addNewItem(filePath);
}

void TableResource::removeResource()
{
	auto indexs = this->selectedIndexes();
	if (indexs.size() > 0)
	{
		auto index = indexs[0];
		auto resrcItem = dynamic_cast<ResourceItem*>(this->item(index.row(), index.column()));
		if (resrcItem != nullptr)
			emit filePathWouldRemoved(resrcItem->getFilePath());

		this->removeRow(index.row());
	}
}

void TableResource::addNewStream()
{
	auto items = this->selectedItems();
	if (items.size() == 0)
		return;

	auto item = dynamic_cast<ResourceItem*>(items[0]);
	if (item != nullptr)
	{
		QString path = item->getFilePath();
		auto streams = m_upAVRsrcFactory->createStreams(path);
		if (!m_pTableStream->addStreams(streams))
		{
			QMessageBox::critical(this, "资源导入失败", QString("资源文件%1导入失败！").arg(path));
		}
	}
}

ResourceItem::ResourceItem(const QString& path) :QTableWidgetItem()
{
	this->setFlags(this->flags() & ~Qt::ItemIsEditable);

	QFile file(path);
	if (file.exists())
	{
		m_filePath = path;
		this->setText(NsStr::filePath2Name(m_filePath));
	}
}

ResourceItem* ResourceItem::createNewItem(const QString& filePath)
{
	auto res = new ResourceItem(filePath);
	if (res->getFilePath() == "")
	{
		delete res;
		res = nullptr;
	}

	return res;
}

const QString& ResourceItem::getFilePath()const
{
	return m_filePath;
}

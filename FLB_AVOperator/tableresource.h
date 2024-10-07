#ifndef TABLESOURCE_H
#define TABLESOURCE_H

#include <QTabWidget>
#include <QTableWidget>

#include "interfaces.h"

class TableStreams;
class TableResource : public QTableWidget
{
	Q_OBJECT
public:
	explicit TableResource(std::unique_ptr<IAVResourceFactory>&& srcFactory, QWidget* p = nullptr);

	void installTableStream(TableStreams* pTableStream);

private:
	void onCustomContextMenu(const QPoint& pos);

	void initMenu();

	void addNewItem(const QString& filePath);

private slots:
	void importFile();

	void removeResource();

	void addNewStream();

	void onTableStreamChangeDestroyed();

signals:
	void filePathWouldRemoved(QString);
private:
	std::unique_ptr<IAVResourceFactory> m_upAVRsrcFactory;
	TableStreams* m_pTableStream = nullptr;

	QMenu* m_pMenu;
	QAction* m_pActionImport;
	QAction* m_pActionRemove;
	QAction* m_pActionAddNewStream;
};

class ResourceItem :public QTableWidgetItem
{
public:
	static ResourceItem* createNewItem(const QString& filePath);

	const QString& getFilePath()const;
private:
	QString m_filePath;

	explicit ResourceItem(const QString& path);
};

#endif // !TABWIDGETSOURCES_H


#ifndef AVFILEMANAGER_H
#define AVFILEMANAGER_H

#include <qglobal.h>
#include <QObject>
#include <QMap>
#include <QList>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>

class AVFileManager : public QObject
{
	Q_OBJECT
public:
	static AVFileManager& getInstance();

public:
	const QStringList& getFileNamePaths()const;

	void setDlgParent(QWidget*);

public slots:
	void importFile();

	bool delFile(int);

signals:
	void newfp(int);

private:
	AVFileManager();
	AVFileManager(const AVFileManager&) = delete;
	AVFileManager& operator=(const AVFileManager&) = delete;

	void initFnPath();
private:
	QStringList m_listFPath;

	QWidget* m_parent = nullptr;
};


class KFPSQLiteManager
{
public:
	static KFPSQLiteManager& getInstance();

	static const QString& tableName();
public:
	bool initDataBase(const QString& dbPath);
	bool openSQLiteDb(const QString& dbPath);
	void closeSQLiteDb();

	bool createUserTable();
	bool insertPath(const QString&);
	QStringList selectAll();
	bool deleteFilePath(const QString&);
private:
	KFPSQLiteManager() = default;
	~KFPSQLiteManager();

	KFPSQLiteManager(const KFPSQLiteManager&) = delete;
	KFPSQLiteManager& operator=(const KFPSQLiteManager&) = delete;

private:
	QSqlDatabase m_dbSqlite;
};

#endif

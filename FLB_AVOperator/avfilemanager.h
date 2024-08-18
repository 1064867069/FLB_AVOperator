#ifndef AVFILEMANAGER_H
#define AVFILEMANAGER_H

#include <qglobal.h>
#include <QObject>
#include <QMap>
#include <QList>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>

#include "interfaces.h"

class AVFileManager : public IAVPathManager
{
	Q_OBJECT
public:
	static AVFileManager& getInstance();

public slots:
	virtual void importPath()override;
	virtual bool delFile(int)override;
private:
	AVFileManager();
	AVFileManager(const AVFileManager&) = delete;
	AVFileManager& operator=(const AVFileManager&) = delete;

	void initFnPath();
};

class AVUrlManager : public IAVPathManager
{
	Q_OBJECT
public:
	static AVUrlManager& getInstance();

public slots:
	virtual void importPath()override;
	virtual bool delFile(int)override;
private:
	AVUrlManager();
	AVUrlManager(const AVUrlManager&) = delete;
	AVUrlManager& operator=(const AVUrlManager&) = delete;

	void initUrlPath();
};

class KFPSQLiteManager
{
public:
	static KFPSQLiteManager& getInstance();

	//static const QString& tableName();
public:
	bool initDataBase(const QString& dbPath);
	bool openSQLiteDb(const QString& dbPath);
	void closeSQLiteDb();

	bool createUserTable(const QString&);
	bool insertPath(const QString&, const QString&);
	QStringList selectAll(const QString&);
	bool deleteFilePath(const QString&, const QString&);

	bool valid()const;
private:
	KFPSQLiteManager();
	~KFPSQLiteManager();

	KFPSQLiteManager(const KFPSQLiteManager&) = delete;
	KFPSQLiteManager& operator=(const KFPSQLiteManager&) = delete;

private:
	QSqlDatabase m_dbSqlite;
	bool m_valid = false;
};

#endif

#include "avfilemanager.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

AVFileManager& AVFileManager::getInstance()
{
	static AVFileManager ls_fileManager;
	return ls_fileManager;
}

AVFileManager::AVFileManager()
{
	auto& sqlManager = KFPSQLiteManager::getInstance();
	sqlManager.initDataBase("avf.db");
	this->initFnPath();
}

const QStringList& AVFileManager::getFileNamePaths()const
{
	return m_listFPath;
}

bool AVFileManager::delFile(int i)
{
	if (i < 0 || i >= m_listFPath.size())
		return false;

	auto& sqlManager = KFPSQLiteManager::getInstance();
	bool succ = sqlManager.deleteFilePath(m_listFPath[i]);
	if (!succ)
	{
		QMessageBox::critical(m_parent, "文件移除", "移除失败！");
	}
	else
	{
		m_listFPath.removeAll(m_listFPath[i]);
	}
	return succ;
}

void AVFileManager::importFile()
{
	QString filePath = QFileDialog::getOpenFileName(m_parent, "选择文件", "",
		"Audio Files(*.mp3 *.wav *.aac *.flac);; Video Files(*.mp4 *.mkv);;");
	if (filePath != "")
	{
		auto& sqlManager = KFPSQLiteManager::getInstance();
		/*QString fn = filePath.mid(filePath.lastIndexOf("/") + 1);
		qDebug() << filePath << fn;*/
		if (sqlManager.insertPath(filePath))
		{
			m_listFPath.append(filePath);
			emit newfp(m_listFPath.size() - 1);
		}
	}
}

void AVFileManager::setDlgParent(QWidget* p)
{
	m_parent = p;
}

void AVFileManager::initFnPath()
{
	m_listFPath.clear();

	auto& sqlManager = KFPSQLiteManager::getInstance();
	QStringList temp = sqlManager.selectAll();
	for (const auto& t : temp)
	{
		QFile f(t);
		if (f.exists())
		{
			m_listFPath.append(t);
		}
		else
		{
			sqlManager.deleteFilePath(t);
		}
	}
}


KFPSQLiteManager& KFPSQLiteManager::getInstance()
{
	static KFPSQLiteManager ls_instance;
	return ls_instance;
}

const QString& KFPSQLiteManager::tableName()
{
	static const QString ls_tbName = "av_file_path";
	return ls_tbName;
}

KFPSQLiteManager::~KFPSQLiteManager()
{
	this->closeSQLiteDb();
}

bool KFPSQLiteManager::initDataBase(const QString& dbPath)
{
	this->closeSQLiteDb();

	if (!this->openSQLiteDb(dbPath))
		return false;

	if (!this->createUserTable())
		return false;

	return true;
}

bool KFPSQLiteManager::openSQLiteDb(const QString& dbPath)
{
	m_dbSqlite = QSqlDatabase::addDatabase("QSQLITE");
	m_dbSqlite.setDatabaseName(dbPath);

	bool ok = m_dbSqlite.open();
	if (!ok)
	{
		qDebug() << "Failed  to open database : " << m_dbSqlite.lastError().text();
		return false;
	}

	return true;
}

void KFPSQLiteManager::closeSQLiteDb()
{
	if (m_dbSqlite.isOpen())
		m_dbSqlite.close();
}

// 创建表
bool KFPSQLiteManager::createUserTable()
{
	QSqlQuery query;
	QString SQL = QString("CREATE TABLE IF NOT EXISTS %1 (filepath TEXT PRIMARY KEY);").arg(KFPSQLiteManager::tableName());

	bool ok = query.exec(SQL);
	if (!ok)
	{
		qDebug() << __FUNCTION__ << __LINE__ << " Failed to execute SQL statement: " << query.lastError().text();
		return false;
	}
	return true;
}

bool KFPSQLiteManager::insertPath(const QString& path)
{
	QSqlQuery query;
	//QString SQL = "INSERT INTO ?(filepath) VALUES ('?');";

	//query.prepare(SQL);//预编译
	//query.addBindValue(KFPSQLiteManager::tableName());
	//query.addBindValue(path);//给占位符绑定变量

	QString sql = QString("INSERT INTO %1(filepath) VALUES ('%2');").arg(KFPSQLiteManager::tableName()).arg(path);
	bool ok = query.exec(sql);// 执行 SQL 
	if (!ok)
	{
		qDebug() << query.lastQuery();
		qDebug() << __FUNCTION__ << __LINE__ << " Failed to execute SQL statement: " << query.lastError().text();
		return false;
	}
	return true;
}

QStringList KFPSQLiteManager::selectAll()
{
	QList<QString> result;
	QString querySQL = "SELECT * FROM %1;";
	querySQL = querySQL.arg(KFPSQLiteManager::tableName());

	QSqlQuery query;
	bool ok = query.exec(querySQL);
	if (!ok)
	{
		qDebug() << "Failed to execute query: " << query.lastError().text();
		return result;
	}

	while (query.next())
	{
		result.append(query.value(0).toString());
	}

	return result;
}

bool KFPSQLiteManager::deleteFilePath(const QString& path)
{
	QString sql = "DELETE FROM %1 WHERE filepath='%2';";
	sql = sql.arg(KFPSQLiteManager::tableName()).arg(path);
	sql = sql.replace("\\", "\\\\");

	QSqlQuery query;
	bool ok = query.exec(sql);
	if (!ok)
	{
		qDebug() << "Failed to execute query: " << query.lastError().text();
		qDebug() << sql;
	}

	return ok;
}


#include "avfilemanager.h"
#include "str_utils.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QInputDialog>


void IAVPathManager::setDlgParent(QWidget* p)
{
	m_parent = p;
}

const QStringList& IAVPathManager::getPaths()const
{
	return m_listFPath;
}

const QStringList& IAVPathManager::getNames()const
{
	return m_listFName;
}

AVFileManager& AVFileManager::getInstance()
{
	static AVFileManager ls_fileManager;
	return ls_fileManager;
}

AVFileManager::AVFileManager()
{
	m_sqlTable = "av_file_path";
	auto& sqlManager = KFPSQLiteManager::getInstance();
	if (sqlManager.createUserTable(m_sqlTable))
		this->initFnPath();
}

bool AVFileManager::delFile(int i)
{
	if (i < 0 || i >= m_listFPath.size())
		return false;

	auto& sqlManager = KFPSQLiteManager::getInstance();
	bool succ = sqlManager.deleteFilePath(m_sqlTable, m_listFPath[i]);
	if (!succ)
	{
		QMessageBox::critical(m_parent, "文件移除", "移除失败！");
	}
	else
	{
		m_listFPath.removeAt(i);
		m_listFName.removeAt(i);
	}
	return succ;
}

void AVFileManager::importPath()
{
	QString filePath = QFileDialog::getOpenFileName(m_parent, "选择文件", "",
		"Audio Files(*.mp3 *.wav *.aac *.flac);; Video Files(*.mp4 *.mkv *.flv);;");
	if (filePath != "")
	{
		auto& sqlManager = KFPSQLiteManager::getInstance();
		/*QString fn = filePath.mid(filePath.lastIndexOf("/") + 1);
		qDebug() << filePath << fn;*/
		if (sqlManager.insertPath(m_sqlTable, filePath))
		{
			m_listFPath.append(filePath);
			m_listFName.append(NsStr::filePath2Name(filePath));
			emit newfp(m_listFPath.size() - 1);
		}
	}
}

void AVFileManager::initFnPath()
{
	m_listFPath.clear();

	auto& sqlManager = KFPSQLiteManager::getInstance();
	QStringList temp = sqlManager.selectAll(m_sqlTable);
	for (const auto& t : temp)
	{
		QFile f(t);
		if (f.exists())
		{
			m_listFPath.append(t);
			m_listFName.append(f.fileName());
		}
		else
		{
			sqlManager.deleteFilePath(m_sqlTable, t);
		}
	}
}

AVUrlManager& AVUrlManager::getInstance()
{
	static AVUrlManager ls_urlManager;
	return ls_urlManager;
}

AVUrlManager::AVUrlManager()
{
	m_sqlTable = "av_url_path";
	auto& sqlManager = KFPSQLiteManager::getInstance();
	if (sqlManager.createUserTable(m_sqlTable))
		this->initUrlPath();
}

void AVUrlManager::importPath()
{
	QString url_str = QInputDialog::getText(m_parent, "输入网址", "输入网址");
	QUrl url(url_str);
	if (!url.isValid())
	{
		QMessageBox::critical(m_parent, "输入网址", "输入的网址不合法！");
		return;
	}

	QString url_name = QInputDialog::getText(m_parent, "输入网址", "输入标签");
	if (url_name == "")
		return;

	auto& sqlManager = KFPSQLiteManager::getInstance();
	if (sqlManager.insertPath(m_sqlTable, url_name + NsStr::sepNameUrl() + url_str))
	{
		m_listFPath.append(url_str);
		m_listFName.append(url_name);
		emit newfp(m_listFPath.size() - 1);
	}
}

bool AVUrlManager::delFile(int i)
{
	if (i < 0 || i >= m_listFPath.size())
		return false;

	auto& sqlManager = KFPSQLiteManager::getInstance();
	bool succ = sqlManager.deleteFilePath(m_sqlTable, m_listFName[i] + NsStr::sepNameUrl() + m_listFPath[i]);
	if (!succ)
	{
		QMessageBox::critical(m_parent, "文件移除", "移除失败！");
	}
	else
	{
		m_listFPath.removeAt(i);
		m_listFName.removeAt(i);
	}
	return succ;
}

void AVUrlManager::initUrlPath()
{
	m_listFPath.clear();

	auto& sqlManager = KFPSQLiteManager::getInstance();
	QStringList temp = sqlManager.selectAll(m_sqlTable);
	for (const auto& t : temp)
	{
		m_listFPath.append(t.split(NsStr::sepNameUrl())[1]);
		m_listFName.append(t.split(NsStr::sepNameUrl())[0]);
	}
}

KFPSQLiteManager::KFPSQLiteManager()
{
	m_valid = this->initDataBase("avf.db");
}

KFPSQLiteManager& KFPSQLiteManager::getInstance()
{
	static KFPSQLiteManager ls_instance;
	return ls_instance;
}

//const QString& KFPSQLiteManager::tableName()
//{
//	static const QString ls_tbName = "av_file_path";
//	return ls_tbName;
//}

KFPSQLiteManager::~KFPSQLiteManager()
{
	this->closeSQLiteDb();
}

bool KFPSQLiteManager::initDataBase(const QString& dbPath)
{
	this->closeSQLiteDb();

	if (!this->openSQLiteDb(dbPath))
		return false;

	/*if (!this->createUserTable())
		return false;*/

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
bool KFPSQLiteManager::createUserTable(const QString& table)
{
	QSqlQuery query;
	QString SQL = QString("CREATE TABLE IF NOT EXISTS %1 (filepath TEXT PRIMARY KEY);").arg(table);

	bool ok = query.exec(SQL);
	if (!ok)
	{
		qDebug() << __FUNCTION__ << __LINE__ << " Failed to execute SQL statement: " << query.lastError().text();
		return false;
	}
	return true;
}

bool KFPSQLiteManager::insertPath(const QString& table, const QString& path)
{
	QSqlQuery query;
	//QString SQL = "INSERT INTO ?(filepath) VALUES ('?');";

	//query.prepare(SQL);//预编译
	//query.addBindValue(KFPSQLiteManager::tableName());
	//query.addBindValue(path);//给占位符绑定变量

	QString pathStr = "";
	QChar pre = '\0';
	for (auto ch : path)
	{
		if ((ch == '\'') && pre != '\\')
			pathStr.append("\'");
		pathStr.append(ch);
		pre = ch;
	}

	QString sql = QString("INSERT INTO %1(filepath) VALUES ('%2');").arg(table).arg(pathStr);
	bool ok = query.exec(sql);// 执行 SQL 
	if (!ok)
	{
		qDebug() << query.lastQuery();
		qDebug() << __FUNCTION__ << __LINE__ << " Failed to execute SQL statement: " << query.lastError().text();
		return false;
	}
	return true;
}

QStringList KFPSQLiteManager::selectAll(const QString& table)
{
	QList<QString> result;
	QString querySQL = "SELECT * FROM %1;";
	querySQL = querySQL.arg(table);

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

bool KFPSQLiteManager::deleteFilePath(const QString& table, const QString& path)
{
	QString sql = "DELETE FROM %1 WHERE filepath='%2';";
	sql = sql.arg(table).arg(path);
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

bool KFPSQLiteManager::valid()const
{
	return m_valid;
}

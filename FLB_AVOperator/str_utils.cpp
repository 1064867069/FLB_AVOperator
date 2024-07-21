#include "str_utils.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

QString NsStr::filePath2Name(const QString& path)
{
	QFileInfo fInf(path);
	return fInf.fileName();
}

const QString& NsStr::cacheDir()
{
	static const QString ls_dir = "cache";
	QDir dir(ls_dir);
	if (!dir.exists())
	{
		bool r = QDir().mkdir(ls_dir);
		qDebug() << r;
	}

	return ls_dir;
}

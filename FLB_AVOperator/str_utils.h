#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <QString>

namespace NsStr
{
	QString filePath2Name(const QString&);

	const QString& cacheDir();

	const QString& sepNameUrl();
};

#endif // !STR_UTILS_H


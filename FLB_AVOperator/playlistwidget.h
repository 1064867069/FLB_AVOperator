#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "interfaces.h"

class PathPlayTable;

class PlayListWidget :public QWidget
{
	Q_OBJECT
public:
	explicit PlayListWidget(IAVPathManager* pm, QWidget* p = nullptr);

	const PathPlayTable* getPlayTable()const;


private:
	PathPlayTable* m_pPlayTable;
	QPushButton* m_pButtonAddFile;
};

#endif // !PLAYLISTWIDGET_H


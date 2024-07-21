#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>
#include <QPushButton>

class LocalPlayTable;

class PlayListWidget :public QWidget
{
	Q_OBJECT
public:
	PlayListWidget(QWidget* p = nullptr);

	const LocalPlayTable* getPlayTable()const;


private:
	LocalPlayTable* m_pPlayTable;
	QPushButton* m_pButtonAddFile;
};

#endif // !PLAYLISTWIDGET_H


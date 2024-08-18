#ifndef PLAYTABLE_H
#define PLAYTABLE_H

#include <QTableWidget>

#include "interfaces.h"

//主要用于播放列表代码的一些复用
class PlayTableBase :public QTableWidget
{
	Q_OBJECT
public:
	explicit PlayTableBase(QWidget* p = nullptr);

public slots:
	virtual void lastAVF() = 0;

	virtual void nextAVF() = 0;

	void play_cancel();
protected slots:
	virtual void resizeEvent(QResizeEvent* event)Q_DECL_OVERRIDE;

signals:
	void played();

protected:
	void addItem(int, const QString&);

protected:
	QString m_curPlayPath;

	int m_curPlayIndex = -1;

	bool m_played = false;

	static PlayTableBase* s_pCurPlayTable;
};

class PathPlayTable :public PlayTableBase
{
	Q_OBJECT
public:
	explicit PathPlayTable(IAVPathManager* pthManager, QWidget* p = nullptr);

public slots:
	//严格依赖IAVPathManager
	void addNewFile(int);

	virtual void lastAVF()Q_DECL_OVERRIDE;

	virtual void nextAVF()Q_DECL_OVERRIDE;

protected slots:
	//virtual void resizeEvent(QResizeEvent* event)override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event)Q_DECL_OVERRIDE;

	virtual void mouseReleaseEvent(QMouseEvent*)Q_DECL_OVERRIDE;

	void delFile();

	void onCustomContextMenu(const QPoint& pos);

	void onPlayTriggered();

signals:
	void playAVF(QString);

private:
	void initAVList();

	void playRowItem(int);

	//void addItem(int, const QString&);

private:
	IAVPathManager* m_pPthManager;
	QMenu* m_pMenu = nullptr;

	QAction* m_pActionPlay;
	QAction* m_pActionDelFile;


	int m_selectRow = -1;
};


#endif // !PLAYTABLE_H


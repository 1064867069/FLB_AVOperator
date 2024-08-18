#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_flb_avoperator.h"

class VideoOpenGLPlayer;

class PlayListWidget;

class FLB_AVOperator : public QMainWindow
{
	Q_OBJECT

public:
	FLB_AVOperator(QWidget* parent = Q_NULLPTR);
	virtual ~FLB_AVOperator();

protected:
	//void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;

private:
	void setDockPlayList();

private slots:
	void openFile(QString fp);
	//void openUrl(QString url);

	void onFullClicked();
private:
	VideoOpenGLPlayer* m_pVideoWidget;

	QTabWidget* m_pTabPlayList;

	PlayListWidget* m_pLocalPlayList;
	PlayListWidget* m_pOnlinePlayList;
private:
	Ui::FLB_AVOperatorClass ui;
};

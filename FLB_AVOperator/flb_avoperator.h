#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_flb_avoperator.h"
#include <QPushButton>

class VideoOpenGLPlayer;
class AVEditWidget;
class PlayListWidget;

class FLB_AVOperator : public QMainWindow
{
	Q_OBJECT
public:
	static FLB_AVOperator* getWindowPtr();

public:
	FLB_AVOperator(QWidget* parent = Q_NULLPTR);
	virtual ~FLB_AVOperator();

protected:
	//void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;

private:
	void setDockPlayList();

	void initWidgets();

private slots:
	void openFile(QString fp);
	//void openUrl(QString url);

	void onFullClicked();

	void onPlayerShow();

	void onEditorShow();
private:
	VideoOpenGLPlayer* m_pVideoWidget;
	QDockWidget* m_pDockPlay;
	QTabWidget* m_pTabPlayList;
	PlayListWidget* m_pLocalPlayList;
	PlayListWidget* m_pOnlinePlayList;

	AVEditWidget* m_pEditWidget;

	QPushButton* m_pBtnPlayer;
	QPushButton* m_pBtnEditor;
private:
	static FLB_AVOperator* s_pMainWindow;

	Ui::FLB_AVOperatorClass ui;
};

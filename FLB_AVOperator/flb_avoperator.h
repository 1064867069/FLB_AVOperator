#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_flb_avoperator.h"

class VideoOpenGLPlayer;

class FLB_AVOperator : public QMainWindow
{
	Q_OBJECT

public:
	FLB_AVOperator(QWidget* parent = Q_NULLPTR);
	virtual ~FLB_AVOperator();

private:
	VideoOpenGLPlayer* m_pVideoWidget;

private:
	Ui::FLB_AVOperatorClass ui;
};

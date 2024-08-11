#ifndef DLGVIDEOPARAM_H
#define DLGVIDEOPARAM_H

#include "ui_dlgvideoparam.h"

class VideoBrightAdjust;
class VideoProcessList;
class VideoChromAdjust;
class VideoContrastAdjust;

class DlgVideoParam :public QDialog
{
	Q_OBJECT
public:
	DlgVideoParam(VideoProcessList*, QWidget* p = nullptr);

private slots:
	void onBrightChanged(int);
	void onChromChanged(int);
	void onContrastChanged(int);

signals:
	void paramUpdated();

private:
	std::shared_ptr<VideoBrightAdjust> m_spVideoBrightProc;
	std::shared_ptr<VideoChromAdjust> m_spVideoChromProc;
	std::shared_ptr<VideoContrastAdjust> m_spVideoContrastProc;

	Ui::DlgVideoParam ui;
};
#endif



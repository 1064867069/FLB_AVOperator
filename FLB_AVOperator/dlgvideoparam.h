#ifndef DLGVIDEOPARAM_H
#define DLGVIDEOPARAM_H

#include "ui_dlgvideoparam.h"

class VideoBrightAdjust;
class VideoProcessList;
class VideoChromAdjust;
class VideoContrastAdjust;
class VideoClrTempAdjust;

class DlgVideoParam :public QDialog
{
	Q_OBJECT
public:
	DlgVideoParam(VideoProcessList*, QWidget* p = nullptr);

private:
	void sliderBind();

private slots:
	void onSliderChanged(int);

signals:
	void paramUpdated();

private:
	std::shared_ptr<VideoBrightAdjust> m_spVideoBrightProc;
	std::shared_ptr<VideoChromAdjust> m_spVideoChromProc;
	std::shared_ptr<VideoContrastAdjust> m_spVideoContrastProc;
	std::shared_ptr<VideoClrTempAdjust> m_spVideoClrTempProc;

	QHash<QSlider*, QLabel*> m_hashSliderLabel;
	Ui::DlgVideoParam ui;
};
#endif



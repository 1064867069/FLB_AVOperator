#ifndef AVEDITWIDGET_H
#define AVEDITWIDGET_H

#include "ui_aveditwidget.h"
#include <QGroupBox>
#include <QPushButton>

class VideoOpenGLPlayer;
class FAVPlayer;
class TableStreams;
class TableResource;
class IAVResourceFactory;
enum class PlayState;

class AVEditWidget : public QWidget
{
	Q_OBJECT
public:
	explicit AVEditWidget(QWidget* p = nullptr);

private:
	void initWidget();

	void initBtmButton();

	void addTableResource(std::unique_ptr<IAVResourceFactory> upResrcFactory);
private slots:
	void onPlayClick();

	void onExport();

	void onPlayStateChanged(PlayState state);
private:
	VideoOpenGLPlayer* m_pVideoPlayer;
	FAVPlayer* m_pPlayer;

	TableResource* m_pTableResourceVideo;
	TableStreams* m_pTableStreams;

	QGroupBox* m_pParamBox;
	QPushButton* m_pBtnPlay;
	QPushButton* m_pBtnExport;

	Ui::AVEditWidget ui;
};



#endif

#ifndef PLAYBTMBAR_H
#define PLAYBTMBAR_H

#include "ui_playbtmbar.h"
#include <QLabel>

class SquareButton;
class FAVPlayer;
class SliderController;
class LabelTimeStampObserver;
class VolumeWidget;
class SpeedCombobox;

class PlayBtmBar :public QWidget
{
	Q_OBJECT
public:
	explicit PlayBtmBar(FAVPlayer* pObs, QWidget* ps, QWidget* p = nullptr);

	virtual ~PlayBtmBar();

	bool isSliderPressed()const;

	bool isMouseEntered()const;
public slots:
	void setPlayState(bool);

	void onEventType(QEvent::Type);

	void onFullScreen();

	void onPlayPauseClicked();
protected slots:
	void paintEvent(QPaintEvent* event)Q_DECL_OVERRIDE;

	void onVolClicked();

	void onVolChanged(int);
private:
	void initBtmBtns();

	void initSlider();

	void initTimeStamp();

	void initLayout();

signals:
	void playClicked();

	void pauseClicked();

	void lastClicked();

	void nextClicked();

	void fullClicked();

	void setClicked();
private:
	FAVPlayer* m_pPlayer;

	SliderController* m_pSliderController;

	QWidget* m_pScreenWidget;

	QLabel* m_pLabelTimestamp;
	LabelTimeStampObserver* m_pLabelTimeObserver;

	SquareButton* m_pBtnPlayPause;

	SquareButton* m_pBtnPlayLast;
	SquareButton* m_pBtnPlayNext;

	SquareButton* m_pBtnForward;
	SquareButton* m_pBtnBackward;

	SquareButton* m_pBtnStop;

	VolumeWidget* m_pVolWidget;
	SquareButton* m_pBtnVolume;

	SpeedCombobox* m_pCmbBoxSpeed;

	SquareButton* m_pBtnSet;

	SquareButton* m_pBtnFull;

	QIcon m_iconPlay;
	QIcon m_iconPause;

	QIcon m_iconFull;
	QIcon m_iconNotFull;

	QIcon m_iconSilence;
	QIcon m_iconVolume;

	int m_preVol = 100;
	bool m_isEntered = false;
	bool m_isPlay = false;
	bool m_volShown = false;
private:
	Ui::PlayBtmBar ui;
};


#endif // !PLAYBTMBAR_H


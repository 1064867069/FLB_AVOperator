#ifndef PLAYBTMBAR_H
#define PLAYBTMBAR_H

#include "ui_playbtmbar.h"
#include <QLabel>

class SquareButton;
class FAVPlayer;
class SliderController;
class LabelTimeStampObserver;

class PlayBtmBar :public QWidget
{
	Q_OBJECT
public:
	explicit PlayBtmBar(FAVPlayer* pObs, QWidget* p = nullptr);

	virtual ~PlayBtmBar();

	bool isSliderPressed()const;

	bool isMouseEntered()const;
public slots:
	void setPlayState(bool);

	void onEventType(QEvent::Type);
protected slots:
	void onPlayPauseClicked();

private:
	void initBtmWidget();

	void initSlider();

	void initTimeStamp();

	void initLayout();

signals:
	void playClicked();

	void pauseClicked();

	void lastClicked();

	void nextClicked();

private:
	FAVPlayer* m_pPlayer;

	SliderController* m_pSliderController;

	QLabel* m_pLabelTimestamp;
	LabelTimeStampObserver* m_pLabelTimeObserver;

	SquareButton* m_pBtnPlayPause;
	SquareButton* m_pBtnPlayLast;
	SquareButton* m_pBtnPlayNext;
	SquareButton* m_pBtnStop;

	QIcon m_iconPlay;
	QIcon m_iconPause;

	bool m_isEntered = false;
	bool m_isPlay = false;
private:
	Ui::PlayBtmBar ui;
};


#endif // !PLAYBTMBAR_H


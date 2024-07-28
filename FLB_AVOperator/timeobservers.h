#ifndef TIMEOBSERVERS_H
#define TIMEOBSERVERS_H

#include "interfaces.h"

#include <QSlider>
#include <QLabel>

class SliderController : public ITimeObserver
{
	Q_OBJECT
public:
	SliderController(FAVPlayer* obj, QSlider* p);

	bool isPressed()const;


protected slots:
	virtual void onSecondChanged(double)Q_DECL_OVERRIDE;

	virtual	void onDurationChanged(double)Q_DECL_OVERRIDE;

	void onSliderPressed();

	void onSliderReleased();

	void resetSlider();

	void onAVStop();

signals:
	void seekProp(double);
private:
	QSlider* m_pSliderObj = nullptr;

	bool m_pressed = false;
};

class LabelTimeStampObserver : public ITimeObserver
{
	Q_OBJECT
public:
	LabelTimeStampObserver(FAVPlayer* obj, QLabel* p);

public slots:
	void resetStamp();

protected slots:
	virtual void onSecondChanged(double)Q_DECL_OVERRIDE;

	virtual	void onDurationChanged(double)Q_DECL_OVERRIDE;


private:
	QLabel* m_pLabel;

	QString m_curText = "00:00";
	QString m_durText = "00:00";
};

#endif // !TIMEOBSERVERS_H


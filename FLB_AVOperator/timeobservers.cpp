#include "timeobservers.h"
#include "avoperator.h"


SliderController::SliderController(FAVPlayer* obj, QSlider* p) : ITimeObserver(obj, p), m_pSliderObj(p)
{
	m_pSliderObj->setMaximum(1000);

	connect(m_pSliderObj, &QSlider::sliderPressed, this, &SliderController::onSliderPressed);
	connect(m_pSliderObj, &QSlider::sliderReleased, this, &SliderController::onSliderReleased);
	connect(obj, &FAVPlayer::stopped, this, &SliderController::onAVStop);
}

bool SliderController::isPressed()const
{
	return m_pressed;
}

void SliderController::onSliderPressed()
{
	m_pressed = true;
}

void SliderController::onSliderReleased()
{
	emit seekProp(static_cast<double>(m_pSliderObj->value() - m_pSliderObj->minimum()) /
		m_pSliderObj->maximum() - m_pSliderObj->minimum());
	m_pressed = false;
}

void SliderController::onDurationChanged(double d)
{
	if (m_pressed || d <= 0)
	{
		return;
	}

	m_duration = d;
	this->resetSlider();
}

void SliderController::onSecondChanged(double s)
{
	if (m_pressed || s < 0 || m_duration <= 0)
		return;

	m_second = s;
	this->resetSlider();
}

void SliderController::resetSlider()
{
	if (m_pSliderObj == nullptr)
		return;

	if (m_duration <= 0 || m_second < 0)
	{
		m_pSliderObj->setValue(m_pSliderObj->minimum());
		m_pSliderObj->setEnabled(false);
		return;
	}

	m_pSliderObj->setEnabled(true);

	int gap = m_pSliderObj->maximum() - m_pSliderObj->minimum();
	m_pSliderObj->setValue(m_pSliderObj->minimum() + gap * m_second / m_duration);
}

void SliderController::onAVStop()
{
	m_second = -1;
	m_duration = 0;
	this->resetSlider();
}

QString getTimeTxt(int d)
{
	return QString("%1:%2").arg(d / 60, 2, 10, QLatin1Char('0')).arg(d % 60, 2, 10, QLatin1Char('0'));
}

LabelTimeStampObserver::LabelTimeStampObserver(FAVPlayer* obj, QLabel* p) :ITimeObserver(obj, p), m_pLabel(p)
{
	if (m_pLabel != nullptr)
		m_pLabel->setAlignment(Qt::AlignCenter);

	this->resetStamp();
	connect(obj, &FAVPlayer::stopped, this, &LabelTimeStampObserver::resetStamp);
}

void LabelTimeStampObserver::resetStamp()
{
	this->onDurationChanged(0);
	this->onSecondChanged(0);
}

void LabelTimeStampObserver::onSecondChanged(double sec)
{
	sec = round(sec);
	if (sec == m_second || sec < 0 || m_duration <= 0)
		return;

	m_second = sec;
	m_curText = getTimeTxt(m_second);
	if (m_pLabel != nullptr)
	{
		m_pLabel->setText(m_curText + "/" + m_durText);
	}
}

void LabelTimeStampObserver::onDurationChanged(double dur)
{
	dur = round(dur);
	if (dur == m_duration)
		return;

	m_duration = dur;
	m_durText = getTimeTxt(m_duration);
	if (m_pLabel != nullptr)
	{
		m_pLabel->setText(m_curText + "/" + m_durText);
	}
}


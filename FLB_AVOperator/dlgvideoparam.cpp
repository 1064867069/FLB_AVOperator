#include "dlgvideoparam.h"
#include "videoprocess.h"
#include "videoopenglplayer.h"

DlgVideoParam::DlgVideoParam(VideoProcessList* pProcs, QWidget* p) :
	QDialog(p), m_spVideoBrightProc(std::make_shared<VideoBrightAdjust>()),
	m_spVideoChromProc(std::make_shared<VideoChromAdjust>()),
	m_spVideoContrastProc(std::make_shared<VideoContrastAdjust>()),
	m_spVideoClrTempProc(std::make_shared<VideoClrTempAdjust>())
{
	ui.setupUi(this);

	pProcs->addVProcessor(m_spVideoBrightProc);
	pProcs->addVProcessor(m_spVideoChromProc);
	pProcs->addVProcessor(m_spVideoContrastProc);
	pProcs->addVProcessor(m_spVideoClrTempProc);

	this->sliderBind();
}

void DlgVideoParam::sliderBind()
{
	QList<QSlider*> sliders = { ui.m_pSliderBright, ui.m_pSliderChrom, ui.m_pSliderContrast, ui.m_pSliderClrTemp };
	QList<QLabel*> labels = { ui.m_labelCurBright, ui.m_labelCurChrom, ui.m_labelCurContrast, ui.m_labelCurClrTemp };

	for (int i = 0; i < sliders.size(); ++i)
	{
		m_hashSliderLabel[sliders[i]] = labels[i];
		connect(sliders[i], &QSlider::valueChanged, this, &DlgVideoParam::onSliderChanged);
		connect(sliders[i], &QSlider::sliderReleased, this, &DlgVideoParam::paramUpdated);
	}

	connect(ui.m_pSliderBright, &QSlider::valueChanged, m_spVideoBrightProc.get(), &VideoBrightAdjust::setBright);
	connect(ui.m_pSliderChrom, &QSlider::valueChanged, m_spVideoChromProc.get(), &VideoChromAdjust::setChrom);
	connect(ui.m_pSliderContrast, &QSlider::valueChanged,
		m_spVideoContrastProc.get(), &VideoContrastAdjust::setContrast);
	connect(ui.m_pSliderClrTemp, &QSlider::valueChanged, m_spVideoClrTempProc.get(), &VideoClrTempAdjust::setClrTemp);
}

void DlgVideoParam::onSliderChanged(int val)
{
	QSlider* pSlider = dynamic_cast<QSlider*>(sender());
	if (pSlider == nullptr)
		return;

	QLabel* pLabel = m_hashSliderLabel[pSlider];

	if (pLabel != nullptr)
		pLabel->setText(QString::number(val));

}


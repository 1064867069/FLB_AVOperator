#include "dlgvideoparam.h"
#include "videoprocess.h"
#include "videoopenglplayer.h"

DlgVideoParam::DlgVideoParam(VideoProcessList* pProcs, QWidget* p) :
	QDialog(p), m_spVideoBrightProc(std::make_shared<VideoBrightAdjust>()),
	m_spVideoChromProc(std::make_shared<VideoChromAdjust>()),
	m_spVideoContrastProc(std::make_shared<VideoContrastAdjust>())
{
	ui.setupUi(this);

	pProcs->addVProcessor(m_spVideoBrightProc);
	pProcs->addVProcessor(m_spVideoChromProc);
	pProcs->addVProcessor(m_spVideoContrastProc);

	connect(ui.m_pSliderBright, &QSlider::valueChanged, this, &DlgVideoParam::onBrightChanged);
	connect(ui.m_pSliderChrom, &QSlider::valueChanged, this, &DlgVideoParam::onChromChanged);
	connect(ui.m_pSliderContrast, &QSlider::valueChanged, this, &DlgVideoParam::onContrastChanged);
}

void DlgVideoParam::onBrightChanged(int val)
{
	if (val == 0)
		m_spVideoBrightProc->setBright(0.0f);
	else
		m_spVideoBrightProc->setBright(static_cast<float>(val) / ui.m_pSliderBright->maximum());


	ui.m_labelCurBright->setText(QString::number(val));
	emit paramUpdated();
}

void DlgVideoParam::onChromChanged(int val)
{
	if (val == 0)
		m_spVideoChromProc->setChrom(0.0f);
	else
		m_spVideoChromProc->setChrom(static_cast<float>(val) / ui.m_pSliderChrom->maximum());

	ui.m_labelCurChrom->setText(QString::number(val));
	emit paramUpdated();
}

void DlgVideoParam::onContrastChanged(int val)
{
	if (val == 0)
		m_spVideoContrastProc->setContrast(0.0f);
	else
		m_spVideoContrastProc->setContrast(static_cast<float>(val) / ui.m_pSliderChrom->maximum());

	ui.m_labelCurContrast->setText(QString::number(val));
	emit paramUpdated();
}

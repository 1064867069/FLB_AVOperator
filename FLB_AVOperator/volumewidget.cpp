#include "volumewidget.h"
#include "audioprocess.h"
#include "avoperator.h"
#include <QDebug>
#include <QPainter>

VolumeWidget::VolumeWidget(FAVPlayer* player, QWidget* p)
	: QWidget(p), m_spVolProc(std::make_shared<VolumnAdjustProcessor>(this))
{
	ui.setupUi(this);

	connect(ui.m_sliderVolume, &QSlider::valueChanged, this, &VolumeWidget::onVolSliderChanged);
	connect(ui.m_chkBoxSilence, &QCheckBox::stateChanged, this, &VolumeWidget::onSilenceChanged);
	connect(this, &VolumeWidget::volChanged, m_spVolProc.get(), &VolumnAdjustProcessor::onVolChanged);

	player->addProcessor(m_spVolProc);
	//this->setFixedHeight(200);
	//this->setAutoFillBackground(true);
	this->setStyleSheet("background-color: #050505;color: white;");

	qApp->installEventFilter(this);
}

void VolumeWidget::setRelatedWidget(QWidget* pw)
{
	m_pRelatedWidget = pw;
}

void VolumeWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setBrush(Qt::black); // 设置刷子的颜色与样式表中的颜色一致
	painter.setPen(Qt::NoPen); // 不绘制边框
	painter.drawRect(this->rect()); // 绘制整个部件的矩形
	QWidget::paintEvent(event); // 调用基类的paintEvent以确保其他绘制正常进行
}

void VolumeWidget::onSilenceChanged(int s)
{
	if (s == Qt::Checked)
		emit volChanged(0);
	else if (s == Qt::Unchecked)
		emit volChanged(ui.m_sliderVolume->value());
}

void VolumeWidget::onVolSliderChanged(int v)
{
	if (ui.m_chkBoxSilence->checkState() == Qt::Unchecked)
	{
		ui.m_volLabel->setText(QString::number(v));
		if (v == 0)
		{
			ui.m_chkBoxSilence->setCheckState(Qt::Checked);
		}
		else
		{
			emit volChanged(v);
		}
	}

}

bool VolumeWidget::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		auto mPos = this->mapFromGlobal(mouseEvent->globalPos());

		if (!this->rect().contains(mPos)) {
			// 当鼠标点击发生在窗口外部时隐藏鼠标指针	
			if (m_pRelatedWidget == nullptr
				|| !m_pRelatedWidget->rect().contains(m_pRelatedWidget->mapFromGlobal(mouseEvent->globalPos())))
				this->hide();
			//return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include "ui_volumewidget.h"

class VolumnAdjustProcessor;
class FAVPlayer;

class VolumeWidget :public QWidget
{
	Q_OBJECT
public:
	explicit VolumeWidget(FAVPlayer*, QWidget* p);

	void setRelatedWidget(QWidget*);
protected slots:
	void onSilenceChanged(int);

	void onVolSliderChanged(int);

	bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;

	void paintEvent(QPaintEvent* event)Q_DECL_OVERRIDE;

signals:
	void volChanged(int);

private:
	std::shared_ptr<VolumnAdjustProcessor> m_spVolProc;

	QWidget* m_pRelatedWidget = nullptr;
private:
	Ui::Form ui;
};


#endif // !VOLUMEWIDGET_H


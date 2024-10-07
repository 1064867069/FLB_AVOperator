#ifndef STREAMTIMEWIDGET_H
#define STREAMTIMEWIDGET_H

#include "ui_streamtimewidget.h"

class AVStreamBlock;
class StreamTimeWidget :public QWidget
{
	Q_OBJECT
public:
	explicit StreamTimeWidget(QWidget* p = nullptr);

public slots:
	void showOpStreamBlock(AVStreamBlock* pObj);

protected slots:
	virtual void hideEvent(QHideEvent* event) Q_DECL_OVERRIDE;

	virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

	//virtual void focusOutEvent(QFocusEvent* event) Q_DECL_OVERRIDE;
private:
	void initWidget();

	void resetTime();
private slots:
	void onFocusChanged(QWidget* old, QWidget* newWidget);

	void onTimeLineChanged();

	void onStartLeft();

	void onStartRight();

	void onEndLeft();

	void onEndRight();

	void onWholeLeft();

	void onWholeRight();
private:
	AVStreamBlock* m_pBlockObject = nullptr;
	double m_startGap = 0.01;
	double m_endGap = 0.01;
	double m_wholeGap = 0.01;

	Ui::StreamTimeWidget ui;
};


#endif // !STREAMTIMEWIDGET_H


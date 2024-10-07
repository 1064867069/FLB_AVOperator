#include "streamtimewidget.h"
#include "streamwidgets.h"
#include "flb_avoperator.h"

#include <QEvent>
#include <QShowEvent>
#include <QDebug>

StreamTimeWidget::StreamTimeWidget(QWidget* p) :QWidget(p)
{
	ui.setupUi(this);

	this->initWidget();
}

void StreamTimeWidget::initWidget()
{
	// 正则表达式：匹配大于0小于10的数字，最多两位小数
	QRegularExpression regex("^(0(\\.\\d{1,2})?|[1-9](\\.\\d{1,2})?|10(\\.0{1,2})?)$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	auto lineEdits = { ui.m_pLineEditMove, ui.m_pLineEditMoveStart, ui.m_pLineEditMoveEnd };
	for (auto pLineEdit : lineEdits)
		pLineEdit->setValidator(validator);
	m_startGap = ui.m_pLineEditMoveStart->text().toDouble();
	m_endGap = ui.m_pLineEditMoveEnd->text().toDouble();
	m_wholeGap = ui.m_pLineEditMove->text().toDouble();

	auto app = qobject_cast<QApplication*>(QApplication::instance());
	if (app)
		connect(app, &QApplication::focusChanged, this, &StreamTimeWidget::onFocusChanged);

	connect(ui.m_pLineEditMove, &QLineEdit::textChanged, this, &StreamTimeWidget::onTimeLineChanged);
	connect(ui.m_pLineEditMoveStart, &QLineEdit::textChanged, this, &StreamTimeWidget::onTimeLineChanged);
	connect(ui.m_pLineEditMoveEnd, &QLineEdit::textChanged, this, &StreamTimeWidget::onTimeLineChanged);

	connect(ui.m_pBtnMvLeftStart, &QPushButton::clicked, this, &StreamTimeWidget::onStartLeft);
	connect(ui.m_pBtnMvLeftEnd, &QPushButton::clicked, this, &StreamTimeWidget::onEndLeft);
	connect(ui.m_pBtnMvRightStart, &QPushButton::clicked, this, &StreamTimeWidget::onStartRight);
	connect(ui.m_pBtnMvRightEnd, &QPushButton::clicked, this, &StreamTimeWidget::onEndRight);
	connect(ui.m_pBtnMvLeft, &QPushButton::clicked, this, &StreamTimeWidget::onWholeLeft);
	connect(ui.m_pBtnMvRight, &QPushButton::clicked, this, &StreamTimeWidget::onWholeRight);

	this->setFixedWidth(this->width());
	this->setFixedHeight(this->height());
	this->setWindowFlags(Qt::Window);
}

void StreamTimeWidget::showOpStreamBlock(AVStreamBlock* pObj)
{
	m_pBlockObject = pObj;
	if (m_pBlockObject != nullptr)
	{
		auto pMainWindow = FLB_AVOperator::getWindowPtr();
		QPoint objPt;
		if (pMainWindow)
		{
			objPt = pMainWindow->mapToGlobal({ (pMainWindow->width() >> 1) - (this->width() >> 1) ,
				(pMainWindow->height() >> 1) - (this->height() >> 1) });
		}

		this->resetTime();
		this->move(objPt);
		this->show();
	}
}

void StreamTimeWidget::resetTime()
{
	double start = 0, end = 0;
	if (m_pBlockObject)
	{
		auto spStream = m_pBlockObject->getStreamPtr();
		start = spStream->timeStart();
		end = spStream->timeEnd();
	}

	ui.m_pLabelTimeStart->setText(QString("%1 秒").arg(start, 0, 'f', 2));
	ui.m_pLabelTimeEnd->setText(QString("%1 秒").arg(end, 0, 'f', 2));
}

void StreamTimeWidget::onFocusChanged(QWidget* old, QWidget* newWidget)
{
	if (!this->isHidden() && newWidget)
	{
		if (!this->isAncestorOf(newWidget))
			this->hide();
	}
}

void StreamTimeWidget::onTimeLineChanged()
{
	auto senderLineEdit = dynamic_cast<QLineEdit*>(sender());
	if (senderLineEdit == ui.m_pLineEditMoveStart)
		m_startGap = senderLineEdit->text().toDouble();
	else if (senderLineEdit == ui.m_pLineEditMoveEnd)
		m_endGap = senderLineEdit->text().toDouble();
	else if (senderLineEdit == ui.m_pLineEditMove)
		m_wholeGap = senderLineEdit->text().toDouble();
}

void StreamTimeWidget::onStartLeft()
{
	if (m_startGap > 1e-4)
		m_pBlockObject->secStartMoveRight(-m_startGap);
}

void StreamTimeWidget::onStartRight()
{
	if (m_startGap > 1e-4)
		m_pBlockObject->secStartMoveRight(m_startGap);
}

void StreamTimeWidget::onEndLeft()
{
	if (m_endGap > 1e-4)
		m_pBlockObject->secEndMoveRight(-m_endGap);
}

void StreamTimeWidget::onEndRight()
{
	if (m_endGap > 1e-4)
		m_pBlockObject->secEndMoveRight(m_endGap);
}

void StreamTimeWidget::onWholeLeft()
{
	if (m_wholeGap > 1e-4)
		m_pBlockObject->secMoveRight(-m_wholeGap);
}

void StreamTimeWidget::onWholeRight()
{
	if (m_wholeGap > 1e-4)
		m_pBlockObject->secMoveRight(m_wholeGap);
}

//void StreamTimeWidget::focusOutEvent(QFocusEvent* event)
//{
//	QWidget::focusOutEvent(event);
//	if (!this->isAncestorOf(QApplication::activeWindow()))
//	{
//		this->hide();
//	}
//}

void StreamTimeWidget::hideEvent(QHideEvent* event)
{
	QWidget::hideEvent(event);
	m_pBlockObject = nullptr;
}

void StreamTimeWidget::showEvent(QShowEvent* event)
{
	if (m_pBlockObject != nullptr)
	{
		QWidget::showEvent(event);
	}
	else
	{
		this->hide();
		event->ignore();
	}
}

#include "uicomponents.h"

#include <QDebug>
#include <QResizeEvent>
#include <QMouseEvent>

SquareButton::SquareButton(QWidget* p) :QPushButton(p)
{
	this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
	this->setAttribute(Qt::WA_Hover, true);
	this->raise();
	this->setMinimumSize(40, 40);
	this->setStyleSheet("QPushButton:hover { background-color: lightblue; }");
}

void SquareButton::resizeEvent(QResizeEvent* event)
{
	QPushButton::resizeEvent(event);
	auto size = event->size();
	if (size.width() != size.height())
		this->setFixedWidth(size.height());

	this->setIconSize(size);

	qDebug() << size;
}

//void SquareButton::mouseMoveEvent(QMouseEvent* event)
//{
//	QPushButton::mouseMoveEvent(event);
//}

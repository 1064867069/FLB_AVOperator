#include "uicomponents.h"

#include <QDebug>
#include <QResizeEvent>
#include <QMouseEvent>

SquareButton::SquareButton(QWidget* p) :QPushButton(p)
{
	this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
	this->setAttribute(Qt::WA_Hover, true);
	this->raise();
	this->setMinimumSize(20, 20);
	this->setStyleSheet("QPushButton { background-color: black; color: white; } "
		"QPushButton:hover { background-color: lightblue; }");
}

void SquareButton::resizeEvent(QResizeEvent* event)
{
	QPushButton::resizeEvent(event);
	auto size = event->size();
	if (size.width() != size.height())
		this->setFixedWidth(size.height());

	auto bl = std::min(size.height(), size.width());
	this->setIconSize(QSize(bl, bl));

	qDebug() << size;
}

//void SquareButton::mouseMoveEvent(QMouseEvent* event)
//{
//	QPushButton::mouseMoveEvent(event);
//}

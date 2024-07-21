#ifndef UICOMPONENTS_H
#define UICOMPONENTS_H

#include <QPushButton>

class SquareButton :public QPushButton
{
	Q_OBJECT
public:
	explicit SquareButton(QWidget* p = nullptr);

protected:
	void resizeEvent(QResizeEvent* event)Q_DECL_OVERRIDE;
	// void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
};


#endif // !UICOMPONENTS_H


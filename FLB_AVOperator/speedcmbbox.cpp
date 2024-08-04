#include "speedcmbbox.h"
#include "avoperator.h"

SpeedCombobox::SpeedCombobox(FAVPlayer* pPlayer, QWidget* p) : QComboBox(p), m_pPlayer(pPlayer)
{
	this->addItem("4.0");
	this->addItem("2.0");
	this->addItem("1.5");
	this->addItem("1.25");
	this->addItem("1.0");
	this->addItem("0.75");
	this->addItem("0.5");

	//this->setToolTip("倍速");
	this->setCurrentIndex(this->count() - 3);
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));

	this->setMinimumHeight(30);
	this->setMaximumWidth(50);
}

bool SpeedCombobox::isPopupShown()const
{
	return m_popupShown;
}

void SpeedCombobox::hidePopup()
{
	QComboBox::hidePopup();
	m_popupShown = false;
}

void SpeedCombobox::showPopup()
{
	QComboBox::showPopup();
	m_popupShown = true;
}

void SpeedCombobox::onCurrentIndexChanged(int i)
{
	if (i < 0 || i >= this->count() || m_pPlayer == nullptr)
		return;

	m_pPlayer->setSpeed(this->itemText(i).toFloat());
}

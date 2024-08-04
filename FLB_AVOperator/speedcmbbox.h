#ifndef SPEEDCMBBOX_H
#define SPEEDCMBBOX_H

#include <QComboBox>

class FAVPlayer;
class SpeedCombobox :public QComboBox
{
	Q_OBJECT
public:
	explicit SpeedCombobox(FAVPlayer* pPlayer, QWidget* p = nullptr);

	bool isPopupShown()const;
protected slots:
	void onCurrentIndexChanged(int);

	virtual void showPopup()Q_DECL_OVERRIDE;
	virtual void hidePopup()Q_DECL_OVERRIDE;
private:
	FAVPlayer* m_pPlayer = nullptr;

	bool m_popupShown = false;
};


#endif

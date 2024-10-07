#ifndef TABLESTREAMS_H
#define TABLESTREAMS_H

#include "interfaces.h"

#include <QTableWidget>

class StreamManager;
class AVStreamItemWidget;
class StreamSliderController;
class StreamTimeSlider;
class AVStreamBlock;
class StreamTimeWidget;

class TableStreams :public QTableWidget
{
	Q_OBJECT
public:
	explicit TableStreams(FAVPlayer* pPlayer, QWidget*);
	~TableStreams();

	bool addStreams(const QList<StreamPtr>& streams);//记得创建对应表项窗口

	std::shared_ptr<StreamManager> getStreamManager()const;
signals:
	void durationSecondChanged(double);

protected slots:
	virtual void resizeEvent(QResizeEvent* event) override;

	void onBackEndChanged(double);
	void onStreamFirstHas();
	void onBlockTimeSet(AVStreamBlock* pBlock);
private:
	std::shared_ptr<StreamManager> m_spStreamManager;
	AVStreamItemWidget* m_pWidgetMaxLen = nullptr;

	StreamTimeWidget* m_pStreamBlockTimeWidget = nullptr;

	FAVPlayer* m_pPlayer;
	StreamTimeSlider* m_pSlider;
	StreamSliderController* m_pSliderController;

	int m_pad = 5;
	bool m_bManagerBind = false;
};


#endif // !TABLESTREAMS_H


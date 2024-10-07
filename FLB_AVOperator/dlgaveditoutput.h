#ifndef DLGAVEDITOUTPUT_H
#define DLGAVEDITOUTPUT_H

#include "ui_dlgaveditoutput.h"

class StreamManager;
class AVFileWriter;
class DlgAVEditOutput : public QDialog
{
	Q_OBJECT
public:
	DlgAVEditOutput(const std::shared_ptr<StreamManager>&, QWidget* p = nullptr);

protected slots:
	virtual void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

	virtual void reject()Q_DECL_OVERRIDE;
private:
	void initWidget();
private slots:
	void onStopClicked();

	void onPauseClicked();

	void onStartClicked();

	void onOutputSecondChanged(double);

	void onOutputFinished();
	//void onProgress(int val);
private:
	std::shared_ptr<StreamManager> m_spManager;
	AVFileWriter* m_pFileWriter;
	double m_duration = 0;

	Ui::DlgAVEditOutput ui;
};

#endif // !DLGAVEDITOUTPUT_H


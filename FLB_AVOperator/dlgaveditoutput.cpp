#include "dlgaveditoutput.h"
#include "avfilewriter.h"
#include "avreader.h"
#include "streamutils.h"

#include <QFileDialog>
#include <QMessageBox>

DlgAVEditOutput::DlgAVEditOutput(const std::shared_ptr<StreamManager>& spManager, QWidget* p) :
	QDialog(p), m_spManager(spManager)
{
	ui.setupUi(this);

	auto spReader = std::make_shared<FAVStreamReader>(spManager);
	m_pFileWriter = new AVFileWriter(spReader, this);
	connect(m_pFileWriter, &AVFileWriter::seek, spReader.get(), &FAVStreamReader::seekSecond, Qt::DirectConnection);

	m_duration = m_spManager->getSecEnd();
	this->initWidget();
}

void DlgAVEditOutput::initWidget()
{
	this->setFixedSize(this->size());
	connect(ui.m_pBtnStart, &QPushButton::clicked, this, &DlgAVEditOutput::onStartClicked);
	connect(ui.m_pBtnPause, &QPushButton::clicked, this, &DlgAVEditOutput::onPauseClicked);
	connect(ui.m_pBtnStop, &QPushButton::clicked, this, &DlgAVEditOutput::onStopClicked);

	ui.m_pBtnStop->setEnabled(false);
	ui.m_pBtnPause->setEnabled(false);
	connect(m_pFileWriter, &AVFileWriter::curSecChanged, this, &DlgAVEditOutput::onOutputSecondChanged,
		Qt::QueuedConnection);
	connect(m_pFileWriter, &AVFileWriter::finished, this, &DlgAVEditOutput::onOutputFinished);
}

void DlgAVEditOutput::closeEvent(QCloseEvent* event)
{
	m_pFileWriter->stop();
	QDialog::closeEvent(event);
}

void DlgAVEditOutput::reject()
{
	m_pFileWriter->stop();
	QDialog::reject();
}

void DlgAVEditOutput::onStopClicked()
{
	if (m_pFileWriter->getState() != WriterState::Stop)
	{
		m_pFileWriter->stop();
	}

	ui.m_pBtnPause->setEnabled(false);
	ui.m_pBtnStop->setEnabled(false);
	ui.m_pBtnStart->setEnabled(true);
}

void DlgAVEditOutput::onStartClicked()
{
	if (m_pFileWriter->getState() == WriterState::Stop)
	{
		QString filePath = QFileDialog::getSaveFileName(this, "新建文件", "",
			"mp3文件(*.mp3);;mp4文件 (*.mp4);;");
		if (filePath == "")
			return;

		if (!m_pFileWriter->writeFile(filePath, ui.m_pCmbSize->currentText()))
		{
			QMessageBox::critical(this, "音视频输出", "音视频打开失败");
			return;
		}
		ui.m_progressBarEncode->setValue(0);
	}
	else if (m_pFileWriter->getState() == WriterState::Working)
	{
		m_pFileWriter->pause(true);
	}

	ui.m_pBtnPause->setEnabled(true);
	ui.m_pBtnStop->setEnabled(true);
	ui.m_pBtnStart->setEnabled(false);
}

void DlgAVEditOutput::onOutputSecondChanged(double sec)
{
	if (m_duration > 0)
	{
		int val = 100 * sec / m_duration;
		ui.m_progressBarEncode->setValue(val);
	}
	else
	{
		m_duration = m_spManager->getSecEnd();
	}
}

void DlgAVEditOutput::onOutputFinished()
{
	QMessageBox::information(this, "文件导出", "文件导出完毕");
	m_pFileWriter->stop();
	ui.m_pBtnPause->setText("暂停");
	ui.m_pBtnPause->setEnabled(false);
	ui.m_pBtnStop->setEnabled(false);
	ui.m_pBtnStart->setEnabled(true);
}

void DlgAVEditOutput::onPauseClicked()
{
	if (m_pFileWriter->getState() == WriterState::Paused)
	{
		m_pFileWriter->pause(false);
		ui.m_pBtnPause->setText("暂停");
	}
	else if (m_pFileWriter->getState() == WriterState::Working)
	{
		m_pFileWriter->pause(true);
		ui.m_pBtnPause->setText("开始");
	}
}

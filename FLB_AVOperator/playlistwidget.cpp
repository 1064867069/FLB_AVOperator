#include "playlistwidget.h"
#include "playtable.h"
#include "avfilemanager.h"

#include <QVBoxLayout>

PlayListWidget::PlayListWidget(IAVPathManager* pm, QWidget* p) :QWidget(p)
{
	m_pPlayTable = new PathPlayTable(pm, this);
	m_pButtonAddFile = new QPushButton(this);

	auto* fm = &AVFileManager::getInstance();

	m_pButtonAddFile->setFixedHeight(30);
	m_pButtonAddFile->setText("添加文件");
	pm->setDlgParent(this);
	connect(m_pButtonAddFile, &QPushButton::clicked, pm, &IAVPathManager::importPath);

	auto vlayout = new QVBoxLayout(this);
	vlayout->addWidget(m_pButtonAddFile);
	vlayout->addWidget(m_pPlayTable);

	vlayout->setSpacing(0);
	vlayout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(vlayout);

	QString styleSheet = R"(
        
        QWidget {
            background-color: #1D1D1D;
            color: white;
        }
        QPushButton {
            background-color: black;
            color: white;
            border: 1px solid white;
        }
        QPushButton:hover {
            background-color: lightblue;
        }
        QTableWidget::item { border: none; }
        QTableWidget::item:selected { border: none; background: lightblue; }
    )";

	this->setStyleSheet(styleSheet);
}

const PathPlayTable* PlayListWidget::getPlayTable()const
{
	return m_pPlayTable;
}


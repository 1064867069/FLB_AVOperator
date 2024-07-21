/********************************************************************************
** Form generated from reading UI file 'playbtmbar.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBTMBAR_H
#define UI_PLAYBTMBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlayBtmBar
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSlider *m_pPlaySlider;
    QHBoxLayout *horizontalLayout;

    void setupUi(QWidget *PlayBtmBar)
    {
        if (PlayBtmBar->objectName().isEmpty())
            PlayBtmBar->setObjectName(QStringLiteral("PlayBtmBar"));
        PlayBtmBar->resize(705, 100);
        PlayBtmBar->setMaximumSize(QSize(16777215, 100));
        PlayBtmBar->setStyleSheet(QStringLiteral(""));
        verticalLayoutWidget = new QWidget(PlayBtmBar);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 681, 80));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_pPlaySlider = new QSlider(verticalLayoutWidget);
        m_pPlaySlider->setObjectName(QStringLiteral("m_pPlaySlider"));
        m_pPlaySlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(m_pPlaySlider);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(PlayBtmBar);

        QMetaObject::connectSlotsByName(PlayBtmBar);
    } // setupUi

    void retranslateUi(QWidget *PlayBtmBar)
    {
        PlayBtmBar->setWindowTitle(QApplication::translate("PlayBtmBar", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PlayBtmBar: public Ui_PlayBtmBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBTMBAR_H

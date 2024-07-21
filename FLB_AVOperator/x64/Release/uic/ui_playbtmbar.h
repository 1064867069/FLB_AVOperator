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
  horizontalLayout->addWidget(m_pBtnLast);

        m_pBtnRewind = new QPushButton(verticalLayoutWidget);
        m_pBtnRewind->setObjectName(QStringLiteral("m_pBtnRewind"));
        m_pBtnRewind->setMinimumSize(QSize(25, 25));

        horizontalLayout->addWidget(m_pBtnRewind);

        m_pBtnPlayPause = new QPushButton(verticalLayoutWidget);
        m_pBtnPlayPause->setObjectName(QStringLiteral("m_pBtnPlayPause"));
        m_pBtnPlayPause->setMinimumSize(QSize(25, 25));

        horizontalLayout->addWidget(m_pBtnPlayPause);

        m_pBtnFastForward = new QPushButton(verticalLayoutWidget);
        m_pBtnFastForward->setObjectName(QStringLiteral("m_pBtnFastForward"));
        m_pBtnFastForward->setMinimumSize(QSize(25, 25));

        horizontalLayout->addWidget(m_pBtnFastForward);

        m_pBtnNext = new QPushButton(verticalLayoutWidget);
        m_pBtnNext->setObjectName(QStringLiteral("m_pBtnNext"));
        m_pBtnNext->setMinimumSize(QSize(25, 25));

        horizontalLayout->addWidget(m_pBtnNext);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_pBtnVolume = new QPushButton(verticalLayoutWidget);
        m_pBtnVolume->setObjectName(QStringLiteral("m_pBtnVolume"));
        m_pBtnVolume->setMinimumSize(QSize(25, 25));

        horizontalLayout->addWidget(m_pBtnVolume);

        m_pBtnFullScreen = new QPushButton(verticalLayoutWidget);
        m_pBtnFullScreen->setObjectName(QStringLiteral("m_pBtnFullScreen"));
        m_pBtnFullScreen->setMinimumSize(QSize(25, 25));

        horizontalLayout->addWidget(m_pBtnFullScreen);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(PlayBtmBar);

        QMetaObject::connectSlotsByName(PlayBtmBar);
    } // setupUi

    void retranslateUi(QWidget *PlayBtmBar)
    {
        PlayBtmBar->setWindowTitle(QApplication::translate("PlayBtmBar", "Form", Q_NULLPTR));
        m_pBtnLast->setText(QString());
        m_pBtnRewind->setText(QString());
        m_pBtnPlayPause->setText(QString());
        m_pBtnFastForward->setText(QString());
        m_pBtnNext->setText(QString());
        m_pBtnVolume->setText(QString());
        m_pBtnFullScreen->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlayBtmBar: public Ui_PlayBtmBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBTMBAR_H

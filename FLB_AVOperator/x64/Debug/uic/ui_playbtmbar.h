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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlayBtmBar
{
public:
    QSlider *horizontalSlider;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *PlayBtmBar)
    {
        if (PlayBtmBar->objectName().isEmpty())
            PlayBtmBar->setObjectName(QStringLiteral("PlayBtmBar"));
        PlayBtmBar->resize(699, 51);
        horizontalSlider = new QSlider(PlayBtmBar);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(110, 13, 271, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        pushButton = new QPushButton(PlayBtmBar);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 31, 31));
        pushButton_2 = new QPushButton(PlayBtmBar);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 10, 31, 31));
        pushButton_3 = new QPushButton(PlayBtmBar);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(70, 10, 31, 31));

        retranslateUi(PlayBtmBar);

        QMetaObject::connectSlotsByName(PlayBtmBar);
    } // setupUi

    void retranslateUi(QWidget *PlayBtmBar)
    {
        PlayBtmBar->setWindowTitle(QApplication::translate("PlayBtmBar", "Form", Q_NULLPTR));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlayBtmBar: public Ui_PlayBtmBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBTMBAR_H

/********************************************************************************
** Form generated from reading UI file 'flb_avoperator.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLB_AVOPERATOR_H
#define UI_FLB_AVOPERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FLB_AVOperatorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FLB_AVOperatorClass)
    {
        if (FLB_AVOperatorClass->objectName().isEmpty())
            FLB_AVOperatorClass->setObjectName(QStringLiteral("FLB_AVOperatorClass"));
        FLB_AVOperatorClass->resize(600, 400);
        menuBar = new QMenuBar(FLB_AVOperatorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FLB_AVOperatorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FLB_AVOperatorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FLB_AVOperatorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FLB_AVOperatorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FLB_AVOperatorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FLB_AVOperatorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FLB_AVOperatorClass->setStatusBar(statusBar);

        retranslateUi(FLB_AVOperatorClass);

        QMetaObject::connectSlotsByName(FLB_AVOperatorClass);
    } // setupUi

    void retranslateUi(QMainWindow *FLB_AVOperatorClass)
    {
        FLB_AVOperatorClass->setWindowTitle(QApplication::translate("FLB_AVOperatorClass", "FLB_AVOperator", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FLB_AVOperatorClass: public Ui_FLB_AVOperatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLB_AVOPERATOR_H

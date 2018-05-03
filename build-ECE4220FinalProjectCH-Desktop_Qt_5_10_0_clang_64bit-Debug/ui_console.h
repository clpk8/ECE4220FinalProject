/********************************************************************************
** Form generated from reading UI file 'console.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Console
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLineEdit *PortNumber;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Console)
    {
        if (Console->objectName().isEmpty())
            Console->setObjectName(QStringLiteral("Console"));
        Console->resize(400, 300);
        centralWidget = new QWidget(Console);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 30, 331, 35));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        PortNumber = new QLineEdit(widget);
        PortNumber->setObjectName(QStringLiteral("PortNumber"));

        horizontalLayout->addWidget(PortNumber);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        horizontalLayout_2->addLayout(horizontalLayout);

        Console->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Console);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        Console->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Console);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Console->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Console);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Console->setStatusBar(statusBar);

        retranslateUi(Console);

        QMetaObject::connectSlotsByName(Console);
    } // setupUi

    void retranslateUi(QMainWindow *Console)
    {
        Console->setWindowTitle(QApplication::translate("Console", "Console", nullptr));
        label->setText(QApplication::translate("Console", "Port number:", nullptr));
        pushButton->setText(QApplication::translate("Console", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Console: public Ui_Console {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLE_H

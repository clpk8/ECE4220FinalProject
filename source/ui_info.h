/********************************************************************************
** Form generated from reading UI file 'info.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFO_H
#define UI_INFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Info
{
public:
    QTableView *tableView;
    QLabel *label_status2;
    QPushButton *exit;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_LOAD_TABLE;
    QPushButton *pushButton_sorttime;
    QPushButton *pushButton_sortID;

    void setupUi(QDialog *Info)
    {
        if (Info->objectName().isEmpty())
            Info->setObjectName(QStringLiteral("Info"));
        Info->resize(970, 494);
        tableView = new QTableView(Info);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(10, 20, 951, 401));
        label_status2 = new QLabel(Info);
        label_status2->setObjectName(QStringLiteral("label_status2"));
        label_status2->setGeometry(QRect(20, 440, 231, 17));
        exit = new QPushButton(Info);
        exit->setObjectName(QStringLiteral("exit"));
        exit->setGeometry(QRect(790, 440, 89, 25));
        layoutWidget = new QWidget(Info);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(320, 440, 254, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_LOAD_TABLE = new QPushButton(layoutWidget);
        pushButton_LOAD_TABLE->setObjectName(QStringLiteral("pushButton_LOAD_TABLE"));

        horizontalLayout->addWidget(pushButton_LOAD_TABLE);

        pushButton_sorttime = new QPushButton(layoutWidget);
        pushButton_sorttime->setObjectName(QStringLiteral("pushButton_sorttime"));

        horizontalLayout->addWidget(pushButton_sorttime);

        pushButton_sortID = new QPushButton(layoutWidget);
        pushButton_sortID->setObjectName(QStringLiteral("pushButton_sortID"));

        horizontalLayout->addWidget(pushButton_sortID);


        retranslateUi(Info);

        QMetaObject::connectSlotsByName(Info);
    } // setupUi

    void retranslateUi(QDialog *Info)
    {
        Info->setWindowTitle(QApplication::translate("Info", "Dialog", nullptr));
        label_status2->setText(QApplication::translate("Info", "+status", nullptr));
        exit->setText(QApplication::translate("Info", "EXIT", nullptr));
        pushButton_LOAD_TABLE->setText(QApplication::translate("Info", "Load Data", nullptr));
        pushButton_sorttime->setText(QApplication::translate("Info", "Sort Time", nullptr));
        pushButton_sortID->setText(QApplication::translate("Info", "Sort ID", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Info: public Ui_Info {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFO_H

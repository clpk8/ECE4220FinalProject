/********************************************************************************
** Form generated from reading UI file 'sending.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDING_H
#define UI_SENDING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sending
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;

    void setupUi(QWidget *Sending)
    {
        if (Sending->objectName().isEmpty())
            Sending->setObjectName(QStringLiteral("Sending"));
        Sending->resize(400, 300);
        label = new QLabel(Sending);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 30, 60, 16));
        label_2 = new QLabel(Sending);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(230, 30, 60, 16));
        pushButton = new QPushButton(Sending);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(280, 190, 113, 32));

        retranslateUi(Sending);

        QMetaObject::connectSlotsByName(Sending);
    } // setupUi

    void retranslateUi(QWidget *Sending)
    {
        Sending->setWindowTitle(QApplication::translate("Sending", "Form", nullptr));
        label->setText(QApplication::translate("Sending", "TextLabel", nullptr));
        label_2->setText(QApplication::translate("Sending", "TextLabel", nullptr));
        pushButton->setText(QApplication::translate("Sending", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Sending: public Ui_Sending {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDING_H

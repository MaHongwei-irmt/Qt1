/********************************************************************************
** Form generated from reading UI file 'dialog_sensoraddr.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_SENSORADDR_H
#define UI_DIALOG_SENSORADDR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_SensorAddr
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_fmName;
    QTableView *tableView;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_readRegister;
    QPushButton *pushButton_writeRegister;
    QPushButton *pushButton_freshTable;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Dialog_SensorAddr)
    {
        if (Dialog_SensorAddr->objectName().isEmpty())
            Dialog_SensorAddr->setObjectName(QStringLiteral("Dialog_SensorAddr"));
        Dialog_SensorAddr->resize(830, 747);
        gridLayout_2 = new QGridLayout(Dialog_SensorAddr);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widget = new QWidget(Dialog_SensorAddr);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_fmName = new QLabel(widget);
        label_fmName->setObjectName(QStringLiteral("label_fmName"));

        gridLayout->addWidget(label_fmName, 0, 0, 1, 1);

        tableView = new QTableView(widget);
        tableView->setObjectName(QStringLiteral("tableView"));

        gridLayout->addWidget(tableView, 1, 0, 1, 1);


        horizontalLayout->addWidget(widget);

        widget_2 = new QWidget(Dialog_SensorAddr);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 157, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton_readRegister = new QPushButton(widget_2);
        pushButton_readRegister->setObjectName(QStringLiteral("pushButton_readRegister"));

        verticalLayout->addWidget(pushButton_readRegister);

        pushButton_writeRegister = new QPushButton(widget_2);
        pushButton_writeRegister->setObjectName(QStringLiteral("pushButton_writeRegister"));

        verticalLayout->addWidget(pushButton_writeRegister);

        pushButton_freshTable = new QPushButton(widget_2);
        pushButton_freshTable->setObjectName(QStringLiteral("pushButton_freshTable"));

        verticalLayout->addWidget(pushButton_freshTable);

        verticalSpacer_3 = new QSpacerItem(20, 156, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        verticalSpacer = new QSpacerItem(20, 288, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_2);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(Dialog_SensorAddr);

        QMetaObject::connectSlotsByName(Dialog_SensorAddr);
    } // setupUi

    void retranslateUi(QDialog *Dialog_SensorAddr)
    {
        Dialog_SensorAddr->setWindowTitle(QApplication::translate("Dialog_SensorAddr", "\344\274\240\346\204\237\345\231\250\345\257\204\345\255\230\345\231\250\350\257\273\345\206\231", Q_NULLPTR));
        label_fmName->setText(QApplication::translate("Dialog_SensorAddr", "TextLabel", Q_NULLPTR));
        pushButton_readRegister->setText(QApplication::translate("Dialog_SensorAddr", "\350\257\273\345\217\226", Q_NULLPTR));
        pushButton_writeRegister->setText(QApplication::translate("Dialog_SensorAddr", "\345\206\231\345\205\245", Q_NULLPTR));
        pushButton_freshTable->setText(QApplication::translate("Dialog_SensorAddr", "\346\230\276\347\244\272\345\210\267\346\226\260", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_SensorAddr: public Ui_Dialog_SensorAddr {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_SENSORADDR_H

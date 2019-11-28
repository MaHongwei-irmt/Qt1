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
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Dialog_SensorAddr
{
public:
    QDialogButtonBox *buttonBox;
    QTableView *tableView;
    QPushButton *pushButton;

    void setupUi(QDialog *Dialog_SensorAddr)
    {
        if (Dialog_SensorAddr->objectName().isEmpty())
            Dialog_SensorAddr->setObjectName(QStringLiteral("Dialog_SensorAddr"));
        Dialog_SensorAddr->resize(756, 668);
        buttonBox = new QDialogButtonBox(Dialog_SensorAddr);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(390, 620, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tableView = new QTableView(Dialog_SensorAddr);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(60, 41, 561, 551));
        pushButton = new QPushButton(Dialog_SensorAddr);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(650, 120, 75, 23));

        retranslateUi(Dialog_SensorAddr);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog_SensorAddr, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog_SensorAddr, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog_SensorAddr);
    } // setupUi

    void retranslateUi(QDialog *Dialog_SensorAddr)
    {
        Dialog_SensorAddr->setWindowTitle(QApplication::translate("Dialog_SensorAddr", "\344\274\240\346\204\237\345\231\250\345\257\204\345\255\230\345\231\250\350\257\273\345\206\231", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Dialog_SensorAddr", "\350\257\273\345\217\226", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_SensorAddr: public Ui_Dialog_SensorAddr {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_SENSORADDR_H

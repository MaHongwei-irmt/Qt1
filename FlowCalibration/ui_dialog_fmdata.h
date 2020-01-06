/********************************************************************************
** Form generated from reading UI file 'dialog_fmdata.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_FMDATA_H
#define UI_DIALOG_FMDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_fmdata
{
public:
    QLabel *label;
    QLabel *label_fmName;
    QToolButton *tbnWrite;
    QToolButton *tbnRead;
    QToolButton *tbnRead_SET_KF2;
    QLabel *label_2;
    QToolButton *tbnWrite_SET_KF2;
    QToolButton *tbnRead_FLOW_RANHE;
    QToolButton *tbnWrite_FLOW_RANHE;
    QLabel *label_3;
    QToolButton *tbnWrite_ZERO_CAL;
    QToolButton *tbnRead_ZERO_CAL;
    QLabel *label_4;
    QToolButton *tbnFMRegister;

    void setupUi(QDialog *Dialog_fmdata)
    {
        if (Dialog_fmdata->objectName().isEmpty())
            Dialog_fmdata->setObjectName(QStringLiteral("Dialog_fmdata"));
        Dialog_fmdata->resize(1292, 590);
        label = new QLabel(Dialog_fmdata);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 110, 54, 12));
        label_fmName = new QLabel(Dialog_fmdata);
        label_fmName->setObjectName(QStringLiteral("label_fmName"));
        label_fmName->setGeometry(QRect(50, 60, 271, 16));
        tbnWrite = new QToolButton(Dialog_fmdata);
        tbnWrite->setObjectName(QStringLiteral("tbnWrite"));
        tbnWrite->setGeometry(QRect(1110, 170, 81, 21));
        tbnRead = new QToolButton(Dialog_fmdata);
        tbnRead->setObjectName(QStringLiteral("tbnRead"));
        tbnRead->setGeometry(QRect(1020, 170, 91, 21));
        tbnRead_SET_KF2 = new QToolButton(Dialog_fmdata);
        tbnRead_SET_KF2->setObjectName(QStringLiteral("tbnRead_SET_KF2"));
        tbnRead_SET_KF2->setGeometry(QRect(1020, 260, 91, 21));
        label_2 = new QLabel(Dialog_fmdata);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 200, 54, 12));
        tbnWrite_SET_KF2 = new QToolButton(Dialog_fmdata);
        tbnWrite_SET_KF2->setObjectName(QStringLiteral("tbnWrite_SET_KF2"));
        tbnWrite_SET_KF2->setGeometry(QRect(1110, 260, 81, 21));
        tbnRead_FLOW_RANHE = new QToolButton(Dialog_fmdata);
        tbnRead_FLOW_RANHE->setObjectName(QStringLiteral("tbnRead_FLOW_RANHE"));
        tbnRead_FLOW_RANHE->setGeometry(QRect(500, 350, 91, 21));
        tbnWrite_FLOW_RANHE = new QToolButton(Dialog_fmdata);
        tbnWrite_FLOW_RANHE->setObjectName(QStringLiteral("tbnWrite_FLOW_RANHE"));
        tbnWrite_FLOW_RANHE->setGeometry(QRect(590, 350, 81, 21));
        label_3 = new QLabel(Dialog_fmdata);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 289, 131, 16));
        tbnWrite_ZERO_CAL = new QToolButton(Dialog_fmdata);
        tbnWrite_ZERO_CAL->setObjectName(QStringLiteral("tbnWrite_ZERO_CAL"));
        tbnWrite_ZERO_CAL->setGeometry(QRect(140, 440, 81, 21));
        tbnRead_ZERO_CAL = new QToolButton(Dialog_fmdata);
        tbnRead_ZERO_CAL->setObjectName(QStringLiteral("tbnRead_ZERO_CAL"));
        tbnRead_ZERO_CAL->setGeometry(QRect(50, 440, 91, 21));
        label_4 = new QLabel(Dialog_fmdata);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(50, 379, 81, 16));
        tbnFMRegister = new QToolButton(Dialog_fmdata);
        tbnFMRegister->setObjectName(QStringLiteral("tbnFMRegister"));
        tbnFMRegister->setGeometry(QRect(1040, 520, 131, 21));

        retranslateUi(Dialog_fmdata);

        QMetaObject::connectSlotsByName(Dialog_fmdata);
    } // setupUi

    void retranslateUi(QDialog *Dialog_fmdata)
    {
        Dialog_fmdata->setWindowTitle(QApplication::translate("Dialog_fmdata", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog_fmdata", "SET_KF1:", Q_NULLPTR));
        label_fmName->setText(QApplication::translate("Dialog_fmdata", "TextLabel", Q_NULLPTR));
        tbnWrite->setText(QApplication::translate("Dialog_fmdata", "\345\206\231\345\205\245", Q_NULLPTR));
        tbnRead->setText(QApplication::translate("Dialog_fmdata", "\350\257\273\345\217\226", Q_NULLPTR));
        tbnRead_SET_KF2->setText(QApplication::translate("Dialog_fmdata", "\350\257\273\345\217\226", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog_fmdata", "SET_KF2:", Q_NULLPTR));
        tbnWrite_SET_KF2->setText(QApplication::translate("Dialog_fmdata", "\345\206\231\345\205\245", Q_NULLPTR));
        tbnRead_FLOW_RANHE->setText(QApplication::translate("Dialog_fmdata", "\350\257\273\345\217\226", Q_NULLPTR));
        tbnWrite_FLOW_RANHE->setText(QApplication::translate("Dialog_fmdata", "\345\206\231\345\205\245", Q_NULLPTR));
        label_3->setText(QApplication::translate("Dialog_fmdata", "FLOW_RANGE:", Q_NULLPTR));
        tbnWrite_ZERO_CAL->setText(QApplication::translate("Dialog_fmdata", "\345\206\231\345\205\245", Q_NULLPTR));
        tbnRead_ZERO_CAL->setText(QApplication::translate("Dialog_fmdata", "\350\257\273\345\217\226", Q_NULLPTR));
        label_4->setText(QApplication::translate("Dialog_fmdata", "ZERO_CAL:", Q_NULLPTR));
        tbnFMRegister->setText(QApplication::translate("Dialog_fmdata", "\344\274\240\346\204\237\345\231\250\345\257\204\345\255\230\345\231\250", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_fmdata: public Ui_Dialog_fmdata {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_FMDATA_H

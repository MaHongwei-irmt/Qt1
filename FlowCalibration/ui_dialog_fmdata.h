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

    void setupUi(QDialog *Dialog_fmdata)
    {
        if (Dialog_fmdata->objectName().isEmpty())
            Dialog_fmdata->setObjectName(QStringLiteral("Dialog_fmdata"));
        Dialog_fmdata->resize(1292, 525);
        label = new QLabel(Dialog_fmdata);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 95, 54, 12));
        label_fmName = new QLabel(Dialog_fmdata);
        label_fmName->setObjectName(QStringLiteral("label_fmName"));
        label_fmName->setGeometry(QRect(50, 60, 271, 16));
        tbnWrite = new QToolButton(Dialog_fmdata);
        tbnWrite->setObjectName(QStringLiteral("tbnWrite"));
        tbnWrite->setGeometry(QRect(1110, 170, 81, 21));
        tbnRead = new QToolButton(Dialog_fmdata);
        tbnRead->setObjectName(QStringLiteral("tbnRead"));
        tbnRead->setGeometry(QRect(1020, 170, 91, 21));

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
    } // retranslateUi

};

namespace Ui {
    class Dialog_fmdata: public Ui_Dialog_fmdata {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_FMDATA_H

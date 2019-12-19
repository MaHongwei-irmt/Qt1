/********************************************************************************
** Form generated from reading UI file 'dialog_showinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_SHOWINFO_H
#define UI_DIALOG_SHOWINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Dialog_showinfo
{
public:
    QGridLayout *gridLayout;
    QTextBrowser *textBrow_calInfo;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_showCalTable;
    QPushButton *pushButton_print;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_printerSelect;
    QLabel *label_printer;
    QLineEdit *lineEdit_printer;
    QCheckBox *checkBox_printer;

    void setupUi(QDialog *Dialog_showinfo)
    {
        if (Dialog_showinfo->objectName().isEmpty())
            Dialog_showinfo->setObjectName(QStringLiteral("Dialog_showinfo"));
        Dialog_showinfo->resize(1369, 756);
        gridLayout = new QGridLayout(Dialog_showinfo);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textBrow_calInfo = new QTextBrowser(Dialog_showinfo);
        textBrow_calInfo->setObjectName(QStringLiteral("textBrow_calInfo"));

        gridLayout->addWidget(textBrow_calInfo, 0, 0, 1, 1);

        groupBox = new QGroupBox(Dialog_showinfo);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_showCalTable = new QPushButton(groupBox);
        pushButton_showCalTable->setObjectName(QStringLiteral("pushButton_showCalTable"));

        horizontalLayout->addWidget(pushButton_showCalTable);

        pushButton_print = new QPushButton(groupBox);
        pushButton_print->setObjectName(QStringLiteral("pushButton_print"));

        horizontalLayout->addWidget(pushButton_print);

        horizontalSpacer = new QSpacerItem(470, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_printerSelect = new QPushButton(groupBox);
        pushButton_printerSelect->setObjectName(QStringLiteral("pushButton_printerSelect"));

        horizontalLayout->addWidget(pushButton_printerSelect);

        label_printer = new QLabel(groupBox);
        label_printer->setObjectName(QStringLiteral("label_printer"));

        horizontalLayout->addWidget(label_printer);

        lineEdit_printer = new QLineEdit(groupBox);
        lineEdit_printer->setObjectName(QStringLiteral("lineEdit_printer"));

        horizontalLayout->addWidget(lineEdit_printer);

        checkBox_printer = new QCheckBox(groupBox);
        checkBox_printer->setObjectName(QStringLiteral("checkBox_printer"));

        horizontalLayout->addWidget(checkBox_printer);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);


        retranslateUi(Dialog_showinfo);

        QMetaObject::connectSlotsByName(Dialog_showinfo);
    } // setupUi

    void retranslateUi(QDialog *Dialog_showinfo)
    {
        Dialog_showinfo->setWindowTitle(QApplication::translate("Dialog_showinfo", "\344\277\241\346\201\257\347\252\227", Q_NULLPTR));
        textBrow_calInfo->setHtml(QApplication::translate("Dialog_showinfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
        groupBox->setTitle(QString());
        pushButton_showCalTable->setText(QApplication::translate("Dialog_showinfo", "\346\237\245\347\234\213\345\205\266\345\256\203\346\212\245\350\241\250", Q_NULLPTR));
        pushButton_print->setText(QApplication::translate("Dialog_showinfo", "\346\211\223\345\215\260", Q_NULLPTR));
        pushButton_printerSelect->setText(QApplication::translate("Dialog_showinfo", "\351\200\211\346\213\251\346\211\223\345\215\260\346\234\272", Q_NULLPTR));
        label_printer->setText(QApplication::translate("Dialog_showinfo", "\346\211\223\345\215\260\346\234\272\357\274\232", Q_NULLPTR));
        checkBox_printer->setText(QApplication::translate("Dialog_showinfo", "\350\207\252\345\212\250\346\211\223\345\215\260", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_showinfo: public Ui_Dialog_showinfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_SHOWINFO_H

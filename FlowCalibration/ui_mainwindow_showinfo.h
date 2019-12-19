/********************************************************************************
** Form generated from reading UI file 'mainwindow_showinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_SHOWINFO_H
#define UI_MAINWINDOW_SHOWINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow_showinfo
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow_showinfo)
    {
        if (MainWindow_showinfo->objectName().isEmpty())
            MainWindow_showinfo->setObjectName(QStringLiteral("MainWindow_showinfo"));
        MainWindow_showinfo->resize(800, 600);
        menubar = new QMenuBar(MainWindow_showinfo);
        menubar->setObjectName(QStringLiteral("menubar"));
        MainWindow_showinfo->setMenuBar(menubar);
        centralwidget = new QWidget(MainWindow_showinfo);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MainWindow_showinfo->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow_showinfo);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow_showinfo->setStatusBar(statusbar);

        retranslateUi(MainWindow_showinfo);

        QMetaObject::connectSlotsByName(MainWindow_showinfo);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow_showinfo)
    {
        MainWindow_showinfo->setWindowTitle(QApplication::translate("MainWindow_showinfo", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow_showinfo: public Ui_MainWindow_showinfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_SHOWINFO_H

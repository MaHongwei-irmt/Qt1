#-------------------------------------------------
#
# Project created by QtCreator 2019-07-05T08:54:04
#
#-------------------------------------------------

QT       += core gui serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FlowCalibration
TEMPLATE = app

win32{
QMAKE_LFLAGS=-static
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    qcustomplot.cpp \
    fsc_mainwindow.cpp \
    dialog_sensoraddr.cpp \
    fsc_mainwindow_plc.cpp \
    fsc_mainwindow_socket.cpp \
    fsc_thread.cpp \
    fsc_mainwindow_plc_ui.cpp \
    fsc_mainwindow_cal.cpp \
    dialog_showinfo.cpp \
    fsc_mainwindow_scale_fm.cpp \
    dialog_fmdata.cpp \
    fsc_mainwindow_cal_manual.cpp

HEADERS += \
    qcustomplot.h \
    fsc_mainwindow.h \
    dialog_sensoraddr.h \
    fsc_thread.h \
    fsc_plc.h \
    dialog_showinfo.h \
    dialog_showinfo.h \
    dialog_fmdata.h \
    fsc_pub.h

FORMS += \
    fsc_mainwindow.ui \
    dialog_sensoraddr.ui \
    dialog_showinfo.ui \
    dialog_showinfo.ui \
    dialog_fmdata.ui

RC_ICONS = FCImage.ico

RESOURCES +=

DISTFILES +=

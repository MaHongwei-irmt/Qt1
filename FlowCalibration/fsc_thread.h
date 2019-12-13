#ifndef FSC_Thread_H
#define FSC_Thread_H

#include <QObject>
#include<QThread>
#include<QtCore>

#include "fsc_mainwindow.h"

class FSC_Thread : public QThread
{
    Q_OBJECT

public:
    explicit FSC_Thread(QObject *parent = nullptr);
    ~FSC_Thread();
    FSC_MainWindow *mw;

protected:
    void run();

signals:
    void signal_startScketInit(int i);
};

#endif // FSC_Thread_H

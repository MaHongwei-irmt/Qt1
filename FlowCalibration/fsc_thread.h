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

    bool stop=false;



signals:
    void signal_startScketInit(int i);

};


//#include <QObject>
//#include<QThread>
//#include<QtCore>

//class MyThread : public QThread
//{
//    Q_OBJECT
//public:
//   explicit MyThread(QObject *parent = 0);
//    ~MyThread();

//public:
//    void run();
//    bool stop=false;
//signals:
//    void NumberChanged(int number);

//};


#endif // FSC_Thread_H

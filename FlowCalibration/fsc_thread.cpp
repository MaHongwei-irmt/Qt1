#include "fsc_thread.h"
#include "fsc_mainwindow.h"

FSC_Thread::FSC_Thread(QObject *parent)
{
    (void)parent;
}

FSC_Thread::~FSC_Thread()
{

}

void FSC_Thread::run()
{
    FSCLOG << "thread start ...";

    mw->SocketConnectTry();

    emit(signal_startScketInit(0));
    //exec();

    FSCLOG << "signal_startScketInit";
}

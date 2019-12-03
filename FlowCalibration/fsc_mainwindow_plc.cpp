#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"


void FSC_MainWindow::openForwardValveAll(void)
{
    openForwardValve1();
    openForwardValve2();
}

void FSC_MainWindow::openReverseValveAll(void)
{
    openReverseValve1();
    openReverseValve2();
}

void FSC_MainWindow::openForwardValve1(void)
{
   // plcStateWrite
}

void FSC_MainWindow::openForwardValve2(void)
{

}

void FSC_MainWindow::openReverseValve1(void)
{

}

void FSC_MainWindow::openReverseValve2(void)
{

}

void FSC_MainWindow::openOutValve(void)
{

}

void FSC_MainWindow::closeForwardValveAll(void)
{
    closeForwardValve1();
    closeForwardValve2();
}

void FSC_MainWindow::closeReverseValveAll(void)
{
    closeReverseValve1();
    closeReverseValve2();
}

void FSC_MainWindow::closeForwardValve1(void)
{

}

void FSC_MainWindow::closeForwardValve2(void)
{

}

void FSC_MainWindow::closeReverseValve1(void)
{

}

void FSC_MainWindow::closeReverseValve2(void)
{

}

void FSC_MainWindow::closeOutValve(void)
{

}

void FSC_MainWindow::pumpOn(void)
{

}

void FSC_MainWindow::pumpOff(void)
{

}


void FSC_MainWindow::on_tbnVOutOpen_clicked()
{
    openOutValve();
}

void FSC_MainWindow::on_tbnVForwardIn1Open_clicked()
{
    openForwardValve1();
}

void FSC_MainWindow::on_tbnVForwardIn2Open_clicked()
{
    openForwardValve2();
}

void FSC_MainWindow::on_tbnVReverseIn1Open_clicked()
{
    openReverseValve1();
}

void FSC_MainWindow::on_tbnVReverseIn2Open_clicked()
{
    openReverseValve2();
}

void FSC_MainWindow::on_tbnPumpForwardOn_clicked()
{
    //ui->tbnPumpForwardOn

    openForwardValveAll();
    Sleep(3000);
    pumpOn();
}

void FSC_MainWindow::on_tbnPumpReverseOn_clicked()
{
    openReverseValveAll();
    Sleep(3000);
    pumpOn();
}

void FSC_MainWindow::on_tbnVOutClose_clicked()
{
    closeOutValve();
}

void FSC_MainWindow::on_tbnVForwardIn1Close_clicked()
{
    closeForwardValve1();
}

void FSC_MainWindow::on_tbnVForwardIn2Close_clicked()
{
    closeForwardValve2();
}

void FSC_MainWindow::on_tbnVReverseIn1Close_clicked()
{
    closeReverseValve1();
}

void FSC_MainWindow::on_tbnVReverseIn2Close_clicked()
{
    closeReverseValve2();
}

void FSC_MainWindow::on_tbnPumpForwardOff_clicked()
{
    pumpOff();
    Sleep(3000);
    closeForwardValveAll();

}

void FSC_MainWindow::on_tbnPumpReverseOff_clicked()
{
    pumpOff();
    Sleep(3000);
    closeReverseValveAll();
}

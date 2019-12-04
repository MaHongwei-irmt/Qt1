#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

#define PLC_FORWARD_VALVE1_OFFSET   0
#define PLC_FORWARD_VALVE2_OFFSET   1
#define PLC_REVERSE_VALVE1_OFFSET   2
#define PLC_REVERSE_VALVE2_OFFSET   3
#define PLC_OUT_VALVE1_OFFSET       4
#define PLC_PUMP2_OFFSET            6
#define PLC_PUMP1_OFFSET            7

#define VALVE_EXCHANGE_DELAY        500
#define PUMP_START_DELAY            1500

void FSC_MainWindow::plcDataInit(void)
{
    plcStateWrite = 0;
}

void FSC_MainWindow::openForwardValve1(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_FORWARD_VALVE1_OFFSET;
}

void FSC_MainWindow::openForwardValve2(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_FORWARD_VALVE2_OFFSET;
}

void FSC_MainWindow::openReverseValve1(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_REVERSE_VALVE1_OFFSET;
}

void FSC_MainWindow::openReverseValve2(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_REVERSE_VALVE2_OFFSET;
}

void FSC_MainWindow::openOutValve(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_OUT_VALVE1_OFFSET;
}

void FSC_MainWindow::closeForwardValve1(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_FORWARD_VALVE1_OFFSET);
}

void FSC_MainWindow::closeForwardValve2(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_FORWARD_VALVE2_OFFSET);
}

void FSC_MainWindow::closeReverseValve1(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_REVERSE_VALVE1_OFFSET);
}

void FSC_MainWindow::closeReverseValve2(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_REVERSE_VALVE2_OFFSET);
}

void FSC_MainWindow::closeOutValve(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_OUT_VALVE1_OFFSET);
}

void FSC_MainWindow::pump1On(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_PUMP1_OFFSET;
}

void FSC_MainWindow::pump2On(void)
{
    uint8_t i = 0x01;
    plcStateWrite |= i << PLC_PUMP2_OFFSET;
}

void FSC_MainWindow::pump1Off(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_PUMP1_OFFSET);
}

void FSC_MainWindow::pump2Off(void)
{
    uint8_t i = 0x01;
    plcStateWrite &= ~(i << PLC_PUMP2_OFFSET);
}

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

void FSC_MainWindow::on_tbnVOutOpen_clicked()
{
    openOutValve();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 放水阀打开") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVOutClose_clicked()
{
    closeOutValve();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 放水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVForwardIn1Open_clicked()
{
    openForwardValve1();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 1#正向进水阀打开") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVForwardIn2Open_clicked()
{
    openForwardValve2();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 2#正向进水阀打开") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVReverseIn1Open_clicked()
{
    openReverseValve1();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 1#反向进水阀打开") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVReverseIn2Open_clicked()
{
    openReverseValve2();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 2#反向进水阀打开") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVForwardIn1Close_clicked()
{
    closeForwardValve1();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 1#正向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVForwardIn2Close_clicked()
{
    closeForwardValve2();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 2#正向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVReverseIn1Close_clicked()
{
    closeReverseValve1();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 1#反向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnVReverseIn2Close_clicked()
{
    closeReverseValve2();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 2#反向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump1ForwardOn_clicked()
{
    openForwardValveAll();
    Sleep(VALVE_EXCHANGE_DELAY);

    closeReverseValveAll();
    Sleep(PUMP_START_DELAY);

    pump1On();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 正向进水阀打开->反向进水阀关闭->1#泵启动") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump1ReverseOn_clicked()
{
    openReverseValveAll();
    reqSetPLC(showSetFlowRate, showSetPWM, 3, 4);

    Sleep(VALVE_EXCHANGE_DELAY);

    closeForwardValveAll();
    reqSetPLC(showSetFlowRate, showSetPWM, 5, 6);

    Sleep(PUMP_START_DELAY);

    pump1On();
    reqSetPLC(showSetFlowRate, showSetPWM, 7, 8);

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 反向进水阀打开->正向进水阀关闭->1#泵启动") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump1ForwardOff_clicked()
{
    pump1Off();
    Sleep(PUMP_START_DELAY);

    closeForwardValveAll();
    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 1#泵停止->正向进水阀关闭 ") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump1ReverseOff_clicked()
{
    pump1Off();
    Sleep(PUMP_START_DELAY);

    closeReverseValveAll();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 1#泵停止->反向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}


void FSC_MainWindow::on_tbnPump2ForwardOn_clicked()
{
    openForwardValveAll();
    Sleep(VALVE_EXCHANGE_DELAY);

    closeReverseValveAll();
    Sleep(PUMP_START_DELAY);

    pump2On();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 正向进水阀打开->反向进水阀关闭->2#泵启动") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump2ReverseOn_clicked()
{
    openReverseValveAll();
    Sleep(VALVE_EXCHANGE_DELAY);

    closeForwardValveAll();
    Sleep(PUMP_START_DELAY);

    pump2On();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 反向进水阀打开->正向进水阀关闭->2#泵启动") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump2ForwardOff_clicked()
{
    pump2Off();
    Sleep(PUMP_START_DELAY);

    closeForwardValveAll();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 2#泵停止->正向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::on_tbnPump2ReverseOff_clicked()
{
    pump2Off();
    Sleep(PUMP_START_DELAY);

    closeForwardValveAll();

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 2#泵停止->反向进水阀关闭") );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}


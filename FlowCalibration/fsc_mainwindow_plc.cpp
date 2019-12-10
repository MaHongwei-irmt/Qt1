#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"
#include "fsc_plc.h"

void FSC_MainWindow::delayMSec(int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void FSC_MainWindow::printInfo(QString str)
{
    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + str);
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

}

void FSC_MainWindow::printInfoWithTime(QString str)
{
    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz: ") + str );
    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

}

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

bool FSC_MainWindow::statePump1(void)
{
    return((plcStateRead >> PLC_PUMP1_OFFSET) & 0x01 );
}

bool FSC_MainWindow::statePump2(void)
{
    return((plcStateRead >> PLC_PUMP2_OFFSET) & 0x01 );
}

bool FSC_MainWindow::stateForwardV1(void)
{
    return((plcStateRead >> PLC_FORWARD_VALVE1_OFFSET) & 0x01 );
}

bool FSC_MainWindow::stateForwardV2(void)
{
    return((plcStateRead >> PLC_FORWARD_VALVE2_OFFSET) & 0x01 );
}


bool FSC_MainWindow::stateReverseV1(void)
{
    return((plcStateRead >> PLC_REVERSE_VALVE1_OFFSET) & 0x01 );
}

bool FSC_MainWindow::stateReverseV2(void)
{
    return((plcStateRead >> PLC_REVERSE_VALVE2_OFFSET) & 0x01 );
}

void FSC_MainWindow::reqSetPLC(void)
{
    sktBufSend[SOCKET_PLC_INDEX].resize(1);

    sktBufSend[SOCKET_PLC_INDEX][0]=0x10;

    flushSendBuf();
}

void FSC_MainWindow::writePLC(void)
{
    float f = QString::number(showSetFlowRate, 'f', 0).toFloat();
    QByteArray* baSnd= &sktBufSend[SOCKET_PLC_INDEX];

    sktBufSend[SOCKET_PLC_INDEX].resize(1 + sizeof(float) + sizeof(uint16_t) * 2);

    sktBufSend[SOCKET_PLC_INDEX][0]=0x01;
    memcpy(&(baSnd->data()[1]), &f, sizeof(float));

    if (ui->radioButton_setFlowRate->isChecked())
    {
        int i = 0;

        memcpy(&(baSnd->data()[1 + sizeof(float)]), &i, sizeof(uint16_t));
    }
    else
    {
        memcpy(&(baSnd->data()[1 + sizeof(float)]), &showSetPWM, sizeof(uint16_t));
    }

    memcpy(&(baSnd->data()[1 + sizeof(float) + sizeof(uint16_t)]), &plcStateWrite, sizeof(uint16_t));

    flushSendBuf();
}

bool FSC_MainWindow::parsePLC(int indexSkt)
{
    QByteArray *rev = &sktBufRev[indexSkt];
    QByteArray ba;
    float f;

    if (indexSkt != 0 )
    {
        return false;
    }

    revdSketPLC = true;

    if (sktBufRev[0].size() < 4)
    {
        return false;
    }

    ba.resize(sizeof(float));
    ba[0] = (*rev)[0];
    ba[1] = (*rev)[1];
    ba[2] = (*rev)[2];
    ba[3] = (*rev)[3];
    memcpy(&f, ba.data(), sizeof (f));
    plcRevFlowRate = static_cast<double>(f);

    ba[0] = (*rev)[4];
    ba[1] = (*rev)[5];
    memcpy(&plcRevPWM, ba.data(), sizeof (uint16_t));

    ba[0] = (*rev)[6];
    ba[1] = (*rev)[7];
    memcpy(&plcStateRead, ba.data(), sizeof (uint16_t));
    plcStateWrite = plcStateRead;

    ba[0] = (*rev)[11];
    ba[1] = (*rev)[10];
    ba[2] = (*rev)[9];
    ba[3] = (*rev)[8];
    memcpy(&f, ba.data(), sizeof (f));
    showSTDFMSum = static_cast<double>(f);

    ba[0] = (*rev)[15];
    ba[1] = (*rev)[14];
    ba[2] = (*rev)[13];
    ba[3] = (*rev)[12];
    memcpy(&f, ba.data(), sizeof (f));
    showSTDFMFlow = static_cast<double>(f);

    (*rev).resize(0);

    showPlcFresh();

    return true;
}

void FSC_MainWindow::showPlcFresh(void)
{
    ui->tbnVOutOpen->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVOutClose->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn1Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn1Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn2Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn2Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn1Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn1Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn2Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn2Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );

    ui->tbnPump1ForwardOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump1ForwardOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump1ReverseOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump1ReverseOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );

    ui->tbnPump2ForwardOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump2ForwardOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump2ReverseOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump2ReverseOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );


    ui->lineEdit_setFlowRate->setEnabled(revdSketPLC);
    ui->lineEdit_setPWM->setEnabled(revdSketPLC);
    ui->radioButton_setFlowRate->setEnabled(revdSketPLC);
    ui->radioButton_setPWM->setEnabled(revdSketPLC);
    if (!ui->radioButton_setFlowRate->isChecked())
    {
        ui->lineEdit_setFlowRate->setEnabled(false);
    }
    if (!ui->radioButton_setPWM->isChecked())
    {
        ui->lineEdit_setPWM->setEnabled(false);
    }

    if( (statePump1() || statePump2()))
    {
        ui->tbnVForwardIn1Open->setEnabled(false);
        ui->tbnVForwardIn1Close->setEnabled(false);
        ui->tbnVForwardIn2Open->setEnabled(false);
        ui->tbnVForwardIn2Close->setEnabled(false);
        ui->tbnVReverseIn1Open->setEnabled(false);
        ui->tbnVReverseIn1Close->setEnabled(false);
        ui->tbnVReverseIn2Open->setEnabled(false);
        ui->tbnVReverseIn2Close->setEnabled(false);
    }


    if ( statePump1() && stateForwardV1() && stateForwardV2() && !stateReverseV1() && !stateReverseV2())
    {
        ui->label_pump1State->setText("1#泵正向进水");

    }
    if ( statePump1() && stateReverseV1() && stateReverseV2() && !stateForwardV1() && !stateForwardV2())
    {
        ui->label_pump1State->setText("1#泵反向进水");
    }

    if ( statePump2() && stateForwardV1() && stateForwardV2() && !stateReverseV1() && !stateReverseV2())
    {
        ui->label_pump2State->setText("2#泵正向进水");
    }
    if ( statePump2() && stateReverseV1() && stateReverseV2() && !stateForwardV1() && !stateForwardV2())
    {
        ui->label_pump2State->setText("2#泵反向进水");
    }

    if (!statePump1())
    {
        ui->label_pump1State->setText("1#泵停止");
    }
    if (!statePump2())
    {
        ui->label_pump2State->setText("2#泵停止");
    }


    if( (statePump1() || statePump2()) && ( stateForwardV1() || stateForwardV2()) \
            && (stateReverseV1() || stateReverseV2()) )
    {
        printInfoWithTime("报警：阀位不对，关闭泵！");
        pump1Off();
        pump2Off();
    }

    if( (statePump1() || statePump2()) && ( !stateForwardV1() || !stateForwardV2()) \
            && (!stateReverseV1() || !stateReverseV2()) )
    {
        printInfoWithTime("报警：阀位不对，关闭泵！");
        pump1Off();
        pump2Off();
    }


    ui->revFlowRate->setText(QString::number(static_cast<double>(plcRevFlowRate), 'f', 0));
    ui->revPWM->setText(QString::number(plcRevPWM));
    ui->labelplcState->setText(PRINT_PLC_STATE_WRITE);
    ui->labelplcStateRead->setText(PRINT_PLC_STATE_READ);

    ui->label_stateVOutOn->setVisible( (plcStateRead >> PLC_OUT_VALVE1_OFFSET) & 0x01);
    ui->label_stateVOutOff->setVisible( !((plcStateRead >> PLC_OUT_VALVE1_OFFSET) & 0x01));

    ui->label_stateVFInOn1->setVisible(stateForwardV1());
    ui->label_stateVFInOff1->setVisible(!stateForwardV1());

    ui->label_stateVFInOn2->setVisible(stateForwardV2());
    ui->label_stateVFInOff2->setVisible(!stateForwardV2());

    ui->label_stateVRInOn1->setVisible(stateReverseV1());
    ui->label_stateVRInOff1->setVisible(!stateReverseV1());

    ui->label_stateVRInOn2->setVisible(stateReverseV2());
    ui->label_stateVRInOff2->setVisible(!stateReverseV2());

}

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
    ui->lineEdit_showPidPwm->setVisible(false);
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

void FSC_MainWindow::reqSetPLCWithSTFM(void)
{
    QByteArray ba;
    QByteArray* baSnd= &sktBufSend[SOCKET_PLC_INDEX];

    baSnd->resize(3);

    baSnd->data()[0]=0x10;

    ba.resize(sizeof(uint16_t));
    uint16_t i = static_cast<uint16_t>(showSTDFMFlow);
    memcpy(ba.data(), &i, sizeof(uint16_t));

    baSnd->data()[1] = ba[1];
    baSnd->data()[2] = ba[0];

    flushSendBuf();
}

void FSC_MainWindow::writePLC(void)
{
    if (!revdSketPLC) return;

    QByteArray ba;
    QByteArray* baSnd= &sktBufSend[SOCKET_PLC_INDEX];
    float f = QString::number(showSetFlowRate, 'f', 0).toFloat();

    baSnd->resize(1 + sizeof(float) + sizeof(uint16_t) * 2);

    baSnd->data()[0]=0x01;

    ba.resize(sizeof(float));
    memcpy(ba.data(), &f, sizeof(float));
    baSnd->data()[1] = ba[3];
    baSnd->data()[2] = ba[2];
    baSnd->data()[3] = ba[1];
    baSnd->data()[4] = ba[0];

    if (ui->radioButton_setFlowRate->isChecked())
    {
        int i = 0;

        memcpy(&(baSnd->data()[1 + sizeof(float)]), &i, sizeof(uint16_t));
    }
    else
    {
        memcpy(ba.data(), &showSetPWM, sizeof(uint16_t));
        baSnd->data()[5] = ba[1];
        baSnd->data()[6] = ba[0];

    }

    memcpy(&(baSnd->data()[1 + sizeof(float) + sizeof(uint16_t)]), &plcStateWrite, sizeof(uint16_t));

    flushSendBuf();
}

bool FSC_MainWindow::parsePLCNoSTFM(int indexSkt)
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
    ba[0] = (*rev)[3];
    ba[1] = (*rev)[2];
    ba[2] = (*rev)[1];
    ba[3] = (*rev)[0];
    memcpy(&f, ba.data(), sizeof (f));
    plcRevFlowRate = static_cast<double>(f);

    ba[0] = (*rev)[5];
    ba[1] = (*rev)[4];
    memcpy(&plcRevPWM, ba.data(), sizeof (uint16_t));

    ba[0] = (*rev)[6];
    ba[1] = (*rev)[7];
    memcpy(&plcStateRead, ba.data(), sizeof (uint16_t));

    if (plcStateWrite == 0xffff)
    {
        plcStateWrite = plcStateRead;
        writePLC();
    }
    if (plcStateWrite != plcStateRead)
    {
        plcRWErr++;
        writePLC();
    }
    else
    {
        plcRWErr = 0;
    }

    ba[0] = (*rev)[17];
    ba[1] = (*rev)[16];
    memcpy(&plcPidPWM, ba.data(), sizeof (uint16_t));

    (*rev).resize(0);

    showPlcFresh();

    return true;
}

bool FSC_MainWindow::parsePLC(int indexSkt)
{
    QByteArray *rev = &sktBufRev[indexSkt];
    QByteArray ba;
    float f;

    if (indexSkt != SOCKET_PLC_INDEX )
    {
        return false;
    }

    revdSketPLC = true;

    if (sktBufRev[0].size() < 4)
    {
        return false;
    }

    ba.resize(sizeof(float));
    ba[0] = (*rev)[3];
    ba[1] = (*rev)[2];
    ba[2] = (*rev)[1];
    ba[3] = (*rev)[0];
    memcpy(&f, ba.data(), sizeof (f));
    plcRevFlowRate = static_cast<double>(f);

    ba[0] = (*rev)[5];
    ba[1] = (*rev)[4];
    memcpy(&plcRevPWM, ba.data(), sizeof (uint16_t));

    ba[0] = (*rev)[6];
    ba[1] = (*rev)[7];
    memcpy(&plcStateRead, ba.data(), sizeof (uint16_t));

    if (plcStateWrite == 0xffff)
    {
        plcStateWrite = plcStateRead;
    }
    if (plcStateWrite != plcStateRead)
    {
        plcRWErr++;
        writePLC();
    }
    else
    {
        plcRWErr = 0;
    }

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

    ba[0] = (*rev)[16];
    ba[1] = (*rev)[17];
    memcpy(&plcPidPWM, ba.data(), sizeof (uint16_t));

    showPlcFresh();

    return true;
}

bool FSC_MainWindow::checkPlc(void)
{
    if (plcRWErr > 2) return false;
    return true;
}

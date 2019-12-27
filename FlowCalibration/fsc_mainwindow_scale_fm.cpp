#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::reqFMData(int indexFM)
{
    if (indexFM < SOCKET_STD_FLOWM_INDEX || indexFM > SOCKET_FLOWM12_INDEX)
    {
        return;
    }

    int     fmIdx = indexFM - SOCKET_FLOWM1_INDEX;
    bool    needToSend = false;

    if (sktRespondOk[indexFM])
    {
        needToSend = true;
    }

    if (fmIdx >= 0 && (!fmSendMsg[fmIdx].isEmpty() || !fmRevMsg[fmIdx].isEmpty()))
    {
        sktReqTime[indexFM] = QDateTime::currentDateTime().toTime_t();
    }

    if ((sktReqTime[indexFM] + FM_REQ_TIMEOUT) < QDateTime::currentDateTime().toTime_t())
    {
        needToSend = true;

        sktTimeoutNum[indexFM]++;
        if (sktTimeoutNum[indexFM] > 1)
        {
            sktTimeoutNum[indexFM] = 0;

            sktStationAddr[indexFM]++;
            if (sktStationAddr[indexFM] > 15)
            {
                sktStationAddr[indexFM] = 0;
            }
        }
    }

    if (needToSend)
    {
        if (fmIdx >= 0 && fmRWflag[fmIdx])
        {
            if (!fmRWTimer[fmIdx]->isActive())
            {
                fmRWTimer[fmIdx]->start(500);
            }

            sktRespondOk[indexFM] = false;
        }
        else
        {
            reqFMSumRateMsg(&sktBufSend[indexFM], sktStationAddr[indexFM]);
            flushSendBuf();

            sktRespondOk[indexFM] = false;
            sktReqTime[indexFM] = QDateTime::currentDateTime().toTime_t();
        }
    }
}

//ststcon + fun code(read: 0x03) + register address(0x97) + register number(flow sum + systime + up ATOF + down ATOF + DTOF + 1~50 flow rate) +crc
void FSC_MainWindow::reqFMSumRateMsg(QByteArray *buf, int station)
{
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = static_cast<char>(station);
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_READ_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(PROTOCOL_XUNYIN_MODBUS_ADDR_APP_REQ_DATA);
    (*buf)[4] = 0;
    (*buf)[5] = (5 + 50) * 2 ;

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);

}

bool FSC_MainWindow::preParseFMSumRateMsg(int indexSkt)
{
    uint16_t crc = 0;
    uint16_t crcRev = 0;
    uint8_t len;

    if (indexSkt < SOCKET_STD_FLOWM_INDEX || (sktBufRev[indexSkt].size() < 5))
    {
        return false;
    }

    if (indexSkt == SOCKET_STD_FLOWM_INDEX)
    {
        if (std::isnan(showSTDFMSum))
        {
            showSTDFMSum = 0;
            showSTDFMFlow = 0;
        }

        if ((sktBufRev[indexSkt][0] != sktStationAddr[indexSkt] )  || (sktBufRev[indexSkt].size() > 255))
        {
            sktBufRev[indexSkt].resize(0);
            return false;
        }
    }
    else
    {
        if (std::isnan(showFMSum[indexSkt - SOCKET_FLOWM1_INDEX]))
        {
            showFMSum[indexSkt - SOCKET_FLOWM1_INDEX] = 0;
            showFMFlow[indexSkt - SOCKET_FLOWM1_INDEX] = 0;
        }

        if ((sktBufRev[indexSkt][0] != sktStationAddr[indexSkt] )  || (sktBufRev[indexSkt].size() > 255))
        {
            sktBufRev[indexSkt].resize(0);
            return false;
        }
    }

    memcpy(&len, &sktBufRev[indexSkt].data()[2], 1);
    if ( (len + 5) !=  sktBufRev[indexSkt].size() )
    {
        if (sktBufRev[indexSkt].size() > (len + 5))
        {
            sktBufRev[indexSkt].resize(0);
        }

        return false;
    }

    crc = Checksum_computeChecksum( sktBufRev[indexSkt].data(), sktBufRev[indexSkt].size() - 2);
    memcpy(&crcRev, &(sktBufRev[indexSkt].data())[sktBufRev[indexSkt].size() - 2], 2);
    if ( crcRev != crc    )
    {
        sktBufRev[indexSkt].resize(0);
        return false;
    }

    return true;

}

bool FSC_MainWindow::parseFMSumRateMsg(int indexSkt)
{

    QByteArray ba;
    float f;
    float f1;

    if ( (sktBufRev[indexSkt].size() < 25) )
    {
        return false;
    }

    ba.resize(4);

    ba[0] = sktBufRev[indexSkt][1 + 3];
    ba[1] = sktBufRev[indexSkt][0 + 3];
    ba[2] = sktBufRev[indexSkt][3 + 3];
    ba[3] = sktBufRev[indexSkt][2 + 3];

    memcpy(&f, ba.data(), 4);

    ba[0] = sktBufRev[indexSkt][1 + 20 + 3];
    ba[1] = sktBufRev[indexSkt][0 + 20 + 3];
    ba[2] = sktBufRev[indexSkt][3 + 20 + 3];
    ba[3] = sktBufRev[indexSkt][2 + 20 + 3];

    memcpy(&f1, ba.data(), 4);


    sktBufRev[indexSkt].resize(0);


    if (indexSkt == SOCKET_STD_FLOWM_INDEX)
    {
        showSTDFMSum = static_cast<double>(f);
        showSTDFMFlow = static_cast<double>(f1);

        reqSetPLCWithSTFM();
    }
    else
    {
         showFMSum[indexSkt - SOCKET_FLOWM1_INDEX] = static_cast<double>(f);
         showFMFlow[indexSkt - SOCKET_FLOWM1_INDEX] = static_cast<double>(f1);


    }

    sktRespondOk[indexSkt] = true;

    return true;

}

bool FSC_MainWindow::fmRWTimerOn(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    if (fmCalibrating[fmIdx] == TISH_STEP_READ_WRITE)
    {
        fmCalibratingSingle(fmIdx);
    }

    return true;
}

bool FSC_MainWindow::fmCalibratingSingle(int fmIdx)
{

    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    //oneCalTag *calTag = &oneCal;
    //double delta = 0;

    fm_valueGAIN_CONTROL_valid[fmIdx] = 0;
    int k = 0;
    while(true)
    {
        sendMsg_readGAIN_CONTROL(fmIdx);

        int i = 0;
        while(fm_valueGAIN_CONTROL_valid[fmIdx] == 0)
        {
            delayMSec(100);

            i++;
            if (i > 10) break;

            printInfo("-");
        }

        k++;
        if (fm_valueGAIN_CONTROL_valid[fmIdx] != 0 || k > 1) break;

    }
    if (fm_valueGAIN_CONTROL_valid[fmIdx] == 0)
    {
        fmCalibrating[fmIdx] = TISH_STEP_READ_FAULT;
        //return false;
    }


    fm_write_suced[fmIdx] = 0;
    k = 0;
    while(true)
    {
        sendMsg_writeGAIN_CONTROL(fmIdx);

        int i = 0;
        while(fm_write_suced[fmIdx] == 0)
        {
            delayMSec(100);

            i++;
            if (i > 10) break;

            printInfo(".");
        }

        k++;
        if (fm_write_suced[fmIdx] != 0 || k > 1) break;
    }
    if (fm_write_suced[fmIdx] == 0)
    {
        fmCalibrating[fmIdx] = TISH_STEP_WRITE_FAULT;
        //return false;
    }


    fm_write_suced[fmIdx] = 0;
    k = 0;
    while(true)
    {
        sendMsg_writeUPDATE_REQ(fmIdx);

        int i = 0;
        while(fm_write_suced[fmIdx] == 0)
        {
            delayMSec(100);

            i++;
            if (i > 10) break;

            printInfo("+");
        }

        k++;
        if (fm_write_suced[fmIdx] != 0 || k > 1) break;
    }
    if (fm_write_suced[fmIdx] == 0)
    {
        fmCalibrating[fmIdx] = TISH_STEP_WRITE_FAULT;
        //return false;
    }

    fmSendMsg[fmIdx].clear();
    fmRevMsg[fmIdx].clear();


    reqFMSumRateMsg(&sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX], sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX]);
    flushSendBuf();

    sktRespondOk[fmIdx + SOCKET_FLOWM1_INDEX] = false;
    sktReqTime[fmIdx + SOCKET_FLOWM1_INDEX] = QDateTime::currentDateTime().toTime_t();

    fmCalibrating[fmIdx] = TISH_STEP_SUCCEED;


    return true;
}

bool FSC_MainWindow::sendMsg_readGAIN_CONTROL(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    QByteArray *buf = &sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX];
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX];
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_READ_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL);
    (*buf)[4] = 0;
    (*buf)[5] = 1;

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);

    fmSendMsg[fmIdx] = *buf;
    flushSendBuf();

    return true;
}

bool FSC_MainWindow::sendMsg_writeGAIN_CONTROL(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    QByteArray *buf = &sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX];
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX];
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL);
    (*buf)[4] = 0;
    (*buf)[5] = fm_valueGAIN_CONTROL[fmIdx];

    //delta = calTag->finalFMSumValue[fmIdx] - calTag->finalScaleSumValue;


    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);

    //oneCal

    fmSendMsg[fmIdx] = *buf;
    flushSendBuf();

    return true;
}

bool FSC_MainWindow::sendMsg_writeUPDATE_REQ(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    QByteArray *buf = &sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX];
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX];
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(PROTOCOL_XUNYIN_MODBUS_ADDR_APP_UPDATE_REQ);
    (*buf)[4] = 0;
    (*buf)[5] = 1;

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);


    fmSendMsg[fmIdx] = *buf;
    flushSendBuf();

    return true;
}

bool FSC_MainWindow::sendMsg_writeRESET(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    QByteArray *buf = &sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX];
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX];
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(PROTOCOL_XUNYIN_MODBUS_ADDR_RESET);
    (*buf)[4] = 0;
    (*buf)[5] = 1;

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);


    fmSendMsg[fmIdx] = *buf;
    flushSendBuf();

    return true;
}

bool FSC_MainWindow::preParseFMMsg(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    if (!fmSendMsg[fmIdx].isEmpty())
    {
        fmRevMsg[fmIdx].append(sktBufRev[fmIdx + SOCKET_FLOWM1_INDEX]);

        sktBufRev[fmIdx + SOCKET_FLOWM1_INDEX].clear();

        return true;
    }

    return false;
}

bool FSC_MainWindow::parseFMMsg(int fmIdx)
{

    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    if (fmRevMsg[fmIdx].size() < 5)
    {
        return false;
    }

    if (fmRevMsg[fmIdx].data()[1] == PROTOCOL_XUNYIN_MODBUS_READ_FUNCODE)
    {
        parseMsg_readGAIN_CONTROL(fmIdx);
    }
    else if(fmRevMsg[fmIdx].data()[1] == PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE)
    {
        parseMsg_writeSINGLE(fmIdx);
    }

    return true;
}

bool FSC_MainWindow::parseMsg_readGAIN_CONTROL(int fmIdx)
{
    uint16_t    crc     = 0;
    uint16_t    crcRev  = 0;
    uint8_t     len     = 0;

    if (fmRevMsg[fmIdx].size() < 7)
    {
        return false;
    }

    if (static_cast<uchar>(fmSendMsg[fmIdx].data()[3]) == PROTOCOL_XUNYIN_MODBUS_ADDR_APP_REQ_DATA)
    {
        len = 7;

        crc = Checksum_computeChecksum( fmRevMsg[fmIdx].data(), len - 2);
        memcpy(&crcRev, &(fmRevMsg[fmIdx].data()[len - 2]), 2);
        if ( crcRev != crc    )
        {
            return false;
        }

        fm_valueGAIN_CONTROL[fmIdx] = fmRevMsg[fmIdx].data()[4];
        fm_valueGAIN_CONTROL_valid[fmIdx] = 1;
    }

    return true;
}

bool FSC_MainWindow::parseMsg_writeSINGLE(int fmIdx)
{
    if (fmRevMsg[fmIdx].size() < 8)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        if (fmSendMsg[fmIdx].data()[i] != fmRevMsg[fmIdx].data()[i])
        {
            return false;
        }
    }

    fm_write_suced[fmIdx] = 1;

    return true;
}

void FSC_MainWindow::reqScaleShow(void)
{
    sktBufSend[SOCKET_SCALE_INDEX].resize(2);
    sktBufSend[SOCKET_SCALE_INDEX][0]=0x1B;
    sktBufSend[SOCKET_SCALE_INDEX][1]=0x70;

    flushSendBuf();
}

void FSC_MainWindow::reqScaleZero(void)
{
    sktBufSend[SOCKET_SCALE_INDEX].resize(2);
    sktBufSend[SOCKET_SCALE_INDEX][0]=0x1B;
    sktBufSend[SOCKET_SCALE_INDEX][1]=0x74;

    flushSendBuf();
}

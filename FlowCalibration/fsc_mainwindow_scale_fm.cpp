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

    if(stfmRWflag || (fmIdx >= 0 && fmRWflag[fmIdx]))
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
        else if(stfmRWflag)
        {
            if (!stfmRWTimer->isActive())
            {
                stfmRWTimer->start(500);
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

bool FSC_MainWindow::stfmRWTimerOn(void)
{
    bool ret = false;

    stfm_write_suced = 0;
    int k = 0;
    while(true)
    {
        sendMsg_writeStfmRESET();

        int i = 0;
        while(stfm_write_suced == 0)
        {
            delayMSec(FM_MSG_WAIT_DELAY);

            i++;
            if (i > FM_MSG_RW_TIMEOUT)
            {
                break;
            }

            printInfo("=");
        }

        k++;
        if (stfm_write_suced != 0 || k > 1)
        {
            break;
        }
    }
    if (stfm_write_suced == 0)
    {
        stfmReadWriteSelect = TISH_STEP_WRITE_FAULT;
    }
    else
    {
        stfmReadWriteSelect = TISH_STEP_SUCCEED;
        ret = true;
    }

    stfmSendMsg.clear();
    stfmRevMsg.clear();

    reqFMSumRateMsg(&sktBufSend[SOCKET_STD_FLOWM_INDEX], sktStationAddr[SOCKET_STD_FLOWM_INDEX]);
    flushSendBuf();

    sktRespondOk[SOCKET_STD_FLOWM_INDEX] = false;
    sktReqTime[SOCKET_STD_FLOWM_INDEX] = QDateTime::currentDateTime().toTime_t();

    return ret;
}

bool FSC_MainWindow::fmRWTimerOn(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    if (fmReadWriteSelect[fmIdx] == CALIBRATION_FM_READ_WRITE)
    {
        fmCalibrationSingle(fmIdx);
    }
    else if (fmReadWriteSelect[fmIdx] == RESET_FM_READ_WRITE)
    {
        fmResetSingle(fmIdx);
    }
    else if (fmReadWriteSelect[fmIdx] == CORRECT_FM_READ_WRITE)
    {
        fmCorrectSingle(fmIdx);
    }

    fmSendMsg[fmIdx].clear();
    fmRevMsg[fmIdx].clear();

    reqFMSumRateMsg(&sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX], sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX]);
    flushSendBuf();

    sktRespondOk[fmIdx + SOCKET_FLOWM1_INDEX] = false;
    sktReqTime[fmIdx + SOCKET_FLOWM1_INDEX] = QDateTime::currentDateTime().toTime_t();

    return true;
}

bool FSC_MainWindow::fmCorrectSingle(int fmIdx)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    fm_write_suced[fmIdx] = 0;
    int k = 0;
    while(true)
    {
        sendMsg_writeRESET(fmIdx);

        int i = 0;
        while(fm_write_suced[fmIdx] == 0)
        {
            delayMSec(FM_MSG_WAIT_DELAY);

            i++;
            if (i > FM_MSG_RW_TIMEOUT)
            {
                break;
            }

            printInfo("|");
        }

        k++;
        if (fm_write_suced[fmIdx] != 0 || k > 1)
        {
            break;
        }
    }
    if (fm_write_suced[fmIdx] == 0)
    {
        fmReadWriteSelect[fmIdx] = TISH_STEP_WRITE_FAULT;
        return false;
    }

    fmReadWriteSelect[fmIdx] = TISH_STEP_SUCCEED;

    return true;
}

bool FSC_MainWindow::writeFM(int fmIdx, int idCode)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    fm_write_suced[fmIdx] = 0;
    int k = 0;
    while(true)
    {
        if (idCode == XUNYIN_SINGLE_WRITE_RESET)
        {
            sendMsg_writeRESET(fmIdx);
            printInfo("writeRESET");
        }
        else if  (idCode == XUNYIN_SINGLE_WRITE_GAIN_CONTROL)
        {
            sendMsg_writeGAIN_CONTROL(fmIdx);
            printInfo("writeGAIN_CONTROL");
        }
        else if  (idCode == XUNYIN_SINGLE_WRITE_UPDATE_REQ)
        {
            sendMsg_writeUPDATE_REQ(fmIdx);
            printInfo("writeUPDATE_REQ");
        }


        int i = 0;
        while(fm_write_suced[fmIdx] == 0)
        {
            delayMSec(FM_MSG_WAIT_DELAY);

            i++;
            if (i > FM_MSG_RW_TIMEOUT)
            {
                break;
            }

            printInfo(">");
        }

        k++;
        if (fm_write_suced[fmIdx] != 0 || k > 1)
        {
            break;
        }
    }
    if (fm_write_suced[fmIdx] == 0)
    {
        fmReadWriteSelect[fmIdx] = TISH_STEP_WRITE_FAULT;
        return false;
    }

    fmReadWriteSelect[fmIdx] = TISH_STEP_SUCCEED;
    return true;
}

bool FSC_MainWindow::readFM(int fmIdx, int idCode)
{
    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    //oneCalTag *calTag = &oneCal;
    //double delta = 0;


    fm_valu_read_valid[fmIdx] = 0;
    int k = 0;
    while(true)
    {
        if (idCode == XUNYIN_READ_GAIN_CONTROL)
        {
            sendMsg_readGAIN_CONTROL(fmIdx);
            printInfo("readGAIN_CONTROL");
        }

        int i = 0;
        while(fm_valu_read_valid[fmIdx] == 0)
        {
            delayMSec(FM_MSG_WAIT_DELAY);

            i++;
            if (i > FM_MSG_RW_TIMEOUT)
            {
                break;
            }

            printInfo("<");
        }

        k++;
        if (fm_valu_read_valid[fmIdx] != 0 || k > 1)
        {
            break;
        }

    }
    if (fm_valu_read_valid[fmIdx] == 0)
    {
        fmReadWriteSelect[fmIdx] = TISH_STEP_READ_FAULT;
        return false;
    }

    fmReadWriteSelect[fmIdx] = TISH_STEP_SUCCEED;
    return true;
}

bool FSC_MainWindow::fmResetSingle(int fmIdx)
{
    return (writeFM(fmIdx, XUNYIN_SINGLE_WRITE_RESET));
}

bool FSC_MainWindow::fmCalibrationSingle(int fmIdx)
{
    bool ret= true;

    ret = readFM(fmIdx, XUNYIN_READ_GAIN_CONTROL);

    //if (ret)
    {
       ret =  writeFM(fmIdx, XUNYIN_SINGLE_WRITE_GAIN_CONTROL);
    }

    //if (ret)
    {
       ret =  writeFM(fmIdx, XUNYIN_SINGLE_WRITE_UPDATE_REQ);
    }

    return ret;
}

bool FSC_MainWindow::sendMsg_read_byAddr(int fmIdx, uchar addr, char num)
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
    (*buf)[3] = static_cast<char>(addr);
    (*buf)[4] = 0;
    (*buf)[5] = num;

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);

    fmSendMsg[fmIdx] = *buf;
    flushSendBuf();

    return true;
}

bool FSC_MainWindow::sendMsg_writeSingle_byAddr(int fmIdx, uchar addr, uint16_t value)
{
    if (fmIdx < -1 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    QByteArray *buf = &sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX];
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX];
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(addr);
    (*buf)[4] = static_cast<char>(value >> 8);
    (*buf)[5] = static_cast<char>(value);

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);

    if (fmIdx == -1)
    {
        stfmSendMsg = *buf;
    }
    else
    {
        fmSendMsg[fmIdx] = *buf;
    }

    flushSendBuf();

    return true;
}

bool FSC_MainWindow::sendMsg_readGAIN_CONTROL(int fmIdx)
{
    return(sendMsg_read_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL, 1));
}

bool FSC_MainWindow::sendMsg_writeGAIN_CONTROL(int fmIdx)
{
    return(sendMsg_writeSingle_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL, \
                                       static_cast<uint16_t>(fm_valueGAIN_CONTROL[fmIdx])));
}

bool FSC_MainWindow::sendMsg_writeUPDATE_REQ(int fmIdx)
{
    return(sendMsg_writeSingle_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_APP_UPDATE_REQ, 1));
}

bool FSC_MainWindow::sendMsg_writeStfmRESET(void)
{
    return(sendMsg_writeSingle_byAddr(-1, PROTOCOL_XUNYIN_MODBUS_ADDR_RESET, 1));
}

bool FSC_MainWindow::sendMsg_writeRESET(int fmIdx)
{
    return(sendMsg_writeSingle_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_RESET, 1));
}

bool FSC_MainWindow::preParseStfmMsg(void)
{
    if (!stfmSendMsg.isEmpty())
    {
        stfmRevMsg.append(sktBufRev[SOCKET_STD_FLOWM_INDEX]);

        sktBufRev[SOCKET_STD_FLOWM_INDEX].clear();

        return true;
    }

    return false;
}

bool FSC_MainWindow::parseStfmMsg(void)
{
    if (stfmRevMsg.size() < 5)
    {
        return false;
    }

    if(stfmRevMsg.data()[1] == PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE)
    {
        parseMsg_writeStfmSINGLE();
    }

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
        fm_valu_read_valid[fmIdx] = 1;
    }

    return true;
}

bool FSC_MainWindow::parseMsg_writeStfmSINGLE(void)
{
    if (stfmRevMsg.size() < 8)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        if (stfmSendMsg.data()[i] != stfmRevMsg.data()[i])
        {
            return false;
        }
    }

    stfm_write_suced = 1;

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

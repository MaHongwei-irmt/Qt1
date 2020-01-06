#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::resetFmCommunicationWithStfm(void)
{
    for (int i = SOCKET_STD_FLOWM_INDEX; i <= SOCKET_FLOWM12_INDEX; i++)
    {
        sktRespondOk[i] = true;
        sktReqTime[i] = QDateTime::currentDateTime().toTime_t();

        if (i >= SOCKET_FLOWM1_INDEX)
        {
            fmSendMsg[i].clear();
        }
    }

    stfmSendMsg.clear();
}

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
        sktTimeoutNum[indexFM] = 0;

    }

    if(stfmRWflag || (fmIdx >= 0 && fmRWflag[fmIdx]))
    {
        sktReqTime[indexFM] = QDateTime::currentDateTime().toTime_t();
    }

    if ((sktReqTime[indexFM] + FM_REQ_TIMEOUT) < QDateTime::currentDateTime().toTime_t())
    {
        if (sktPause[indexFM])
        {
             sktReqTime[indexFM] = QDateTime::currentDateTime().toTime_t();
        }
        else
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

            sktRespondOk[indexFM] = true;
            sktReqTime[indexFM] = QDateTime::currentDateTime().toTime_t();
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
            if (!sktPause[indexFM])
            {
                reqFMSumRateMsg(&sktBufSend[indexFM], sktStationAddr[indexFM]);
                flushSendBuf();

                sktRespondOk[indexFM] = false;
            }
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
        else if  (idCode == XUNYIN_READ_SET_KF1)
        {
            sendMsg_writeSET_KF1(fmIdx);
            printInfo("writeSET_KF1");
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
        else if (idCode == XUNYIN_READ_SET_KF1)
        {
            sendMsg_readSET_KF1(fmIdx);
            printInfo("readSET_KF1");
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

bool FSC_MainWindow::fmCorrectSingle(int fmIdx)
{
    bool ret = true;

    ret = readFM(fmIdx, XUNYIN_READ_SET_KF1);

    //if (ret)
    {
       ret =  writeFM(fmIdx, XUNYIN_READ_SET_KF1);
    }

//    //if (ret)
//    {
//       ret =  writeFM(fmIdx, XUNYIN_SINGLE_WRITE_UPDATE_REQ);
//    }

    return ret;
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

bool FSC_MainWindow::sendMsg_writeMulti_byAddr(int fmIdx, uchar addr, uint16_t num, QByteArray *ba)
{
    if (fmIdx < -1 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return false;
    }

    QByteArray *buf = &sktBufSend[fmIdx + SOCKET_FLOWM1_INDEX];
    uint16_t crc = 0;

    buf->resize(9 + num * 2);

    (*buf)[0] = sktStationAddr[fmIdx + SOCKET_FLOWM1_INDEX];
    (*buf)[1] = PROTOCOL_XUNYIN_MODBUS_WRITE_MULTIPLE_FUNCODE;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(addr);

    (*buf)[4] = static_cast<char>(num >> 8);
    (*buf)[5] = static_cast<char>(num);

    (*buf)[6] = static_cast<char>(num * 2);

    memcpy(&((*buf).data()[7]), ba->data(), num * 2);

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[7 + num * 2] = static_cast<char>(crc) ;
    (*buf)[7 + num * 2 + 1] = static_cast<char>(crc >> 8);

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

bool FSC_MainWindow::sendMsg_readZERO_CAL(int fmIdx)
{
    return(sendMsg_read_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_ZERO_CAL, XUNYIN_SET_ZERO_CAL_NUM * 2));
}
bool FSC_MainWindow::sendMsg_writeZERO_CAL(int fmIdx)
{
    QByteArray ba;

    ba_fm_tmp.resize(XUNYIN_SET_FLOW_RANGE_NUM * 2 * 2);
    ba.resize(sizeof (float));

    for (int i = 0; i < XUNYIN_SET_ZERO_CAL_NUM; i++)
    {
        memcpy(ba.data(), &fmData[fmIdx].fm_valueZERO_CAL[i], sizeof (float));

        ba_fm_tmp[i * 4 + 0] = ba[1];
        ba_fm_tmp[i * 4 + 1] = ba[0];
        ba_fm_tmp[i * 4 + 2] = ba[3];
        ba_fm_tmp[i * 4 + 3] = ba[2];
    }

    return(sendMsg_writeMulti_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_ZERO_CAL, XUNYIN_SET_ZERO_CAL_NUM * 2, &ba_fm_tmp));
}

bool FSC_MainWindow::sendMsg_readFLOW_RANGE(int fmIdx)
{
    return(sendMsg_read_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_FLOW_RANGE, XUNYIN_SET_FLOW_RANGE_NUM * 2));
}
bool FSC_MainWindow::sendMsg_writeFLOW_RANGE(int fmIdx)
{
    QByteArray ba;

    ba_fm_tmp.resize(XUNYIN_SET_FLOW_RANGE_NUM * 2 * 2);
    ba.resize(sizeof (float));

    for (int i = 0; i < XUNYIN_SET_FLOW_RANGE_NUM; i++)
    {
        memcpy(ba.data(), &fmData[fmIdx].fm_valueFLOW_RANGE[i], sizeof (float));

        ba_fm_tmp[i * 4 + 0] = ba[1];
        ba_fm_tmp[i * 4 + 1] = ba[0];
        ba_fm_tmp[i * 4 + 2] = ba[3];
        ba_fm_tmp[i * 4 + 3] = ba[2];
    }

    return(sendMsg_writeMulti_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_FLOW_RANGE, XUNYIN_SET_FLOW_RANGE_NUM * 2, &ba_fm_tmp));
}

bool FSC_MainWindow::sendMsg_readSET_KF1(int fmIdx)
{
    return(sendMsg_read_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_SET_KF1, 38));
}

bool FSC_MainWindow::sendMsg_readSET_KF2(int fmIdx)
{
    return(sendMsg_read_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_SET_KF2, 38));
}

bool FSC_MainWindow::sendMsg_readGAIN_CONTROL(int fmIdx)
{
    return(sendMsg_read_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL, 1));
}

bool FSC_MainWindow::sendMsg_writeGAIN_CONTROL(int fmIdx)
{
    return(sendMsg_writeSingle_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL, \
                                       static_cast<uint16_t>(fmData[fmIdx].fm_valueGAIN_CONTROL)));
}

bool FSC_MainWindow::sendMsg_writeUPDATE_REQ(int fmIdx)
{
    return(sendMsg_writeSingle_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_APP_UPDATE_REQ, 1));
}

bool FSC_MainWindow::sendMsg_writeSET_KF1(int fmIdx)
{
    QByteArray ba_kf1;

    ba_fm_tmp.resize(XUNYIN_SET_KF_NUM * 2 * 2);
    ba_kf1.resize(sizeof (float));

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        memcpy(ba_kf1.data(), &fmData[fmIdx].fm_valueSET_KF1[i], sizeof (float));

        ba_fm_tmp[i * 4 + 0] = ba_kf1[1];
        ba_fm_tmp[i * 4 + 1] = ba_kf1[0];
        ba_fm_tmp[i * 4 + 2] = ba_kf1[3];
        ba_fm_tmp[i * 4 + 3] = ba_kf1[2];
    }

    return(sendMsg_writeMulti_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_SET_KF1, XUNYIN_SET_KF_NUM * 2, &ba_fm_tmp));
}

bool FSC_MainWindow::sendMsg_writeSET_KF2(int fmIdx)
{
    QByteArray ba_kf2;

    ba_fm_tmp.resize(XUNYIN_SET_KF_NUM * 2 * 2);
    ba_kf2.resize(sizeof (float));

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        memcpy(ba_kf2.data(), &fmData[fmIdx].fm_valueSET_KF2[i], sizeof (float));

        ba_fm_tmp[i * 4 + 0] = ba_kf2[1];
        ba_fm_tmp[i * 4 + 1] = ba_kf2[0];
        ba_fm_tmp[i * 4 + 2] = ba_kf2[3];
        ba_fm_tmp[i * 4 + 3] = ba_kf2[2];
    }

    return(sendMsg_writeMulti_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_SET_KF2, XUNYIN_SET_KF_NUM * 2, &ba_fm_tmp));
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
        parseMsg_readFm(fmIdx);
    }
    else if(fmRevMsg[fmIdx].data()[1] == PROTOCOL_XUNYIN_MODBUS_WRITE_SINGLE_FUNCODE)
    {
        parseMsg_writeSINGLE(fmIdx);
    }
    else if(fmRevMsg[fmIdx].data()[1] == PROTOCOL_XUNYIN_MODBUS_WRITE_MULTIPLE_FUNCODE)
    {
        parseMsg_writeMUTIL(fmIdx);
    }
    return true;
}

bool FSC_MainWindow::parseMsg_readFm(int fmIdx)
{
    uint16_t    crc     = 0;
    uint16_t    crcRev  = 0;
    uint8_t     len     = 0;

    QByteArray ba;

    if (fmRevMsg[fmIdx].size() < 7)
    {
        return false;
    }

    len = static_cast<uchar>(fmRevMsg[fmIdx].data()[2]) + 5;

    if (fmRevMsg[fmIdx].size() < len)
    {
        return false;
    }

    crc = Checksum_computeChecksum( fmRevMsg[fmIdx].data(), len - 2);
    memcpy(&crcRev, &(fmRevMsg[fmIdx].data()[len - 2]), 2);
    if ( crcRev != crc    )
    {
        fmRevMsg[fmIdx].clear();
        FSCLOG << "crc error.";
        return false;
    }

    switch (static_cast<uchar>(fmSendMsg[fmIdx].data()[3]))
    {
    case PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL:

        if (len != 5 + 2)
        {
            return false;
        }

        fmData[fmIdx].fm_valueGAIN_CONTROL = fmRevMsg[fmIdx].data()[4];


        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x84_GAIN_CONTROL, ba.data(), sizeof (uint16_t));

        break;

    case PROTOCOL_XUNYIN_MODBUS_ADDR_SET_KF1:

        if (len != 5 + XUNYIN_SET_KF_NUM * sizeof(float))
        {
            return false;
        }

        ba.resize(4);
        for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_valueSET_KF1[i], ba.data(), sizeof (float));

            fmData[fmIdx].fm_0xe5_SET_KF1[i] = fmData[fmIdx].fm_valueSET_KF1[i];
        }

        break;

    case PROTOCOL_XUNYIN_MODBUS_ADDR_SET_KF2:

        if (len != 5 + XUNYIN_SET_KF_NUM * sizeof(float))
        {
            return false;
        }

        ba.resize(4);
        for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_valueSET_KF2[i], ba.data(), sizeof (float));

            fmData[fmIdx].fm_0xe6_SET_KF2[i] = fmData[fmIdx].fm_valueSET_KF2[i];
        }

        break;

    case PROTOCOL_XUNYIN_MODBUS_ADDR_FLOW_RANGE:

        if (len != 5 + XUNYIN_SET_FLOW_RANGE_NUM * sizeof(float))
        {
            return false;
        }

        ba.resize(4);
        for (int i = 0; i < XUNYIN_SET_FLOW_RANGE_NUM; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_valueFLOW_RANGE[i], ba.data(), sizeof (float));

            fmData[fmIdx].fm_0xe1_FLOW_RANGE[i] =  fmData[fmIdx].fm_valueFLOW_RANGE[i];
        }


        break;

    case PROTOCOL_XUNYIN_MODBUS_ADDR_ZERO_CAL:

        if (len != 5 + XUNYIN_SET_ZERO_CAL_NUM * sizeof(float))
        {
            return false;
        }

        ba.resize(4);
        for (int i = 0; i < XUNYIN_SET_ZERO_CAL_NUM; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_valueZERO_CAL[i], ba.data(), sizeof (float));

            fmData[fmIdx].fm_0xeB_ZERO_CAL[i] = fmData[fmIdx].fm_valueZERO_CAL[i];
        }

        break;

    case 0x80:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x80_GAP_PLS_ADC_START, ba.data(), sizeof (uint16_t));

        break;

    case 0x81:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x81_NUM_PLS, ba.data(), sizeof (uint16_t));

        break;

    case 0x82:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x82_GAP_UPS_DNS, ba.data(), sizeof (uint16_t));

        break;

    case 0x83:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x83_GAP_UPS_UPS, ba.data(), sizeof (uint16_t));

        break;


    case 0x85:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x85_METER_CONSTANT, ba.data(), sizeof (uint16_t));

        break;

    case 0x86:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x86_XT2_FREQ, ba.data(), sizeof (uint16_t));

        break;

    case 0x87:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x87_ADC_SAMP_FREQ, ba.data(), sizeof (uint16_t));

        break;


    case 0x88:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x88_SIG_SAMP_FREQ, ba.data(), sizeof (uint16_t));

        break;

    case 0x89:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x89_ADC_OVERSAMPLING, ba.data(), sizeof (uint16_t));

        break;


    case 0x8a:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x88_SIG_SAMP_FREQ, ba.data(), sizeof (uint32_t));

        break;

    case 0x8b:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x89_ADC_OVERSAMPLING, ba.data(), sizeof (float));

        break;


    case 0x8c:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x8C_CAPTURE_DURATION, ba.data(), sizeof (uint16_t));

        break;


    case 0x8d:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x8D_PARAM1, ba.data(), sizeof (uint32_t));

        break;

    case 0x8e:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x8E_PARAM2, ba.data(), sizeof (uint32_t));

        break;


    case 0x8f:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x8F_PARAM3, ba.data(), sizeof (uint32_t));

        break;

    case 0x90:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x90_PARAM4, ba.data(), sizeof (uint32_t));

        break;


    case 0x9e:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x9E_PARAM5, ba.data(), sizeof (uint32_t));

        break;


    case 0x9f:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x9F_PARAM6, ba.data(), sizeof (uint32_t));

        break;


    case 0x91:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x91_PARAM7, ba.data(), sizeof (uint32_t));

        break;


    case 0x92:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x92_PARAM8, ba.data(), sizeof (uint32_t));

        break;


    case 0x93:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x93_PARAM9, ba.data(), sizeof (uint32_t));

        break;


    case 0x94:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[2] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[3] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x94_PARAM10, ba.data(), sizeof (uint32_t));

        break;


    case 0x96:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x96_APP_UPDATE_REQ, ba.data(), sizeof (uint16_t));

        break;

    case 0x98:

    {
        int num = fmRevMsg[fmIdx].data()[2];

        if (num > static_cast<int>(sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t)) )
        {
            num =  sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t);
        }

        ba.resize(2);

        for (int i = 0; i < num; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];

            memcpy(&fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1[i], ba.data(), static_cast<uint>(num) );
        }
    }

        break;


    case 0x99:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0x99_APP_VERSION, ba.data(), sizeof (uint16_t));

        break;


    case 0x9b:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[0] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0x9B_F1_F2, ba.data(), sizeof (uint16_t));

        break;


    case 0xe0:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xe0_AUTO_GAIN_ENABLE, ba.data(), sizeof (uint16_t));

        break;


    case 0xe2:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xe2_RUNNING_AVG, ba.data(), sizeof (uint16_t));

        break;

    case 0xe3:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[0] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0xe3_SET_DAC, ba.data(), sizeof (uint32_t));

        break;

    case 0xe4:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[0] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0xe4_SET_KF, ba.data(), sizeof (uint32_t));

        break;

    case 0xe7:

        ba.resize(4);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];
        ba[0] = fmRevMsg[fmIdx].data()[3 + 3];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 2];

        memcpy(&fmData[fmIdx].fm_0xe7_SET_SNSR_TYP, ba.data(), sizeof (uint32_t));

        break;

    case 0xe8:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xe8_LNCAL_EN, ba.data(), sizeof (uint16_t));

        break;

    case 0xe9:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xe9_STA_UP, ba.data(), sizeof (uint16_t));

        break;

    case 0xea:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xeA_RESET, ba.data(), sizeof (uint16_t));

        break;

    case 0xec:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xeC_AUTO_GAIN_UP, ba.data(), sizeof (uint16_t));

        break;

    case 0xed:

        ba.resize(2);

        ba[0] = fmRevMsg[fmIdx].data()[3 + 1];
        ba[1] = fmRevMsg[fmIdx].data()[3 + 0];

        memcpy(&fmData[fmIdx].fm_0xeD_ADDR, ba.data(), sizeof (uint16_t));

        break;

    case 0xef:

    {
        int num = fmRevMsg[fmIdx].data()[2];

        if (num > static_cast<int>(sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t)) )
        {
            num =  sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t);
        }

        ba.resize(4);

        for (int i = 0; i < num; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_0xef_APP_REQ_CAPTURES2[i], ba.data(), static_cast<uint>(num) );
        }
    }


        break;

    case 0xf0:

    {
        int num = fmRevMsg[fmIdx].data()[2];

        if (num > static_cast<int>(sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t)) )
        {
            num =  sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t);
        }

        ba.resize(4);

        for (int i = 0; i < num; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_0xf0_APP_REQ_CAPTURES3[i], ba.data(), static_cast<uint>(num) );
        }
    }


    break;

    case 0xf1:

    {
        int num = fmRevMsg[fmIdx].data()[2];

        if (num > static_cast<int>(sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t)) )
        {
            num =  sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t);
        }

        ba.resize(4);

        for (int i = 0; i < num; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_0xf1_APP_REQ_CAPTURES4[i], ba.data(), static_cast<uint>(num) );
        }
    }


        break;


    case 0x97:

    {
        int num = fmRevMsg[fmIdx].data()[2];

        if (num > static_cast<int>(sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t)) )
        {
            num =  sizeof (fmData[fmIdx].fm_0x98_APP_REQ_CAPTURES1) / sizeof (uint16_t);
        }

        ba.resize(4);

        for (int i = 0; i < num; i++)
        {
            ba[0] = fmRevMsg[fmIdx].data()[3 + i * 4 + 1];
            ba[1] = fmRevMsg[fmIdx].data()[3 + i * 4 + 0];
            ba[2] = fmRevMsg[fmIdx].data()[3 + i * 4 + 3];
            ba[3] = fmRevMsg[fmIdx].data()[3 + i * 4 + 2];

            memcpy(&fmData[fmIdx].fm_0x97_APP_REQ_DATA[i], ba.data(), static_cast<uint>(num) );
        }
    }


        break;



    default:
        break;

    }

    fm_valu_read_valid[fmIdx] = 1;
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

bool FSC_MainWindow::parseMsg_readGAIN_CONTROL(int fmIdx)
{
    uint16_t    crc     = 0;
    uint16_t    crcRev  = 0;
    uint8_t     len     = 0;

    if (fmRevMsg[fmIdx].size() < 7)
    {
        return false;
    }

    len = static_cast<uchar>(fmSendMsg[fmIdx].data()[2]);

    if (static_cast<uchar>(fmSendMsg[fmIdx].data()[3]) == PROTOCOL_XUNYIN_MODBUS_ADDR_APP_REQ_DATA)
    {
        len = 7;

        crc = Checksum_computeChecksum( fmRevMsg[fmIdx].data(), len - 2);
        memcpy(&crcRev, &(fmRevMsg[fmIdx].data()[len - 2]), 2);
        if ( crcRev != crc    )
        {
            return false;
        }

        fmData[fmIdx].fm_valueGAIN_CONTROL = fmRevMsg[fmIdx].data()[4];

        fm_valu_read_valid[fmIdx] = 1;
    }

    return true;
}

bool FSC_MainWindow::parseMsg_writeMUTIL(int fmIdx)
{
    if (fmRevMsg[fmIdx].size() < 8)
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
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

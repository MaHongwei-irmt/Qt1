#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::reqFMData(int indexFM)
{
    if (indexFM < SOCKET_STD_FLOWM_INDEX || indexFM > SOCKET_FLOWM12_INDEX)
    {
        return;
    }

    char     station = 0;
    bool    sendToNeed = false;

    if (indexFM == SOCKET_STD_FLOWM_INDEX)
    {
        station = stationSTDFM;
    }
    else
    {
        station = stationFM[indexFM - SOCKET_FLOWM1_INDEX];
    }

    if (sktRespondOk[indexFM])
    {
        sendToNeed = true;
    }

    if ((sktReqTime[indexFM] + FM_REQ_TIMEOUT) < QDateTime::currentDateTime().toTime_t())
    {
        sendToNeed = true;

        station++;
        if (station > 15)
        {
            station = 0;
        }

        if (indexFM == SOCKET_STD_FLOWM_INDEX)
        {
            stationSTDFM = station;
        }
        else
        {
            stationFM[indexFM - SOCKET_FLOWM1_INDEX] = station;
        }
    }

    if (sendToNeed)
    {
        reqFMSumRateMsg(&sktBufSend[indexFM], station);
        flushSendBuf();

        sktRespondOk[indexFM] = false;
        sktReqTime[indexFM] = QDateTime::currentDateTime().toTime_t();
    }

}

//ststcon + fun code(read: 0x03) + register address(0x97) + register number(flow sum + systime + up ATOF + down ATOF + DTOF + 1~50 flow rate) +crc
void FSC_MainWindow::reqFMSumRateMsg(QByteArray *buf, int station)
{
    uint16_t crc = 0;

    buf->resize(8);

    (*buf)[0] = static_cast<char>(station);
    (*buf)[1] = 0x3;
    (*buf)[2] = 0;
    (*buf)[3] = static_cast<char>(0x97);
    (*buf)[4] = 0;
    (*buf)[5] = (5 + 50) * 2 ;

    crc = Checksum_computeChecksum( buf->data(), buf->size() - 2);

    (*buf)[6]= static_cast<char>(crc) ;
    (*buf)[7]= static_cast<char>(crc >> 8);

}

bool FSC_MainWindow::preParseFMMsg(int indexSkt)
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

        if ((sktBufRev[indexSkt][0] != stationSTDFM )  || (sktBufRev[indexSkt].size() > 255))
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

        if ((sktBufRev[indexSkt][0] != stationFM[indexSkt - SOCKET_FLOWM1_INDEX] )  || (sktBufRev[indexSkt].size() > 255))
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

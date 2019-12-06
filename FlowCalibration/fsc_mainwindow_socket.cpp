#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::SocketInit(void)
{
    sktConMapper = new QSignalMapper();
    sktDisconMapper = new QSignalMapper();
    sktErrMapper = new QSignalMapper();
    sktReadMapper = new QSignalMapper();

    for( int i = 0; i < SOCKET_NUMBER; i++)
    {
        fsc_global::sktTcp[i] = new QTcpSocket();

        connect(fsc_global::sktTcp[i], SIGNAL(connected()), sktConMapper, SLOT(map()));
        sktConMapper->setMapping(fsc_global::sktTcp[i], i);

        connect(fsc_global::sktTcp[i], SIGNAL(disconnected()), sktDisconMapper, SLOT(map()));
        sktDisconMapper->setMapping(fsc_global::sktTcp[i], i);

        connect(fsc_global::sktTcp[i], SIGNAL(error(QAbstractSocket::SocketError)), sktErrMapper, SLOT(map()));
        sktErrMapper->setMapping(fsc_global::sktTcp[i], i);

        connect(fsc_global::sktTcp[i], SIGNAL(readyRead()), sktReadMapper, SLOT(map()));
        sktReadMapper->setMapping(fsc_global::sktTcp[i], i);
    }

    connect(sktConMapper, SIGNAL(mapped(int)), this, SLOT(skt_connect_suc(int)));
    connect(sktDisconMapper, SIGNAL(mapped(int)), this, SLOT(skt_connect_dis(int)));
    connect(sktErrMapper, SIGNAL(mapped(int)), this, SLOT(skt_error(int)));
    connect(sktReadMapper, SIGNAL(mapped(int)), this, SLOT(skt_read(int)));

    for (int i = 0; i < SOCKET_NUMBER; i++)
    {
        if ( i == SOCKET_PLC_INDEX || i == SOCKET_SCALE_INDEX )
        {

            FSCLOG << "SocketInit->connectToHost...";

            fsc_global::sktTcp[i]->connectToHost(QHostAddress(fsc_global::ip[i]), fsc_global::port_number[i]);

            FSCLOG << "SocketInit->connectToHost";

            sktConCommandTime[i] = QDateTime::currentDateTime().toTime_t();
        }
    }
}

void FSC_MainWindow::skt_connect_suc(int i)
{
    sktConed[i] = true;
    FSCLOG << QString::number(i) + " socket con";

    if ( i == SOCKET_PLC_INDEX)
    {
        return;
    }

    for (int i = SOCKET_SCALE_INDEX; i < SOCKET_NUMBER; i++)
    {
        if (! sktConed[i] &&  (sktConCommandTime[i] + SOCKET_TCP_RETRY_CON_TIMEOUT) < QDateTime::currentDateTime().toTime_t() )
        {

            fsc_global::sktTcp[i]->abort();

            fsc_global::sktTcp[i]->connectToHost(QHostAddress(fsc_global::ip[i]), fsc_global::port_number[i]);
            sktConCommandTime[i] = QDateTime::currentDateTime().toTime_t();


            FSCLOG << QString::number(i) << " socket con retry";
        }

        if (sktDataState[i] != DATA_WRITE_OK && (sktDataWriteTime[i] + DATA_READ_TIMEOUT) < QDateTime::currentDateTime().toTime_t() )
        {
            sktDataState[i] = DATA_TIMEOUT;
            //sktDataWriteTime[i] = QDateTime::currentDateTime().toTime_t();
        }

        if (sktConCommandTime[i] == 0)
        {
            fsc_global::sktTcp[i]->connectToHost(QHostAddress(fsc_global::ip[i]), fsc_global::port_number[i]);
            sktConCommandTime[i] = QDateTime::currentDateTime().toTime_t();
        }
    }
}

void FSC_MainWindow::skt_connect_dis(int i)
{
    fsc_global::sktTcp[i]->abort();
    sktConed[i] = false;

    if ( (sktConCommandTime[i] + SOCKET_TCP_RETRY_CON_TIMEOUT) > QDateTime::currentDateTime().toTime_t() )
    {
        return;
    }

    fsc_global::sktTcp[i]->connectToHost(QHostAddress(fsc_global::ip[i]), fsc_global::port_number[i]);
    sktConCommandTime[i] = QDateTime::currentDateTime().toTime_t();

    FSCLOG << QString::number(i) << " socket discon - recon";

    switch (i)
    {
    case SOCKET_SCALE_INDEX:

        showScaleSum = static_cast<double>(nanf(""));
        showScaleFlow = static_cast<double>(nanf(""));

        break;

    case SOCKET_STD_FLOWM_INDEX:

        showSTDFMSum = static_cast<double>(nanf(""));
        showSTDFMFlow = static_cast<double>(nanf(""));

        break;
    }

    if (i >= SOCKET_FLOWM1_INDEX)
    {
        showFMSum[i - SOCKET_FLOWM1_INDEX] = static_cast<double>(nanf(""));
        showFMFlow[i - SOCKET_FLOWM1_INDEX] = static_cast<double>(nanf(""));
    }
}

void FSC_MainWindow::skt_error(int i)
{
    FSCLOG << "info Socket: " + QString::number(i) + " " + fsc_global::sktTcp[i]->QAbstractSocket::peerAddress().toString()\
              + "  " + fsc_global::sktTcp[i]->errorString();
}


void FSC_MainWindow::skt_read(int i)
{
    sktBufRev[i].append(fsc_global::sktTcp[i]->readAll());

    if (debugSkt[i])
    {
        FSCLOG << "Socket read hex: " + QString::number(i) + " " + QString::number(sktBufRev[i].size()) + " " + ByteArrayToHexString(sktBufRev[i]);

        ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" +\
                                      QDateTime::currentDateTime().toString("hh:mm:ss:zzz<-") + ByteArrayToHexString(sktBufRev[i]));
        ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
    }

    switch (i)
    {
    case SOCKET_SCALE_INDEX:

        //FSCLOG << "Socket read string: " + QString::number(i) + " " + QString::number(sktBufRev[i].size()) + " " + sktBufRev[i];

        showScaleFlow = sktBufRev[i].toDouble() - showScaleSum;
        showScaleSum = sktBufRev[i].toDouble();

        sktBufRev[i].resize(0);

        sktDataState[SOCKET_SCALE_INDEX] = DATA_READ_OK;

        break;


    case SOCKET_FLOWM9_INDEX:
        showFMSum[8] = 0;
        showFMFlow[8] = 0;

        if (fsc_global::sktTcp[SOCKET_FLOWM9_INDEX])
        {
            static double scale_test = 1.1;
            static int j = 1;

            if (scaleTestZero)
            {
                scale_test = 0;
                j = 0;

                scaleTestZero = 0;
            }

            scale_test += 0.1 * j++;

            sktBufSend[SOCKET_FLOWM9_INDEX] = QByteArray::number(scale_test, 'f', 2 );

            flushSendBuf();
        }

        break;

    case SOCKET_FLOWM10_INDEX:
    case SOCKET_FLOWM11_INDEX:

         QByteArray * baSend = &sktBufSend[i];

        {
            baSend->resize(3 + 220 +2);


            (*baSend)[0] = 4;
            (*baSend)[1] = 3;
            (*baSend)[2] = static_cast<char>(220);

            QByteArray ba;
            static float f = static_cast<float>(111.111);

            ba.resize(4);


            for (int i = 0; i < 55; i++)
            {
                memcpy(ba.data(), &f, sizeof (f));

                (*baSend)[3 + i * 4] = ba[1];
                (*baSend)[4 + i * 4] = ba[0];
                (*baSend)[5 + i * 4] = ba[3];
                (*baSend)[6 + i * 4] = ba[2];

                f +=  static_cast<float>(0.01);

            }



            uint16_t crc = 0;

            crc = Checksum_computeChecksum( baSend->data(), 3 + 220);

            (*baSend)[223] = static_cast<char>(crc) ;
            (*baSend)[224] = static_cast<char>(crc >> 8);

            flushSendBuf();

        }

            break;



    }

    parsePLC(i);
    preParseFMMsg(i);
    parseFMSumRateMsg(i);

}

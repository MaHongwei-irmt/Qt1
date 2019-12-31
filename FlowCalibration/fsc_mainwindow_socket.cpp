#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::SocketConnectTry(void)
{

    FSCLOG << "SocketInit->connectToHostTry1..." << fsc_global::ip[0] << fsc_global::port_number[0];

    QTcpSocket socket;
    socket.connectToHost(QHostAddress(fsc_global::ip[0]), fsc_global::port_number[0]);
    if (socket.waitForConnected(3000))
    {
        socket.disconnect();
        socket.waitForDisconnected(200);

    }
    FSCLOG << "SocketInit->connectToHostTry1" << fsc_global::ip[0] << fsc_global::port_number[0];


    FSCLOG << "SocketInit->connectToHostTry2..." << fsc_global::ip[1] << fsc_global::port_number[1];

    socket.connectToHost(QHostAddress(fsc_global::ip[1]), fsc_global::port_number[1]);
    if (socket.waitForConnected(3000))
    {
        socket.disconnect();
        socket.waitForDisconnected(300);
    }

    FSCLOG << "SocketInit->connectToHostTry2" << fsc_global::ip[1] << fsc_global::port_number[1];

}

void FSC_MainWindow::startSocketConnect(int i)
{
    (void)i;

    delayMSec(200);
    SocketConnect();
    socketWellDone = true;
}

void FSC_MainWindow::SocketConnect(void)
{

    for (int i = 0; i < SOCKET_NUMBER; i++)
    {
        if ( i == SOCKET_PLC_INDEX || i == SOCKET_SCALE_INDEX )
        {

            FSCLOG << "SocketInit->connectToHost..." << fsc_global::ip[i] << fsc_global::port_number[i];

            fsc_global::sktTcp[i]->connectToHost(QHostAddress(fsc_global::ip[i]), fsc_global::port_number[i]);

            FSCLOG << "SocketInit->connectToHost";

            sktConCommandTime[i] = QDateTime::currentDateTime().toTime_t();
        }
    }
}

void FSC_MainWindow::skt_connect_suc(int i)
{
    if (i >= SOCKET_NUMBER)
    {
        return;
    }

    sktConed[i] = true;
    FSCLOG << QString::number(i) + " socket con";

    sktReqTime[i] = QDateTime::currentDateTime().toTime_t();

}

void FSC_MainWindow::skt_connect_dis(int i)
{
    if (i >= SOCKET_NUMBER)
    {
        return;
    }

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
    if (i >= SOCKET_NUMBER)
    {
        return;
    }

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

        //break;

    case SOCKET_FLOWM11_INDEX:

         QByteArray * baSend = &sktBufSend[i];

        {
            baSend->resize(3 + 220 +2);

            (*baSend)[0] = sktBufRev[i][0];
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

    parsePLCNoSTFM(i);

    if (i >= SOCKET_FLOWM1_INDEX && i <= SOCKET_FLOWM12_INDEX)
    {
        if (preParseFMMsg(i - SOCKET_FLOWM1_INDEX))
        {
            parseFMMsg(i - SOCKET_FLOWM1_INDEX);
        }
        else if(preParseStfmMsg())
        {
            parseStfmMsg();
        }
        else
        {
            if(preParseFMSumRateMsg(i))
            {
                parseFMSumRateMsg(i);
            }
        }
    }
    else if(preParseFMSumRateMsg(i))
    {
        parseFMSumRateMsg(i);
    }
}

void FSC_MainWindow::flushSendBuf(void)
{
    for (int i = 0; i < SOCKET_NUMBER; i++)
    {

        if (sktConed[i] && sktBufSend[i].size() > 0)
        {


            if (debugSkt[i])
            {
                FSCLOG << "Socket write hex: skt-" + QString::number(i) + " " + QString::number(sktBufSend[i].size()) + " " + ByteArrayToHexString(sktBufSend[i]);

                ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" +\
                                              QDateTime::currentDateTime().toString("hh:mm:ss:zzz->")+ ByteArrayToHexString(sktBufSend[i]));
                ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
            }


            fsc_global::sktTcp[i]->write(sktBufSend[i]);
            fsc_global::sktTcp[i]->flush();
            fsc_global::sktTcp[i]->waitForBytesWritten();

            sktBufSend[i].resize(0);
        }
    }
}

void FSC_MainWindow::socketCommunication(void)
{
    if (!socketWellDone)
    {
        FSCLOG << "wait socket start up ...";
        return;
    }

    for (int i = 0; i <= SOCKET_SCALE_INDEX; i++)
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

    }

    if (sktConed[SOCKET_SCALE_INDEX])
    {
        for (int i = SOCKET_SCALE_INDEX + 1; i < SOCKET_NUMBER; i++)
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

        }

    }
}

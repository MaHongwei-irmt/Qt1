#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

#include <QFile>
#include <QtCore/QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <math.h>

QVector <fsc_para_ini>  fsc_global::para_ini;
QTcpSocket*             fsc_global::sktTcp[SOCKET_NUMBER];
QString                 fsc_global::ip_PLC;
QString                 fsc_global::ip_RS_Server;
quint16                 fsc_global::port_number[SOCKET_NUMBER];
QString                 fsc_global::ip[SOCKET_NUMBER];

FSC_MainWindow::FSC_MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::FSC_MainWindow),
    mainLoopTimer(new QTimer(this))
{
    ui->setupUi(this);

    uiReInit();
    ParaInit();
    PlotInit();
    DataInit();
    SocketInit();
    plcDataInit();

    connect(mainLoopTimer, SIGNAL(timeout()), this, SLOT(mainLoop()));
    mainLoopTimer->start(100);
}

FSC_MainWindow::~FSC_MainWindow()
{
    delete ui;
}

void FSC_MainWindow::uiReInit(void)
{
    int x = 97;
    int y = 153;

    for (int i = 0; i < SPAN_NUMBER; i++)
    {

        checkBox_spanCal[i] =  new QCheckBox(this);
        checkBox_spanCal[i]->move(x, y + (SPAN_NUMBER - i - 1) * 19);
        checkBox_spanCal[i]->show();

        checkBox_spanCheck[i] =  new QCheckBox(this);
        checkBox_spanCheck[i]->move(x + 20, y + (SPAN_NUMBER - i - 1) * 19);
        checkBox_spanCheck[i]->show();

        checkBox_spanCorrect[i] =  new QCheckBox(this);
        checkBox_spanCorrect[i]->move(x + 40, y + (SPAN_NUMBER - i - 1) * 19);
        checkBox_spanCorrect[i]->show();


        checkBox_spanCalReverse[i] =  new QCheckBox(this);
        checkBox_spanCalReverse[i]->move(x + 80, y + (SPAN_NUMBER - i - 1) * 19);
        checkBox_spanCalReverse[i]->show();

        checkBox_spanCheckReverse[i] =  new QCheckBox(this);
        checkBox_spanCheckReverse[i]->move(x + 100, y + (SPAN_NUMBER - i - 1) * 19);
        checkBox_spanCheckReverse[i]->show();

        checkBox_spanCorrectReverse[i] =  new QCheckBox(this);
        checkBox_spanCorrectReverse[i]->move(x + 120, y + (SPAN_NUMBER - i - 1) * 19);
        checkBox_spanCorrectReverse[i]->show();
     }


    lineEdit_FMSum[0] = ui->lineEdit_FM_1;
    lineEdit_FMSum[1] = ui->lineEdit_FM_2;
    lineEdit_FMSum[2] = ui->lineEdit_FM_3;
    lineEdit_FMSum[3] = ui->lineEdit_FM_4;
    lineEdit_FMSum[4] = ui->lineEdit_FM_5;
    lineEdit_FMSum[5] = ui->lineEdit_FM_6;
    lineEdit_FMSum[6] = ui->lineEdit_FM_7;
    lineEdit_FMSum[7] = ui->lineEdit_FM_8;
    lineEdit_FMSum[8] = ui->lineEdit_FM_9;
    lineEdit_FMSum[9] = ui->lineEdit_FM_10;
    lineEdit_FMSum[10] = ui->lineEdit_FM_11;
    lineEdit_FMSum[11] = ui->lineEdit_FM_12;

    lineEdit_FMFlow[0] = ui->lineEdit_FM_1_flow;
    lineEdit_FMFlow[1] = ui->lineEdit_FM_2_flow;
    lineEdit_FMFlow[2] = ui->lineEdit_FM_3_flow;
    lineEdit_FMFlow[3] = ui->lineEdit_FM_4_flow;
    lineEdit_FMFlow[4] = ui->lineEdit_FM_5_flow;
    lineEdit_FMFlow[5] = ui->lineEdit_FM_6_flow;
    lineEdit_FMFlow[6] = ui->lineEdit_FM_7_flow;
    lineEdit_FMFlow[7] = ui->lineEdit_FM_8_flow;
    lineEdit_FMFlow[8] = ui->lineEdit_FM_9_flow;
    lineEdit_FMFlow[9] = ui->lineEdit_FM_10_flow;
    lineEdit_FMFlow[10] = ui->lineEdit_FM_11_flow;
    lineEdit_FMFlow[11] = ui->lineEdit_FM_12_flow;

    buttonDebugMapper = new QSignalMapper();
    for (int i = 0; i < SOCKET_NUMBER; i++)
    {
        sktBufRev[i].resize(0);
        sktBufSend[i].resize(0);

        sktDataState[i] = DATA_READ_OK;

        debugSkt[i] = false;

        buttonDebug[i] = new QPushButton(this);
        buttonDebug[i]->setText(QString::number(i));
        buttonDebug[i]->setFixedSize(75,23);
        buttonDebug[i]->move(1660, 80 + i * 29);
        buttonDebug[i]->show();

        connect(buttonDebug[i], SIGNAL(clicked()), buttonDebugMapper, SLOT(map()));
        buttonDebugMapper->setMapping(buttonDebug[i], i);
    }
    connect(buttonDebugMapper, SIGNAL(mapped(int)), this, SLOT(buttonDebug_clicked(int)));

}

void FSC_MainWindow::ParaInit(void)
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    fsc_global::ip_PLC = configIni->value("IP_ADDRESS/PLC" ).toString();
    fsc_global::ip_RS_Server = configIni->value("IP_ADDRESS/RS_SERVER" ).toString();

    fsc_global::ip[0] = fsc_global::ip_PLC;
    for (int i = 1; i < SOCKET_NUMBER; i++)
    {
        fsc_global::ip[i] = fsc_global::ip_RS_Server;
    }

    fsc_global::port_number[0] = 2100;
    for(quint16 i = 1; i < SOCKET_NUMBER; i++)
    {
        fsc_global::port_number[i] = 4000 + i;
    }

    FSCLOG << fsc_global::ip_PLC;
    FSCLOG << fsc_global::ip_RS_Server;


    int i = 1;
    fsc_para_ini tmp;

    ui->comboBox_SensorTypeName->clear();
    fsc_global::para_ini.clear();
    fsc_global::para_ini.swap(fsc_global::para_ini);
    ui->leFlowSpeed_SensorSpan->setText("");
    ui->leFMTypeName->setText("");

    while (configIni->value( "sensor_type_" + QString::number(i) +"/type_name" ).toString() != "")
    {

        tmp.type_name = configIni->value( "sensor_type_" + QString::number(i) +"/type_name" ).toString();
        FSCLOG << tmp.type_name;

        tmp.span_ml_per_min = configIni->value( "sensor_type_" + QString::number(i) +"/span_ml_per_min" ).toDouble();

        QString str;
        for (int k = 0; k < SPAN_NUMBER; k++)
        {
            str = QString().sprintf("/%d_percent_span_to_be_calibrate", (k + 1) * 10 );
            tmp.spanCal[k] = configIni->value( "sensor_type_" + QString::number(i) + str ).toBool();

            str = QString().sprintf("/%d_percent_span_to_be_verify", (k + 1) * 10 );
            tmp.spanCheck[k] = configIni->value( "sensor_type_" + QString::number(i) + str ).toBool();

            str = QString().sprintf("/%d_percent_span_to_be_correct", (k + 1) * 10 );
            tmp.spanCorrect[k] = configIni->value( "sensor_type_" + QString::number(i) + str ).toBool();


            str = QString().sprintf("/%d_percent_span_to_be_calibrate_reverse", (k + 1) * 10 );
            tmp.spanCalReverse[k] = configIni->value( "sensor_type_" + QString::number(i) + str ).toBool();

            str = QString().sprintf("/%d_percent_span_to_be_verify_reverse", (k + 1) * 10 );
            tmp.spanCheckReverse[k] = configIni->value( "sensor_type_" + QString::number(i) + str ).toBool();

            str = QString().sprintf("/%d_percent_span_to_be_correct_reverse", (k + 1) * 10 );
            tmp.spanCorrectReverse[k] = configIni->value( "sensor_type_" + QString::number(i) + str ).toBool();
        }

        fsc_global::para_ini.append(tmp);

        ui->comboBox_SensorTypeName->addItem(tmp.type_name);

        i++;
    }

    if(fsc_global::para_ini.size() > 0)
    {
        on_comboBox_SensorTypeName_currentIndexChanged(0);
    }

    delete configIni;

}

void FSC_MainWindow::PlotInit(void)
{
    ui->MyCustomPlot->legend->setVisible(true);
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    ui->MyCustomPlot->addGraph();
    ui->MyCustomPlot->graph(0)->setPen(QPen(Qt::red));
    ui->MyCustomPlot->graph(0)->setName("天平");


    ui->MyCustomPlot->addGraph();
    ui->MyCustomPlot->graph(1)->setPen(QPen(Qt::blue));
    ui->MyCustomPlot->graph(1)->setName("标准流量");
    ui->MyCustomPlot->addGraph();
    ui->MyCustomPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->MyCustomPlot->graph(2)->setName("标准流速");

    ui->MyCustomPlot->addGraph();
    ui->MyCustomPlot->graph(3)->setPen(QPen(Qt::black));
    ui->MyCustomPlot->graph(3)->setName("流量");
    ui->MyCustomPlot->addGraph();
    ui->MyCustomPlot->graph(4)->setPen(QPen(Qt::black));
    ui->MyCustomPlot->graph(4)->setName("流速");

    ui->MyCustomPlot->axisRect()->setupFullAxesBox();

    ui->MyCustomPlot->xAxis->setLabel(tr("采样点时序"));
    ui->MyCustomPlot->yAxis->setLabel(tr("天平质量/流量/流速"));
    ui->MyCustomPlot->rescaleAxes(true);
    ui->MyCustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    ui->MyCustomPlot->replot();
}


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
        sktConed[i] = false;
        fsc_global::sktTcp[i]->connectToHost(QHostAddress(fsc_global::ip[i]), fsc_global::port_number[i]);
        sktConCommandTime[i] = QDateTime::currentDateTime().toTime_t();
    }
}

void FSC_MainWindow::DataInit(void)
{
    showScaleSum = static_cast<double>(nanf(""));
    showScaleFlow = static_cast<double>(nanf(""));
    showSTDFMSum = static_cast<double>(nanf(""));
    showSTDFMFlow = static_cast<double>(nanf(""));

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        showFMSum[i] = static_cast<double>(nanf(""));
        showFMFlow[i] = static_cast<double>(nanf(""));

        stationFM[i] = FM_STATION_ADDRESS;
     }
    stationSTDFM = FM_STATION_ADDRESS;

    ui->radioButton_setFlowRate->setChecked(true);

    QDoubleValidator *pDfValidator = new QDoubleValidator(1, 900000.0, 0, ui->lineEdit_setFlowRate);
    pDfValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_setFlowRate->setValidator(pDfValidator);
    ui->lineEdit_setFlowRate->setMaxLength(7);

    ui->lineEdit_setPWM->setValidator(new QIntValidator(1, 100, this));
    ui->lineEdit_setPWM->setMaxLength(2);

    showSetFlowRate = 1000;
    showSetPWM = 80;

    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));

    revdSketPLC = false;

    plotLoop = 0;
    calOn = CAL_STATE_STOP;

    ui->textBrow_calInfo->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 程序启动"));

    dataInit_calStepInit();
    calStepInfoFresh();
}

void FSC_MainWindow::dataInit_calStepInit(void)
{
    currentStep.span_ml_per_min = ui->leFlowSpeed_SensorSpan->text().toDouble();

    if (currentStep.span_ml_per_min < 1)
    {
        QString str;

        str = ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:");

        str += " 型号：";
        str += ui->comboBox_SensorTypeName->currentText();
        str += "  ";
        str += "量程：";
        str += ui->leFlowSpeed_SensorSpan->text();
        str += " |-> |";

        return;
    }

    for(int i = 0; i < SPAN_NUMBER; i++)
    {
        currentStep.spanCal[i] = 0;
        currentStep.spanCheck[i] = 0;
        currentStep.spanCorrect[i] = 0;

        currentStep.spanCalReverse[i] = 0;
        currentStep.spanCheckReverse[i] = 0;
        currentStep.spanCorrectReverse[i] = 0;
    }

    for (int i = 0; i < SPAN_NUMBER; i++)
    {
        if (checkBox_spanCal[i]->checkState())
        {
            currentStep.spanCal[i] =  CAL_CURRENT_STAT_NEED_EXECUTE;
        }
        if (checkBox_spanCorrect[i]->checkState())
        {
            currentStep.spanCorrect[i] =  CAL_CURRENT_STAT_NEED_EXECUTE;
        }
        if (checkBox_spanCheck[i]->checkState())
        {
            currentStep.spanCheck[i] =  CAL_CURRENT_STAT_NEED_EXECUTE;
        }

        if (checkBox_spanCalReverse[i]->checkState())
        {
            currentStep.spanCalReverse[i] =  CAL_CURRENT_STAT_NEED_EXECUTE;
        }
        if (checkBox_spanCorrectReverse[i]->checkState())
        {
            currentStep.spanCorrectReverse[i] =  CAL_CURRENT_STAT_NEED_EXECUTE;
        }
        if (checkBox_spanCheckReverse[i]->checkState())
        {
            currentStep.spanCheckReverse[i] =  CAL_CURRENT_STAT_NEED_EXECUTE;
        }
    }

    currentStep.stepCurrent = 0;
}

void FSC_MainWindow::calStepInfoFresh(void)
{
    QString str;

    str = ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:");

    str += " 型号：";
    str += ui->comboBox_SensorTypeName->currentText();
    str += "  ";
    str += "量程：";
    str += ui->leFlowSpeed_SensorSpan->text();
    str += " |-> ";

    currentStep.stepTotal = 0;


    for (int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCal[i])
        {
            currentStep.stepTotal++;

            if (currentStep.stepTotal > currentStep.stepCurrent)
            {
                str += "(" + QString::number(currentStep.stepTotal) + ")";
                str += QString::number((i + 1) * 10) + "%量程标定-> ";
            }
        }
        if (currentStep.spanCorrect[i])
        {
            currentStep.stepTotal++;

            if (currentStep.stepTotal > currentStep.stepCurrent)
            {
                str += "(" + QString::number(currentStep.stepTotal) + ")";
                str += QString::number((i + 1) * 10) + "%量程修正-> ";
            }
        }
        if (currentStep.spanCheck[i])
        {
            currentStep.stepTotal++;

            if (currentStep.stepTotal > currentStep.stepCurrent)
            {

                str += "(" + QString::number(currentStep.stepTotal) + ")";
                str += QString::number((i + 1) * 10) + "%量程验证-> ";
            }

        }
    }

    for (int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCalReverse[i])
        {
            currentStep.stepTotal++;

            if (currentStep.stepTotal > currentStep.stepCurrent)
            {
                str += "(" + QString::number(currentStep.stepTotal) + ")";
                str += "反向" + QString::number((i + 1) * 10) + "%量程标定-> ";
            }
        }
        if (currentStep.spanCorrectReverse[i])
        {
            currentStep.stepTotal++;

            if (currentStep.stepTotal > currentStep.stepCurrent)
            {
                str += "(" + QString::number(currentStep.stepTotal) + ")";
                str += "反向" + QString::number((i + 1) * 10) + "%量程修正-> ";
            }
        }
        if (currentStep.spanCheckReverse[i])
        {
            currentStep.stepTotal++;

            if (currentStep.stepTotal > currentStep.stepCurrent)
            {
                str += "(" + QString::number(currentStep.stepTotal) + ")";
                str += "反向" + QString::number((i + 1) * 10) + "%量程验证-> ";
            }
        }
    }


    str += "|\r\n";
    ui->textBrow_calInfo->setText(str);

    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

}

void FSC_MainWindow::mainLoop()
{
    QString str;

    static int j = 0;

    j++;


    for (int i = 0; i < SOCKET_NUMBER; i++)
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


    if (j % 5 == 0)
    {
        if (sktDataState[SOCKET_SCALE_INDEX] == DATA_READ_OK || \
                (sktDataWriteTime[SOCKET_SCALE_INDEX] + DATA_READ_TIMEOUT) < QDateTime::currentDateTime().toTime_t() )
        {
            reqScaleShow();

            sktDataState[SOCKET_SCALE_INDEX] = DATA_WRITE_OK;
            sktDataWriteTime[SOCKET_SCALE_INDEX] = QDateTime::currentDateTime().toTime_t();
        }
    }

    if (j % 5 == 1)
    {
        reqSetPLC(showSetFlowRate, showSetPWM, 1, 2);
    }

    if (j % 5 == 2)
    {
        reqFMData(SOCKET_FLOWM1_INDEX);
        reqFMData(SOCKET_FLOWM2_INDEX);
        reqFMData(SOCKET_FLOWM3_INDEX);
        reqFMData(SOCKET_FLOWM4_INDEX);
        reqFMData(SOCKET_FLOWM5_INDEX);
        reqFMData(SOCKET_FLOWM6_INDEX);
    }

    if (j % 5 == 4)
    {
//        for (int i = SOCKET_FLOWM1_INDEX; i <= SOCKET_NUMBER; i++)
//        {
//            reqFMData(i);
//        }


        reqFMData(SOCKET_FLOWM7_INDEX);
        reqFMData(SOCKET_FLOWM8_INDEX);
        //reqFMData(SOCKET_FLOWM9_INDEX);
        //reqFMData(SOCKET_FLOWM10_INDEX);
        //reqFMData(SOCKET_FLOWM11_INDEX);
        reqFMData(SOCKET_FLOWM12_INDEX);

    }

    if (j % 5 == 0)
    {
        plotAddDataAndFresh();

        if (ui->textBrow_calInfo->toPlainText().size() > CAL_INFO_MAX_STR_NUM)
        {
            ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText().right(CAL_INFO_MAX_STR_NUM / 2)) ;
        }
    }



    //flushSendBuf();


    if (showSetFlowRate >= 123455 && showSetFlowRate <= 123457 &&  showSetPWM == 65)
    {
        setFixedSize(1780, 880);
        setMinimumSize(0, 0);
        setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

        showSetFlowRate = 1000;
        showSetPWM = 80;

        ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
        ui->lineEdit_setPWM->setText(QString::number(showSetPWM));
    }

    showFresh();

    switch (calOn)
    {
    case CAL_START:

        {
            on_tbnPoltClear_clicked();

            str = ui->textBrow_calInfo->toPlainText();
            str += "\r\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
            str += "启动标定->";
            str += "清空曲线图->";
            str += "关闭所有阀门->";
            str += "稳定3秒...\r\n";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_START_BALANCE;
            calOnTime = QDateTime::currentDateTime().toTime_t();
        }

        break;

    case CAL_START_BALANCE:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 2)
        {

            on_tbnScaleZero_clicked();

            str = ui->textBrow_calInfo->toPlainText();
            str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
            str += "天平清零->";
            str += "稳定3秒...\r\n";

            ui->textBrow_calInfo->setText(str);


            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_SCALE_ZERO;
            calOnTime = QDateTime::currentDateTime().toTime_t();
        }

        break;

    case CAL_SCALE_ZERO:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 2)
        {

            calOn = CAL_SCALE_ZERO_BALANCE;
            calOnTime = QDateTime::currentDateTime().toTime_t();

        }

        break;

    case CAL_SCALE_ZERO_BALANCE:

        {
            startCal_dir_type_span(&currentStep.startDirect, &currentStep.startType, \
                                   &currentStep.startSpanPercent, &currentStep.startSpan);

            str = ui->textBrow_calInfo->toPlainText();
            str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");

            str += QString().sprintf("(%d)->", currentStep.stepCurrent + 1);

            if (currentStep.startDirect == START_CAL_DIRECT_FORWARD)
            {
                str += "打开正向进水阀->";
            }
            else if (currentStep.startDirect == START_CAL_DIRECT_REVERSE)
            {
                str += "打开反向进水阀->";
            }

            if (currentStep.startType == START_CAL_TYPE_CAL)
            {
                str += "标定->";
            }
            else if (currentStep.startType == START_CAL_TYPE_CORRECT)
            {
                str += "修正->";
            }
            else if (currentStep.startType == START_CAL_TYPE_CHECK)
            {
                 str += "验证->";
            }

            str += QString().sprintf("%d", currentStep.startSpanPercent);
            str += "%量程->";
            str += QString().sprintf("%0.3fml/min->", currentStep.startSpan);

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_OPEN_VALVE;
            calOnTime = QDateTime::currentDateTime().toTime_t();

            break;

    }

    case CAL_OPEN_VALVE:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 1)
        {

            str = ui->textBrow_calInfo->toPlainText();
            str += "启动水泵->";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_OPEN_PUMP;
            calOnTime = QDateTime::currentDateTime().toTime_t();

        }

        break;


    case CAL_OPEN_PUMP:

        {
            str = ui->textBrow_calInfo->toPlainText();
            str += "开始绘图...\r\n";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_PLOT_START;
            calOnTime = QDateTime::currentDateTime().toTime_t();

            PlotReplay(ui->comboBox_PlotSenSel->currentText());

        }

        break;

    }

}

int FSC_MainWindow::startCal_dir_type_span(int *dir, int *type, int *spanPercent, double *span)
{
    int step = 0;

    for(int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCal[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (currentStep.stepCurrent == step)
            {
               *dir     =   START_CAL_DIRECT_FORWARD;
               *type    =   START_CAL_TYPE_CAL;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;

         }

        if (currentStep.spanCorrect[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (currentStep.stepCurrent == step)
            {
               *dir     =   START_CAL_DIRECT_FORWARD;
               *type    =   START_CAL_TYPE_CORRECT;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }

        if (currentStep.spanCheck[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (currentStep.stepCurrent == step)
            {
               *dir     =   START_CAL_DIRECT_FORWARD;
               *type    =   START_CAL_TYPE_CHECK;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }
    }

    for(int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCalReverse[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (currentStep.stepCurrent  == step)
            {
               *dir     =   START_CAL_DIRECT_REVERSE;
               *type    =   START_CAL_TYPE_CAL;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }

        if (currentStep.spanCorrect[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (currentStep.stepCurrent == step)
            {
               *dir     =   START_CAL_DIRECT_REVERSE;
               *type    =   START_CAL_TYPE_CORRECT;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }

        if (currentStep.spanCheck[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (currentStep.stepCurrent == step)
            {
               *dir     =   START_CAL_DIRECT_REVERSE;
               *type    =   START_CAL_TYPE_CHECK;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }

    }

    return false;
}

void FSC_MainWindow::plotAddDataAndFresh(void)
{
    //static uint poltNew = QDateTime::currentDateTime().toTime_t();
    //uint16_t newMS;

    if (calOn == CAL_PLOT_START)
    {
//        if (plotLoop == 0)
//        {
//            poltNew = QDateTime::currentDateTime().toTime_t();
//        }

        //NewMS = (QDateTime::currentDateTime().toTime_t() - poltNew) * 1000 + static_cast<uint>( QTime::currentTime().msec());

        //plotScaleSumTimeX.append( NewMS );

        //plotScaleSumTimeX.append( plotLoop * 0.5 );
        //plotScaleSumValueY.append(showFMSum[0]);


        //plotScaleSumValueY.append(showScaleSum);


        //FSCLOG << NewMS  << "   "  << showFMSum[0];

        //FSCLOG << plotLoop * 0.5  << "   "  << showFMSum[0];



        plotScaleSumTimeX.append( plotLoop * 0.5 );
        plotSTDFMSumTimeX.append( plotLoop * 0.5 );
        plotSTDFMFlowTimeX.append( plotLoop * 0.5 );
        plotFMSumTimeX.append( plotLoop * 0.5 );
        plotFMFlowTimeX.append( plotLoop * 0.5 );

        plotScaleSumValueY.append(showScaleSum);
        plotSTDFMSumValueY.append(showSTDFMSum);
        plotSTDFMFlowValueY.append(showSTDFMFlow);
        plotFMSumValueY.append(showFMSum[0]);
        plotFMFlowValueY.append(showFMFlow[0]);



        FSCLOG << plotLoop * 0.5  << "   "  << showScaleSum;

        plotFresh();

        if (plotLoop++ > PLOT_VALUE_NUMBER)
        {
            calOn = CAL_STATE_STOP;
            //plotLoop = 0;
            //plotScaleSumTimeX.clear();

        }

    }
    else
    {

        plotScaleSumTimeX.clear();
        plotScaleSumValueY.clear();
    }

}

void FSC_MainWindow::PlotReplay(const QString &arg1)
{
    FSCLOG  << arg1;

    ui->MyCustomPlot->graph(3)->setName(ui->comboBox_PlotSenSel->currentText() + "流量");
    ui->MyCustomPlot->graph(4)->setName(ui->comboBox_PlotSenSel->currentText() + "流速");

    ui->MyCustomPlot->replot();
}

void FSC_MainWindow::plotFresh(void)
{

    ui->MyCustomPlot->legend->setVisible(true);

    //设定右上角图形标注的字体
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    QVector<double> x(0),y(0);

    ui->MyCustomPlot->graph(0)->setData(plotScaleSumTimeX, plotScaleSumValueY);

    ui->MyCustomPlot->graph(1)->setData(plotSTDFMSumTimeX, plotSTDFMSumValueY);
    ui->MyCustomPlot->graph(2)->setData(plotSTDFMFlowTimeX, plotSTDFMFlowValueY);
    ui->MyCustomPlot->graph(3)->setData(plotFMSumTimeX, plotFMSumValueY);
    ui->MyCustomPlot->graph(4)->setData(plotFMFlowTimeX, plotFMFlowValueY);


 //   ui->MyCustomPlot->xAxis->setLabel(tr("采样点时序"));
 //   ui->MyCustomPlot->yAxis->setLabel(tr("天平质量/流量/流速"));
    ui->MyCustomPlot->rescaleAxes(true);
   // ui->MyCustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);



    ui->MyCustomPlot->replot();

    //-------------------------------------------------------------------
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


bool FSC_MainWindow::parsePLC(int indexSkt)
{
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

    showSTDFMSum = 0;
    //showSTDFMFlow = 0;

    ba.resize(4);
    ba[0] = sktBufRev[0][0];
    ba[1] = sktBufRev[0][1];
    ba[2] = sktBufRev[0][2];
    ba[3] = sktBufRev[0][3];
    memcpy(&f, ba.data(), 4);
    showSTDFMSum = static_cast<double>(f);

//    ba[0] = sktBufRev[0][4];
//    ba[1] = sktBufRev[0][5];
//    ba[2] = sktBufRev[0][6];
//    ba[3] = sktBufRev[0][7];
//    memcpy(&f, ba.data(), 4);
//    showSTDFMFlow = static_cast<double>(f);

    showSTDFMFlow = sktBufRev[0][4];

    sktBufRev[0].resize(0);

    return true;

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

        if ( (sktBufRev[indexSkt][0] != stationSTDFM )  || (sktBufRev[indexSkt].size() > 255))
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

        if ( (sktBufRev[indexSkt][0] != stationFM[indexSkt - SOCKET_FLOWM1_INDEX] )  || (sktBufRev[indexSkt].size() > 255))
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

    ba[0] = sktBufRev[indexSkt][1 + 16 + 3];
    ba[1] = sktBufRev[indexSkt][0 + 16 + 3];
    ba[2] = sktBufRev[indexSkt][3 + 16 + 3];
    ba[3] = sktBufRev[indexSkt][2 + 16 + 3];

    memcpy(&f1, ba.data(), 4);


    sktBufRev[indexSkt].resize(0);


    if (indexSkt == SOCKET_STD_FLOWM_INDEX)
    {
        showSTDFMSum = static_cast<double>(f);
        showSTDFMFlow = static_cast<double>(f1);
    }
    else
    {
         showFMSum[indexSkt - SOCKET_FLOWM1_INDEX] = static_cast<double>(f);
         showFMFlow[indexSkt - SOCKET_FLOWM1_INDEX] = static_cast<double>(f1);


    }

    return true;

}


void FSC_MainWindow::reqSetPLC(double flowRate, int PWM, int devOn, int devOff)
{
    float f = QString::number(flowRate, 'f', 0).toFloat();
    QByteArray* baSnd= &sktBufSend[SOCKET_PLC_INDEX];

    sktBufSend[SOCKET_PLC_INDEX].resize(1 + sizeof(float) + 3 *  sizeof(uint16_t));

    sktBufSend[SOCKET_PLC_INDEX][0]=0x01;
    memcpy(&(baSnd->data()[1]), &f, sizeof(float));
    memcpy(&(baSnd->data()[1 + sizeof(float)]), &PWM, sizeof(uint16_t));
    memcpy(&(baSnd->data()[1 + sizeof(float) + sizeof(uint16_t)]), &devOn, sizeof(uint16_t));
    memcpy(&(baSnd->data()[1 + sizeof(float) + 2 * sizeof(uint16_t)]), &devOff, sizeof(uint16_t));

    flushSendBuf();
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


void FSC_MainWindow::reqFMData(int indexFM)
{
    if (indexFM < SOCKET_STD_FLOWM_INDEX)
    {
        return;
    }

    reqFMSumRateMsg(&sktBufSend[indexFM], FM_STATION_ADDRESS);
    flushSendBuf();
}

void FSC_MainWindow::flushSendBuf(void)
{
    for (int i = 0; i < SOCKET_NUMBER; i++)
    {

        if (sktConed[i] && sktBufSend[i].size() > 0)
        {
            fsc_global::sktTcp[i]->write(sktBufSend[i]);
            fsc_global::sktTcp[i]->flush();
            fsc_global::sktTcp[i]->waitForBytesWritten();

            if (debugSkt[i])
            {
                FSCLOG << "Socket write hex: skt-" + QString::number(i) + " " + QString::number(sktBufSend[i].size()) + " " + ByteArrayToHexString(sktBufSend[i]);

                ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n" +\
                                              QDateTime::currentDateTime().toString("hh:mm:ss:zzz->")+ ByteArrayToHexString(sktBufSend[i]));
                ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
            }

            sktBufSend[i].resize(0);
        }
    }
}

void FSC_MainWindow::showFresh(void)
{
    ui->lineEdit_scale_show->setEnabled(sktConed[SOCKET_SCALE_INDEX]);
    ui->lineEdit_scale_flow->setEnabled(sktConed[SOCKET_SCALE_INDEX]);
    ui->lineEdit_scale_show->setText(QString::number(showScaleSum, 'f', 3));
    ui->lineEdit_scale_flow->setText(QString::number(showScaleFlow, 'f', 3));

    ui->lineEdit_stdFM_sum->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    ui->lineEdit_stdFM_flow->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    ui->lineEdit_stdFM_sum->setText(QString::number(showSTDFMSum, 'f', 3));
    ui->lineEdit_stdFM_flow->setText(QString::number(showSTDFMFlow, 'f', 3));

    for (int i = 0; i  < FLOWMETER_NUMBER; i++)
    {
        lineEdit_FMSum[i]->setEnabled(sktConed[i + SOCKET_FLOWM1_INDEX]);
        lineEdit_FMFlow[i]->setEnabled(sktConed[i + SOCKET_FLOWM1_INDEX]);

        lineEdit_FMSum[i]->setText(QString::number(showFMSum[i], 'f', 3));
        lineEdit_FMFlow[i]->setText(QString::number(showFMFlow[i], 'f', 3));
    }

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

    ui->lineEdit_plotTime->setText(QString::number(plotLoop * 0.5, 'f', 1) + "s");

    ui->labelplcState->setText(PRINT_PLC_STATE);

    ui->tbnSysDevCheck->setVisible(false);
    ui->tbnManualCheckDev->setVisible(false);
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

void FSC_MainWindow::skt_connect_suc(int i)
{
    sktConed[i] = true;
    FSCLOG << QString::number(i) + " socket con";
}

void FSC_MainWindow::skt_connect_dis(int i)
{
    fsc_global::sktTcp[i]->abort();

    sktConed[i] = false;
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

void FSC_MainWindow::on_tbnSysDevCheck_clicked()
{

}

void FSC_MainWindow::on_tbnManualCheckDev_clicked()
{
    Dialog_CheckDev *ChkDev = new Dialog_CheckDev();
    ChkDev->exec();
}

void FSC_MainWindow::on_comboBox_PlotSenSel_currentIndexChanged(const QString &arg1)
{
    PlotReplay(arg1);
}

void FSC_MainWindow::on_comboBox_SensorTypeName_currentIndexChanged(int index)
{
    if (fsc_global::para_ini.size() <= index || index < 0)
    {
        return;
    }

    ui->leFlowSpeed_SensorSpan->setText(QString::number(fsc_global::para_ini.at(index).span_ml_per_min));

    for (int i = 0; i < SPAN_NUMBER; i++)
    {
        checkBox_spanCal[i]->setChecked(fsc_global::para_ini.at(index).spanCal[i]);
        checkBox_spanCheck[i]->setChecked(fsc_global::para_ini.at(index).spanCheck[i]);
        checkBox_spanCorrect[i]->setChecked(fsc_global::para_ini.at(index).spanCorrect[i]);

        checkBox_spanCalReverse[i]->setChecked(fsc_global::para_ini.at(index).spanCalReverse[i]);
        checkBox_spanCheckReverse[i]->setChecked(fsc_global::para_ini.at(index).spanCheckReverse[i]);
        checkBox_spanCorrectReverse[i]->setChecked(fsc_global::para_ini.at(index).spanCorrectReverse[i]);
    }

    ui->leFMTypeName->setText(fsc_global::para_ini.at(index).type_name);
}


void FSC_MainWindow::on_lineEdit_setFlowRate_textChanged(const QString &arg1)
{
    (void)arg1;

    showSetFlowRate = ui->lineEdit_setFlowRate->text().toDouble();
}



void FSC_MainWindow::on_lineEdit_setPWM_textChanged(const QString &arg1)
{
    (void)arg1;

    showSetPWM = ui->lineEdit_setPWM->text().toInt();
}



void FSC_MainWindow::on_tbnScaleZero_clicked()
{
    if (calOn == CAL_STATE_STOP)
    {
        reqScaleZero();

        scaleTestZero = 1;
    }
}

void FSC_MainWindow::on_tbnCalStart_clicked()
{
    if (calOn != CAL_STATE_STOP)
    {
        return;
    }

    calOn = CAL_STATE_STOP;

    on_tbnPoltClear_clicked();

    calStepInfoFresh();
    if (currentStep.stepTotal > 0)
    {
        calOn = CAL_START;
    }
}

void FSC_MainWindow::on_tbnCalTermination_clicked()
{
    calOn = CAL_STATE_STOP;

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 终止"));

    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

    currentStep.stepCurrent = 0;
}

void FSC_MainWindow::on_tbnPoltClear_clicked()
{
    if (calOn == CAL_STATE_STOP)
    {
        plotLoop = 0;

        plotScaleSumTimeX.clear();
        plotSTDFMSumTimeX.clear();
        plotSTDFMFlowTimeX.clear();
        plotFMSumTimeX.clear();
        plotFMFlowTimeX.clear();

        plotScaleSumValueY.clear();
        plotSTDFMSumValueY.clear();
        plotSTDFMFlowValueY.clear();
        plotFMSumValueY.clear();
        plotFMFlowValueY.clear();

        plotFresh();

    }
}

void FSC_MainWindow::on_pushButton_debugReturn_clicked()
{
    showSetFlowRate = 1000;
    showSetPWM = 80;

    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));

    setFixedSize(1625, 880);
}

void FSC_MainWindow::buttonDebug_clicked(int i)
{
     debugSkt[i] = !debugSkt[i];
}

void FSC_MainWindow::on_pushButton_debugOff_clicked()
{
    for (int i = 0; i < SOCKET_NUMBER; i++)
    {
        debugSkt[i] = false;
    }
}

void FSC_MainWindow::on_tbnCalStepNext_clicked()
{
    currentStep.stepCurrent++;

    if (currentStep.stepCurrent == currentStep.stepTotal)
    {
        currentStep.stepCurrent--;
    }

    calStepInfoFresh();

}

void FSC_MainWindow::on_tbnModifyFMTypePara_clicked()
{
    dataInit_calStepInit();

    if (calOn != CAL_STATE_STOP)
    {
        return;
    }

    calStepInfoFresh();
}

void FSC_MainWindow::on_tbnCalStepPre_clicked()
{
    if (currentStep.stepCurrent > 0)
    {
       currentStep.stepCurrent--;
    }

    calStepInfoFresh();
}

void FSC_MainWindow::on_tbnCalPause_clicked()
{
    calOn = CAL_STATE_STOP;

    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 暂停"));

    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::paraWrite(void)
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    if (fsc_global::ip_PLC.size() == 0)
    {
        configIni->setValue("IP_ADDRESS/PLC",  "192.168.1.220");
    }
    else
    {
        configIni->setValue("IP_ADDRESS/PLC",  fsc_global::ip_PLC);
    }

    if (fsc_global::ip_PLC.size() == 0)
    {
        configIni->setValue("IP_ADDRESS/RS_SERVER",  "192.168.1.221");
    }
    else
    {
        configIni->setValue("IP_ADDRESS/RS_SERVER",  fsc_global::ip_RS_Server);
    }

    for (int i = 0; i < fsc_global::para_ini.size(); i++)
    {
        configIni->setValue( "sensor_type_" + QString::number(i + 1) +"/type_name" ,  fsc_global::para_ini[i].type_name);
        configIni->setValue( "sensor_type_" + QString::number(i + 1) +"/span_ml_per_min",  fsc_global::para_ini[i].span_ml_per_min);

        QString str;
        for (int k = 0; k < SPAN_NUMBER; k++)
        {
            str = QString().sprintf("/%d_percent_span_to_be_calibrate", (k + 1) * 10 );
            configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, fsc_global::para_ini[i].spanCal[k]);

            str = QString().sprintf("/%d_percent_span_to_be_verify", (k + 1) * 10 );
            configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, fsc_global::para_ini[i].spanCheck[k]);

            str = QString().sprintf("/%d_percent_span_to_be_correct", (k + 1) * 10 );
            configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, fsc_global::para_ini[i].spanCorrect[k]);


            str = QString().sprintf("/%d_percent_span_to_be_calibrate_reverse", (k + 1) * 10 );
            configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, fsc_global::para_ini[i].spanCalReverse[k]);

            str = QString().sprintf("/%d_percent_span_to_be_verify_reverse", (k + 1) * 10 );
            configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, fsc_global::para_ini[i].spanCheckReverse[k]);

            str = QString().sprintf("/%d_percent_span_to_be_correct_reverse", (k + 1) * 10 );
            configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, fsc_global::para_ini[i].spanCorrectReverse[k]);

        }

    }

    delete configIni;
}

void FSC_MainWindow::on_tbnParaAdd_clicked()
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    if (fsc_global::ip_PLC.size() == 0)
    {
        configIni->setValue("IP_ADDRESS/PLC",  "192.168.1.220");
    }
    else
    {
        configIni->setValue("IP_ADDRESS/PLC",  fsc_global::ip_PLC);
    }

    if (fsc_global::ip_PLC.size() == 0)
    {
        configIni->setValue("IP_ADDRESS/RS_SERVER",  "192.168.1.221");
    }
    else
    {
        configIni->setValue("IP_ADDRESS/RS_SERVER",  fsc_global::ip_RS_Server);
    }

    if (ui->leFMTypeName->text().trimmed().size() > 0)
    {
        int i;

        for (i = 0; i < fsc_global::para_ini.size(); i++)
        {
            if (QString::compare(fsc_global::para_ini[i].type_name, ui->leFMTypeName->text().trimmed()) == 0)
            {

                configIni->setValue( "sensor_type_" + QString::number(i + 1) +"/type_name" ,  fsc_global::para_ini[i].type_name);
                configIni->setValue( "sensor_type_" + QString::number(i + 1) +"/span_ml_per_min",  fsc_global::para_ini[i].span_ml_per_min);

                QString str;
                for (int k = 0; k < SPAN_NUMBER; k++)
                {
                    str = QString().sprintf("/%d_percent_span_to_be_calibrate", (k + 1) * 10 );
                    configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, checkBox_spanCal[k]->checkState());

                    str = QString().sprintf("/%d_percent_span_to_be_verify", (k + 1) * 10 );
                    configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, checkBox_spanCheck[k]->checkState());

                    str = QString().sprintf("/%d_percent_span_to_be_correct", (k + 1) * 10 );
                    configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, checkBox_spanCorrect[k]->checkState());


                    str = QString().sprintf("/%d_percent_span_to_be_calibrate_reverse", (k + 1) * 10 );
                    configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, checkBox_spanCalReverse[k]->checkState());

                    str = QString().sprintf("/%d_percent_span_to_be_verify_reverse", (k + 1) * 10 );
                    configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, checkBox_spanCheckReverse[k]->checkState());

                    str = QString().sprintf("/%d_percent_span_to_be_correct_reverse", (k + 1) * 10 );
                    configIni->setValue( "sensor_type_" + QString::number(i + 1) + str, checkBox_spanCorrectReverse[k]->checkState());

                }

                goto end_return;
            }

        }

        fsc_para_ini tmp;

        tmp.type_name = ui->leFMTypeName->text().trimmed();
        tmp.span_ml_per_min = ui->leFlowSpeed_SensorSpan->text().toDouble();

        for (int k = 0; k < SPAN_NUMBER; k++)
        {
            tmp.spanCal[k] = checkBox_spanCal[k]->checkState();
            tmp.spanCheck[k] = checkBox_spanCheck[k]->checkState();
            tmp.spanCorrect[k] = checkBox_spanCorrect[k]->checkState();

            tmp.spanCalReverse[k] = checkBox_spanCalReverse[k]->checkState();
            tmp.spanCheckReverse[k] = checkBox_spanCheckReverse[k]->checkState();
            tmp.spanCorrectReverse[k] = checkBox_spanCorrectReverse[k]->checkState();

        }

        fsc_global::para_ini.append(tmp);

        paraWrite();

    }

end_return:

    delete configIni;

    ParaInit();
}

void FSC_MainWindow::on_tbnParaErase_clicked()
{
    for (int i = 0; i < fsc_global::para_ini.size(); i++)
    {
        if (QString::compare(fsc_global::para_ini[i].type_name, ui->leFMTypeName->text().trimmed()) == 0)
        {
            fsc_global::para_ini.remove(i);

            //fsc_global::para_ini.resize(fsc_global::para_ini.size() - 1);

            QFile::remove("para.ini");

            paraWrite();
            ParaInit();

            return;
        }
    }
}


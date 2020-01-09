#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"
#include "dialog_showinfo.h"
#include "dialog_fmdata.h"
#include "dialog_sensoraddr.h"

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
    mainLoopTimer(new QTimer(this)),
    startUpTimer(new QTimer(this)),
    calTimer(new QTimer(this)),
    STFMTimer(new QTimer(this))
{
    ui->setupUi(this);

    uiReInit();

    connect(startUpTimer, SIGNAL(timeout()), this, SLOT(startUp()));
    startUpTimer->start(200);

    connect(STFMTimer, SIGNAL(timeout()), this, SLOT(startSTFM()));
}

FSC_MainWindow::~FSC_MainWindow()
{
    showInfo->close();
    delete ui;
}

void FSC_MainWindow::startUp()
{
    startUpTimer->stop();

    FSCLOG << "SocketDataInit";
    SocketDataInit();

    ParaInit();
    FSCLOG << "ParaInit";

    DataInit();
    FSCLOG << "DataInit";

    plcDataInit();
    FSCLOG << "plcDataInit";

    PlotInit();
    FSCLOG << "PlotInit";

    showFresh();

    connect(calTimer, SIGNAL(timeout()), this, SLOT(startCal()));
    connect(mainLoopTimer, SIGNAL(timeout()), this, SLOT(mainLoop()));
    mainLoopTimer->start(MAIN_LOOP_CYCLE);
}

void FSC_MainWindow::SocketDataInit(void)
{
    sktConMapper = new QSignalMapper();
    sktDisconMapper = new QSignalMapper();
    sktErrMapper = new QSignalMapper();
    sktReadMapper = new QSignalMapper();

    for( int i = 0; i < SOCKET_NUMBER; i++)
    {
        sktConed[i] = false;
        sktConCommandTime[i] = 0;
        sktTimeoutNum[i] = 0;
        sktStationAddr[i] = FM_STATION_ADDRESS;

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
}

void FSC_MainWindow::uiReInit(void)
{
    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    FSCLOG << screen_width << screen_height;

    int fixedWidth = 1600;
    int fixedHeight = 870;

    int fontSize = 12;
    int btnW = 120;
    int btnH = 20;

    this->setFixedSize(fixedWidth, fixedHeight);
    setWindowFlags( Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint| Qt::WindowMaximizeButtonHint);

    QString qss = tr("QWidget { font-size: %0px; } #btnWbTest{ width: %1px; height: %2px; }").arg(fontSize).arg(btnW).arg(btnH);
    this->setStyleSheet(qss);

    int x = 85;
    int y = 143;
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

    txtBrow_showInfo = ui->textBrow_calInfo;

    buttonDebugMapper = new QSignalMapper();
    for (int i = 0; i < SOCKET_NUMBER; i++)
    {
        sktBufRev[i].resize(0);
        sktBufSend[i].resize(0);
        sktRespondOk[i] = true;
        sktPause[i] = false;

        sktDataState[i] = DATA_READ_OK;

        debugSkt[i] = false;

        buttonDebug[i] = new QPushButton(this);
        buttonDebug[i]->setText(QString::number(i));
        buttonDebug[i]->setFixedSize(75,23);
        buttonDebug[i]->move(1645, 115 + i * 29);
        buttonDebug[i]->show();

        connect(buttonDebug[i], SIGNAL(clicked()), buttonDebugMapper, SLOT(map()));
        buttonDebugMapper->setMapping(buttonDebug[i], i);
    }
    connect(buttonDebugMapper, SIGNAL(mapped(int)), this, SLOT(buttonDebug_clicked(int)));


    rightKeyCorrectMenuMapper =  new QSignalMapper();
    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        rightKeyCorrectMenu[i] = new QAction(QString().sprintf("%d#传感器", i + 1), ui->tbnCalManualCorrectWrite);
        ui->tbnCalManualCorrectWrite->addAction(rightKeyCorrectMenu[i]);
        ui->tbnCalManualCorrectWrite->setContextMenuPolicy(Qt::ActionsContextMenu);

        connect(rightKeyCorrectMenu[i], SIGNAL(triggered()), rightKeyCorrectMenuMapper, SLOT(map()));
        rightKeyCorrectMenuMapper->setMapping(rightKeyCorrectMenu[i], i);

    }
    connect(rightKeyCorrectMenuMapper, SIGNAL(mapped(int)), this, SLOT(action_rightKeyCorrectMenu(int)));


    showInfo = static_cast<QDialog*>(new Dialog_showinfo(this));
    dialog_fmData = static_cast<QDialog*>(new Dialog_fmdata(this));
    dialog_fmRegister = static_cast<QDialog*>(new Dialog_SensorAddr(this));


    ui->tbnSysDevCheck->setVisible(false);
    ui->tbnManualCheckDev->setVisible(false);

    ui->tbnPump1ReverseOff->setVisible(false);
    ui->tbnPump2ReverseOff->setVisible(false);

    calGoingInfoLabRemove();
}

void FSC_MainWindow::socketParaInit(void)
{
    quint16 i = 0;
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    fsc_global::ip_PLC = configIni->value("IP_ADDRESS/PLC" ).toString();
    fsc_global::ip_RS_Server = configIni->value("IP_ADDRESS/RS_SERVER" ).toString();

    if (fsc_global::ip_PLC.size() == 0)
    {
        fsc_global::ip_PLC = "192.168.1.220";
    }
    if (fsc_global::ip_RS_Server.size() == 0)
    {
        fsc_global::ip_RS_Server = "192.168.1.221";
    }


    fsc_global::ip[0] = fsc_global::ip_PLC;
    fsc_global::port_number[0] = 2100;

    for (i = 1; i < SOCKET_NUMBER; i++)
    {
        fsc_global::ip[i] = fsc_global::ip_RS_Server;
        fsc_global::port_number[i] = 4000 + i;
    }

    FSCLOG << fsc_global::ip_PLC;
    FSCLOG << fsc_global::ip_RS_Server;

    if (configIni->value("IP_ADDRESS/DEBUG_NON_FIRST" ).toString() != "")
    {
        firstCal = ! configIni->value("IP_ADDRESS/DEBUG_NON_FIRST" ).toBool();
    }
    if (configIni->value("IP_ADDRESS/DEBUG_PLOT_POS_NUMBER" ).toString() != "")
    {
        plotPosNumber = configIni->value("IP_ADDRESS/DEBUG_PLOT_POS_NUMBER" ).toInt();
    }
    for(i = 0; i < SOCKET_NUMBER; i++)
    {
        if (configIni->value("IP_ADDRESS/DEBUG_SOCKET_" +  QString::number(i)).toString() != "")
        {
            debugSkt[i] =  configIni->value("IP_ADDRESS/DEBUG_SOCKET_" +  QString::number(i)).toBool();
        }
    }

    if (configIni->value("IP_ADDRESS/POLL_CYCLE_STFM" ).toString() != "")
    {
        pollCycleSTFM = configIni->value("IP_ADDRESS/POLL_CYCLE_STFM" ).toInt();
        FSCLOG << "pollCycleSTFM=" << pollCycleSTFM;
    }
    if (configIni->value("IP_ADDRESS/PRINTER_NAME" ).toString() != "")
    {
        printerName = configIni->value("IP_ADDRESS/PRINTER_NAME" ).toString();
        FSCLOG << "printerName=" << printerName;
    }
    if (configIni->value("IP_ADDRESS/PRINTER_AUTO" ).toString() != "")
    {
        autoPrinterBool = configIni->value("IP_ADDRESS/PRINTER_AUTO" ).toBool();
        FSCLOG << "printerName=" << autoPrinterBool;

        if (autoPrinterBool)
        {
            autoPrinter = Qt::Checked;
        }
        else
        {
            autoPrinter = Qt::Unchecked;
        }
    }

    delete configIni;
}

void FSC_MainWindow::ParaInit(void)
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

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

void FSC_MainWindow::DataInit(void)
{
    showScaleSum = static_cast<double>(nanf(""));
    showScaleFlow = static_cast<double>(nanf(""));
    showSTDFMSum = static_cast<double>(nanf(""));
    showSTDFMFlow = static_cast<double>(nanf(""));

    fmRWMapper = new QSignalMapper();
    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        showFMSum[i] = static_cast<double>(nanf(""));
        showFMFlow[i] = static_cast<double>(nanf(""));

        fm_valu_read_valid[i] = 0;
        fm_write_suced[i] = 0;

        fmSendMsg[i].clear();
        fmRevMsg[i].clear();

        fmReadWriteSelect[i] = 0;
        fmRWflag[i] = 0;
        fmRWTimer[i] = new QTimer(this);

        connect(fmRWTimer[i], SIGNAL(timeout()), fmRWMapper, SLOT(map()));
        fmRWMapper->setMapping(fmRWTimer[i], i);

        fmData[i].fm_valueGAIN_CONTROL = 0;
        fmData[i].fm_valu_read_valid = 0;
        fmData[i].fm_valueSET_KF = 0;

        for (int k = 0; k < XUNYIN_SET_KF_NUM; k++)
        {
            fmData[i].fm_valueSET_KF1[k] = 0;
            fmData[i].fm_valueSET_KF2[k] = 0;
        }
        for (int k = 0; k < XUNYIN_SET_FLOW_RANGE_NUM; k++)
        {
            fmData[i].fm_valueFLOW_RANGE[k] = 0;
        }
        for (int k = 0; k < XUNYIN_SET_ZERO_CAL_NUM; k++)
        {
            fmData[i].fm_valueZERO_CAL[k] = 0;
        }

     }
    connect(fmRWMapper, SIGNAL(mapped(int)), this, SLOT(fmRWTimerOn(int)));

    stfmRWTimer =  new QTimer(this);
    connect(stfmRWTimer, SIGNAL(timeout()), this, SLOT(stfmRWTimerOn()));
    stfmSendMsg.clear();
    stfmRevMsg.clear();

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

    for(int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        plotFMSumValueY_bak[i].clear();
        plotFMFlowValueY_bak[i].clear();
    }

    allCalNeedToReport = false;
    for(int i = 0; i < CAL_MAX_STEP; i++)
    {
        allCalAvailable[i]  = false;
    }

    ui->textBrow_calInfo->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 程序启动"));
    dataInit_calStepInit();
    calStepInfoFresh();

    calRunLink.clear();
    calRunLink_calStepInfoInit();
    calRunLink_listInit();
    calRunLink_StepInfoFresh();

}

void FSC_MainWindow::calRunLink_listInit(void)
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    QString str;

    for (int i = 0; i < fsc_global::para_ini.size(); i++)
    {
        if (QString::compare(fsc_global::para_ini[i].type_name, currentStep.type_name) == 0)
        {

            for (int k = 0; k < calRunLink.size(); k++)
            {
                str = "sensor_type_" + QString::number(i + 1);
            }
            break;
        }
    }

    if (str.isEmpty())
    {
        return;
    }

    QVector<int> calLinklist;

    int i = 0;
    int pos = 0;

    while(true)
    {
        i++;

        pos = configIni->value( str + QString().sprintf("/step_%d", i)).toInt();

        if (pos > 0)
        {
             calLinklist.append(pos);
        }
        else
        {
            break;
        }
    }

    QVector<calLink> linkTmp;

    for (i = 0; i < calRunLink.size(); i++)
    {
        if (i >= calLinklist.size())
        {
            break;
        }

        if (calLinklist[i] > calRunLink.size())
        {
            break;
        }

        linkTmp.append(calRunLink[calLinklist[i] - 1]);
    }

    calRunLink = linkTmp;

}

void FSC_MainWindow::dataInit_calStepInit(void)
{
    currentStep.type_name = ui->comboBox_SensorTypeName->currentText();
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

void FSC_MainWindow::calStepInfoFreshOnUI(void)
{
    QString str;
    int stepShow = currentStep.stepCurrent;
    int stepTotalShow = 0;

    printInfoWithTime(" 型号：");
    printInfo(currentStep.type_name);
    printInfo("量程：");
    printInfo(QString::number(currentStep.span_ml_per_min, 'f', 3));
    printInfo(" |-> ");

    if (oneCal.step != 0)
    {
        stepShow++;
        if (stepShow > currentStep.stepTotal)
        {
            stepShow = currentStep.stepTotal;
        }
    }

    for (int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCal[i])
        {
            stepTotalShow++;

            if (stepTotalShow > stepShow)
            {
                str += "(" + QString::number(stepTotalShow) + ")";
                str += QString::number((i + 1) * 10) + "%量程标定-> ";
            }
        }
        if (currentStep.spanCorrect[i])
        {
            stepTotalShow++;

            if (stepTotalShow > stepShow)
            {
                str += "(" + QString::number(stepTotalShow) + ")";
                str += QString::number((i + 1) * 10) + "%量程修正-> ";
            }
        }
        if (currentStep.spanCheck[i])
        {
            stepTotalShow++;

            if (stepTotalShow > stepShow)
            {

                str += "(" + QString::number(stepTotalShow) + ")";
                str += QString::number((i + 1) * 10) + "%量程验证-> ";
            }

        }
    }


    for (int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCalReverse[i])
        {
            stepTotalShow++;

            if (stepTotalShow > stepShow)
            {
                str += "(" + QString::number(stepTotalShow) + ")";
                str += "反向" + QString::number((i + 1) * 10) + "%量程标定-> ";
            }
        }
        if (currentStep.spanCorrectReverse[i])
        {
            stepTotalShow++;

            if (stepTotalShow > stepShow)
            {
                str += "(" + QString::number(stepTotalShow) + ")";
                str += "反向" + QString::number((i + 1) * 10) + "%量程修正-> ";
            }
        }
        if (currentStep.spanCheckReverse[i])
        {
            stepTotalShow++;

            if (stepTotalShow > stepShow)
            {
                str += "(" + QString::number(stepTotalShow) + ")";
                str += "反向" + QString::number((i + 1) * 10) + "%量程验证-> ";
            }
        }
    }

    str += "|\r\n";
    printInfo(str);
}

void FSC_MainWindow::calRunLink_calStepInfoInit(void)
{
    calLink calLinkItem;

    calRunLink.clear();

    for (int i = 0; i < currentStep.stepTotal; i++)
    {
        if (stepCal_dir_type_span(&calLinkItem.direct, &calLinkItem.type, &calLinkItem.spanPercent, &calLinkItem.span, i))
        {
            calLinkItem.stepNum = i;

            calRunLink.append(calLinkItem);
        }
    }
}

void FSC_MainWindow::calRunLink_StepInfoFresh(void)
{
    QString str;

    str = ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:");

    str += " 型号：";
    str += currentStep.type_name;
    str += "  ";
    str += "量程：";
    str += QString::number(currentStep.span_ml_per_min, 'f', 3);
    str += QString().sprintf(" 共%d步|-> ", calRunLink.size());

    int size = calRunLink.size();

    for (int i = curShowStep; i < size; i++)
    {
        str += "(" + QString::number(calRunLink[i].stepNum + 1) + ")";

        if (calRunLink[i].direct == START_CAL_DIRECT_REVERSE)
        {
            str += "反向";
        }

        str += QString::number(calRunLink[i].spanPercent) + "%量程";

        if (calRunLink[i].type == START_CAL_TYPE_CAL)
        {
            str += "标定-> ";
        }
        else if (calRunLink[i].type == START_CAL_TYPE_CORRECT)
        {
            str += "修正-> ";
        }
        else if (calRunLink[i].type == START_CAL_TYPE_CHECK)
        {
            str += "验证-> ";
        }

    }

    str += "|\r\n";
    ui->textBrow_calInfo->setText(str);

    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
}

void FSC_MainWindow::calStepInfoFresh(void)
{
    QString str;

    str = ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:");

    str += " 型号：";
    str += currentStep.type_name;
    str += "  ";
    str += "量程：";
    str += QString::number(currentStep.span_ml_per_min, 'f', 3);
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
//    ui->textBrow_calInfo->setText(str);

    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

}

void FSC_MainWindow::mainLoop(void)
{
    QString str;

    static int j = 0;

    j++;

    socketCommunication();

    if ( sktConed[SOCKET_PLC_INDEX] && ( (j % (POLL_PLC_CYCLE / MAIN_LOOP_CYCLE)) == 0) )
    {
        reqSetPLCWithSTFM();
    }

    if ( sktConed[SOCKET_SCALE_INDEX] && ( (j % (POLL_SCALE_CYCLE / MAIN_LOOP_CYCLE)) == 0) )
    {
        if (sktDataState[SOCKET_SCALE_INDEX] == DATA_READ_OK || \
                (sktDataWriteTime[SOCKET_SCALE_INDEX] + DATA_READ_TIMEOUT) < QDateTime::currentDateTime().toTime_t() )
        {
            reqScaleShow();

            sktDataState[SOCKET_SCALE_INDEX] = DATA_WRITE_OK;
            sktDataWriteTime[SOCKET_SCALE_INDEX] = QDateTime::currentDateTime().toTime_t();
        }
    }

    if ( sktConed[SOCKET_SCALE_INDEX] && ( (j % (POLL_FM_CYCLE / MAIN_LOOP_CYCLE)) == 0) )
    {
        if (!STFMTimer->isActive())
        {
            STFMTimer->start(pollCycleSTFM);
        }

        reqFMData(SOCKET_FLOWM1_INDEX);
        reqFMData(SOCKET_FLOWM2_INDEX);
        reqFMData(SOCKET_FLOWM3_INDEX);
        reqFMData(SOCKET_FLOWM4_INDEX);
        reqFMData(SOCKET_FLOWM5_INDEX);
        reqFMData(SOCKET_FLOWM6_INDEX);
    }

    if ( sktConed[SOCKET_SCALE_INDEX] && ( (j % (POLL_FM_CYCLE / MAIN_LOOP_CYCLE)) == 0) )
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

    if ( (j % (POLL_FM_CYCLE / MAIN_LOOP_CYCLE)) == 0 )
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

        //setWindowState(Qt::WindowMaximized);
        //setWindowFlags(Qt::Window);
        //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint| Qt::WindowMaximizeButtonHint);

    }

    //if (j % (POLL_SCALE_CYCLE / MAIN_LOOP_CYCLE) == 0)
    {
         showFresh();
    }

    if (calOn && !calTimer->isActive())
    {
        calTimer->start(MS_500);
    }
}


void FSC_MainWindow::startSTFM(void)
{
    if ( sktConed[SOCKET_SCALE_INDEX]  )
    {

        reqFMData(SOCKET_STD_FLOWM_INDEX);

    }

}

int FSC_MainWindow::stepCal_dir_type_span(int *dir, int *type, int *spanPercent, double *span, int stepNum)
{
    int step = 0;

    if (dir == nullptr || type == nullptr || spanPercent == nullptr || span == nullptr )
    {
        FSCLOG << "err:" << "startCal_dir_type_span" << "nullptr";
    }

    for(int i = SPAN_NUMBER - 1; i >= 0; i--)
    {
        if (currentStep.spanCal[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (stepNum == step)
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
            if (stepNum == step)
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
            if (stepNum == step)
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
            if (stepNum  == step)
            {
               *dir     =   START_CAL_DIRECT_REVERSE;
               *type    =   START_CAL_TYPE_CAL;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }

        if (currentStep.spanCorrectReverse[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (stepNum == step)
            {
               *dir     =   START_CAL_DIRECT_REVERSE;
               *type    =   START_CAL_TYPE_CORRECT;
               *spanPercent    =   (i + 1) * 10;
               *span    =   currentStep.span_ml_per_min * (*spanPercent) / 100;

                return true;
            }
            step++;
         }

        if (currentStep.spanCheckReverse[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
        {
            if (stepNum == step)
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

int FSC_MainWindow::startCal_dir_type_span(int *dir, int *type, int *spanPercent, double *span)
{
    int step = 0;

    if (dir == nullptr || type == nullptr || spanPercent == nullptr || span == nullptr )
    {
        FSCLOG << "err:" << "startCal_dir_type_span" << "nullptr";
    }

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

        if (currentStep.spanCorrectReverse[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
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

        if (currentStep.spanCheckReverse[i] == CAL_CURRENT_STAT_NEED_EXECUTE)
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

    if (calOn == CAL_STATE_STOP)
    {
        ui->groupBox_config->setEnabled(true);

        ui->tbnCalStart->setEnabled(sktConed[SOCKET_PLC_INDEX] && revdSketPLC && !std::isnan(showScaleSum));

        ui->lineEdit_setFlowRate->setEnabled(revdSketPLC);
        ui->lineEdit_setPWM->setEnabled(revdSketPLC);
        ui->radioButton_setFlowRate->setEnabled(revdSketPLC);
        ui->radioButton_setPWM->setEnabled(revdSketPLC);

        ui->tbnScaleZero->setEnabled(true);
        ui->tbnPoltClear->setEnabled(true);
        ui->tbnCalManual->setEnabled(true);
        ui->lineEdit_calParameter->setEnabled(true);
        ui->tbnCalManualWrite->setEnabled(true);
        ui->tbnCalManualCorrectWrite->setEnabled(true);

    }
    else
    {
        ui->groupBox_config->setEnabled(false);

        ui->tbnCalStart->setEnabled(false);

        ui->lineEdit_setFlowRate->setEnabled(false);
        ui->lineEdit_setPWM->setEnabled(false);
        ui->radioButton_setFlowRate->setEnabled(false);
        ui->radioButton_setPWM->setEnabled(false);

        ui->tbnScaleZero->setEnabled(false);
        ui->tbnPoltClear->setEnabled(false);
        ui->tbnCalManual->setEnabled(false);
        ui->lineEdit_calParameter->setEnabled(false);
        ui->tbnCalManualWrite->setEnabled(false);
        ui->tbnCalManualCorrectWrite->setEnabled(false);

    }

    if (calOn != CAL_STATE_STOP)
    {
        if (!sktConed[SOCKET_PLC_INDEX] || !revdSketPLC || (std::isnan(showScaleSum) && !sktConed[SOCKET_SCALE_INDEX])|| !checkPlc() )
        {
            calFaultStop(&oneCal);
        }
    }

    showPlotFresh();
    showPlcFresh();
}

void FSC_MainWindow::on_tbnSysDevCheck_clicked()
{
    Dialog_SensorAddr *ChkDev = new Dialog_SensorAddr();
    ChkDev->exec();
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

    oneCal.step = 0;
    currentStep.stepCurrent = 0;
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

    calRunLink_StepInfoFresh();

    for(int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        plotFMSumValueY_bak[i].clear();
        plotFMFlowValueY_bak[i].clear();
    }

    ui->groupBox_manual->setEnabled(true);
    calManualDoing = false;
}

void FSC_MainWindow::on_tbnCalTermination_clicked()
{
    calOn = CAL_STATE_STOP;
    currentStep.stepCurrent = 0;
    curShowStep = 0;

    printInfoWithTime(" 终止");
    calStop(&oneCal);

    allCalEnd = true;

    if (ui->groupBox_manual->isEnabled())
    {
        makeCalRecordPrint(&oneCal);
    }
    else
    {
        ui->groupBox_manual->setEnabled(true);
        calManualDoing = false;
    }
}

void FSC_MainWindow::on_tbnPoltClear_clicked()
{
    if (calOn == CAL_STATE_STOP && calOn != CAL_PLOT_START )
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

        for(int i = 0; i < FLOWMETER_NUMBER; i++)
        {
            plotFMSumValueY_bak[i].clear();
            plotFMFlowValueY_bak[i].clear();
        }

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

    if (currentStep.stepCurrent > currentStep.stepTotal)
    {
        currentStep.stepCurrent = currentStep.stepTotal;
    }

    if (calOn == CAL_STATE_STOP ||calOn == CAL_PLOT_START)
    {
        calStepInfoFresh();
    }
    else
    {
        calStepInfoFreshOnUI();
    }

    curShowStep++;
    if (curShowStep > calRunLink.size())
    {
        curShowStep = calRunLink.size();
    }
    calRunLink_StepInfoFresh();
}

void FSC_MainWindow::on_tbnCalStepPre_clicked()
{

    if (currentStep.stepCurrent > 0)
    {
        currentStep.stepCurrent--;
    }
    else
    {
        oneCal.step = 0;
    }


    if (oneCal.step >= currentStep.stepTotal)
    {
        oneCal.step--;
    }

    if (calOn == CAL_STATE_STOP ||calOn == CAL_PLOT_START)
    {
        calStepInfoFresh();
    }
    else
    {
        calStepInfoFreshOnUI();
    }

    if (curShowStep > 0)
    {
        curShowStep--;
    }
    calRunLink_StepInfoFresh();
}

void FSC_MainWindow::on_tbnModifyFMTypePara_clicked()
{
    dataInit_calStepInit();

    if (calOn != CAL_STATE_STOP)
    {
        return;
    }

    allCalNeedToReport = false;
    oneCal.step = 0;

    currentStep.stepCurrent = 0;
    curShowStep = 0;

    calStepInfoFresh();


    calRunLink.clear();
    calRunLink_calStepInfoInit();
    calRunLink_StepInfoFresh();
}

void FSC_MainWindow::on_tbnCalPause_clicked()
{
    ui->textBrow_calInfo->setText(ui->textBrow_calInfo->toPlainText() + "\r\n\r\n" + \
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: 即将暂停"));

    ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

    oneCal.pause = true;
}

void FSC_MainWindow::paraIpWrite(void)
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

    delete configIni;
}

void FSC_MainWindow::paraWrite(void)
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    paraIpWrite();

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

void FSC_MainWindow::paraStepWrite(void)
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    for (int i = 0; i < fsc_global::para_ini.size(); i++)
    {
        if (QString::compare(fsc_global::para_ini[i].type_name, currentStep.type_name) == 0)
        {

            for (int k = 0; k < calRunLink.size(); k++)
            {
                configIni->setValue( "sensor_type_" + QString::number(i + 1) + QString().sprintf("/step_%d", k + 1), calRunLink[k].stepNum + 1);
            }
            break;

        }

    }

    delete configIni;
}

void FSC_MainWindow::on_tbnParaAdd_clicked()
{
    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);

    paraIpWrite();

    if (ui->leFMTypeName->text().trimmed().size() > 0)
    {
        for (int i = 0; i < fsc_global::para_ini.size(); i++)
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

    on_tbnModifyFMTypePara_clicked();
    paraStepWrite();;
    ParaInit();
}

void FSC_MainWindow::on_tbnParaErase_clicked()
{
    for (int i = 0; i < fsc_global::para_ini.size(); i++)
    {
        if (QString::compare(fsc_global::para_ini[i].type_name, ui->leFMTypeName->text().trimmed()) == 0)
        {
            fsc_global::para_ini.remove(i);

            QFile::remove("para.ini");

            paraWrite();
            ParaInit();

            return;
        }
    }
}

void FSC_MainWindow::on_tbnFMCalTable_clicked()
{
    if (!allCalEnd)
    {
        makeCalRecordPrint(allCal);
    }

    showCalTable = true;

    showInfo->show();
}

void FSC_MainWindow::on_pushButton_showInfo_clicked()
{
    Dialog_showinfo *showInfo = new Dialog_showinfo(this);
    showInfo->show();
}

void FSC_MainWindow::on_textBrow_calInfo_textChanged()
{
    showInfoUpdata = true;
}

void FSC_MainWindow::on_pushButton_stepUp_clicked()
{
    int curPos = currentStep.stepCurrent;
    calLink tmp;

    if (curPos <= 0)
    {
        return;
    }

    if (calOn != CAL_STATE_STOP ||calOn == CAL_PLOT_START)
    {
        return;
    }

    tmp = calRunLink[curPos];

    calRunLink[curPos] = calRunLink[curPos - 1];
    calRunLink[curPos - 1] = tmp;

    currentStep.stepCurrent--;
    curShowStep--;
    calRunLink_StepInfoFresh();
}

void FSC_MainWindow::on_pushButton_stepDown_clicked()
{
    int curPos = currentStep.stepCurrent;
    calLink tmp;

    if (curPos >= calRunLink.size())
    {
        return;
    }

    if (calOn != CAL_STATE_STOP ||calOn == CAL_PLOT_START)
    {
        return;
    }

    tmp = calRunLink[curPos];

    calRunLink[curPos] = calRunLink[curPos + 1];
    calRunLink[curPos + 1] = tmp;

    currentStep.stepCurrent++;
    curShowStep++;
    calRunLink_StepInfoFresh();
}

void FSC_MainWindow::on_pushButton_stepSave_clicked()
{
    if (calOn != CAL_STATE_STOP ||calOn == CAL_PLOT_START)
    {
        return;
    }

    currentStep.stepCurrent = 0;
    curShowStep = 0;
    calRunLink_StepInfoFresh();

    paraStepWrite();
}

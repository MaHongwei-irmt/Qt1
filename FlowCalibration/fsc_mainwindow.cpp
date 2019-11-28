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

    ParaInit();
    PlotInit();
    DataInit();
    SocketInit();

    connect(mainLoopTimer, SIGNAL(timeout()), this, SLOT(mainLoop()));
    mainLoopTimer->start(100);
}

FSC_MainWindow::~FSC_MainWindow()
{
    delete ui;
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
    fsc_global::port_number[1] = 4001;
    fsc_global::port_number[2] = 4002;
    fsc_global::port_number[3] = 4003;
    fsc_global::port_number[4] = 4004;
    fsc_global::port_number[5] = 4005;
    fsc_global::port_number[6] = 4006;
    fsc_global::port_number[7] = 4007;
    fsc_global::port_number[8] = 4008;
    fsc_global::port_number[9] = 4009;
    fsc_global::port_number[10] = 4010;
    fsc_global::port_number[11] = 4011;
    fsc_global::port_number[12] = 4012;
    fsc_global::port_number[13] = 4013;
    fsc_global::port_number[14] = 4014;
    fsc_global::port_number[15] = 4015;
    fsc_global::port_number[16] = 4016;

    FSCLOG << fsc_global::ip_PLC;
    FSCLOG << fsc_global::ip_RS_Server;


    int i = 1;
    fsc_para_ini tmp;

    ui->comboBox_SensorTypeName->clear();

    while (configIni->value( "sensor_type_" + QString::number(i) +"/type_name" ).toString() != "")
    {

        tmp.type_name = configIni->value( "sensor_type_" + QString::number(i) +"/type_name" ).toString();
        FSCLOG << tmp.type_name;

        tmp.span_ml_per_min = configIni->value( "sensor_type_" + QString::number(i) +"/span_ml_per_min" ).toDouble();

        tmp.span10_cal[0] = configIni->value( "sensor_type_" + QString::number(i) +"/10_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[1] = configIni->value( "sensor_type_" + QString::number(i) +"/20_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[2] = configIni->value( "sensor_type_" + QString::number(i) +"/30_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[3] = configIni->value( "sensor_type_" + QString::number(i) +"/40_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[4] = configIni->value( "sensor_type_" + QString::number(i) +"/50_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[5] = configIni->value( "sensor_type_" + QString::number(i) +"/60_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[6] = configIni->value( "sensor_type_" + QString::number(i) +"/70_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[7] = configIni->value( "sensor_type_" + QString::number(i) +"/80_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[8] = configIni->value( "sensor_type_" + QString::number(i) +"/90_percent_position_to_be_calibrate" ).toBool();
        tmp.span10_cal[9] = configIni->value( "sensor_type_" + QString::number(i) +"/100_percent_position_to_be_calibrate" ).toBool();

        tmp.span10_check[0] = configIni->value( "sensor_type_" + QString::number(i) +"/10_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[1] = configIni->value( "sensor_type_" + QString::number(i) +"/20_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[2] = configIni->value( "sensor_type_" + QString::number(i) +"/30_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[3] = configIni->value( "sensor_type_" + QString::number(i) +"/40_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[4] = configIni->value( "sensor_type_" + QString::number(i) +"/50_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[5] = configIni->value( "sensor_type_" + QString::number(i) +"/60_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[6] = configIni->value( "sensor_type_" + QString::number(i) +"/70_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[7] = configIni->value( "sensor_type_" + QString::number(i) +"/80_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[8] = configIni->value( "sensor_type_" + QString::number(i) +"/90_percent_position_to_be_verify" ).toBool();
        tmp.span10_check[9] = configIni->value( "sensor_type_" + QString::number(i) +"/100_percent_position_to_be_verify" ).toBool();

        tmp.span10_correct[0] = configIni->value( "sensor_type_" + QString::number(i) +"/10_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[1] = configIni->value( "sensor_type_" + QString::number(i) +"/20_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[3] = configIni->value( "sensor_type_" + QString::number(i) +"/30_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[3] = configIni->value( "sensor_type_" + QString::number(i) +"/40_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[4] = configIni->value( "sensor_type_" + QString::number(i) +"/50_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[5] = configIni->value( "sensor_type_" + QString::number(i) +"/60_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[6] = configIni->value( "sensor_type_" + QString::number(i) +"/70_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[7] = configIni->value( "sensor_type_" + QString::number(i) +"/80_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[8] = configIni->value( "sensor_type_" + QString::number(i) +"/90_percent_position_to_be_correct" ).toBool();
        tmp.span10_correct[9] = configIni->value( "sensor_type_" + QString::number(i) +"/100_percent_position_to_be_correct" ).toBool();

        fsc_global::para_ini.append(tmp);

        ui->comboBox_SensorTypeName->addItem(tmp.type_name);

        i++;
    }

    on_comboBox_SensorTypeName_currentIndexChanged(0);

    delete configIni;

}

void FSC_MainWindow::PlotInit(void)
{
    int iGraphIndex = 0;

    //ui->widget->setBackground(QBrush(Qt::black));

    //设定右上角图形标注可见
    ui->MyCustomPlot->legend->setVisible(true);

    //设定右上角图形标注的字体
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    QVector<double> x(101),y(101);

    //图形为y=x^3
    for(int i=0; i<101; i++)
    {
        x[i] = i/5.0-10;
        y[i] = x[i]*x[i]*x[i] * x[i] * x[i];//qPow(x[i],3)
    }

    //添加图形
    ui->MyCustomPlot->addGraph();

    //设置画笔
    ui->MyCustomPlot->graph(0)->setPen(QPen(Qt::red));

    //设置画刷,曲线和X轴围成面积的颜色
    //ui->MyCustomPlot->graph(0)->setBrush(QBrush(QColor(255,255,0)));

    //设置右上角图形标注名称
    ui->MyCustomPlot->graph(0)->setName("天平");

    //传入数据，setData的两个参数类型为double
    ui->MyCustomPlot->graph(0)->setData(x,y);

    QVector<double> temp(20);
    QVector<double> temp1(20);

    //图形为y = 100*x;
    for(int i=0;i<20;i++)
    {
        temp[i] = i;
        temp1[i] = 1000*i+10;
    }

    //添加图形
    ui->MyCustomPlot->addGraph();

    //设置画笔
    ui->MyCustomPlot->graph(1)->setPen(QPen(Qt::blue));

    //设置画刷,曲线和X轴围成面积的颜色
    //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));

    //传入数据
    ui->MyCustomPlot->graph(1)->setData(temp,temp1);

    /*-------------------------------------------*/
    //画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
    //动态曲线可以通过另一种设置坐标的方法解决坐标问题：
    //setRange ( double position, double size, Qt::AlignmentFlag alignment )
    //参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询
    /*-------------------------------------------*/

    //设置右上角图形标注名称
    ui->MyCustomPlot->graph(1)->setName("标准流量");












    iGraphIndex = 2;


    QVector<double> t(1);
    QVector<double> tl(1);


    for (iGraphIndex = 2; iGraphIndex <= 4; iGraphIndex++)
    {

        t[0] = 30;
        tl[0] = 0;

        //添加图形
        ui->MyCustomPlot->addGraph();

        //设置画笔
        ui->MyCustomPlot->graph(iGraphIndex)->setPen(QPen(Qt::black));

        //设置画刷,曲线和X轴围成面积的颜色
        //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));

        //传入数据：
        ui->MyCustomPlot->graph(iGraphIndex)->setData(t,tl);

        /*-------------------------------------------*/
        //画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
        //动态曲线可以通过另一种设置坐标的方法解决坐标问题：
        //setRange ( double position, double size, Qt::AlignmentFlag alignment )
        //参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询
        /*-------------------------------------------*/

        //设置右上角图形标注名称
        //ui->MyCustomPlot->graph(iGraphIndex)->setName(QString::number(iGraphIndex,10));


        if(iGraphIndex == 3) ui->MyCustomPlot->graph(iGraphIndex)->setName(ui->comboBox_PlotSenSel->currentText() + "流量");
        if(iGraphIndex == 4) ui->MyCustomPlot->graph(iGraphIndex)->setName(ui->comboBox_PlotSenSel->currentText() + "流速");

        if(iGraphIndex == 2)
        {

            ui->MyCustomPlot->graph(iGraphIndex)->setName("标准流速");
            ui->MyCustomPlot->graph(iGraphIndex)->setPen(QPen(Qt::blue));
        }
    }

















    //设置X轴文字标注
    ui->MyCustomPlot->xAxis->setLabel("time");


    //设置Y轴文字标注
    ui->MyCustomPlot->yAxis->setLabel("temp/shidu");

    //设置X轴坐标范围
    //ui->MyCustomPlot->xAxis->setRange(-20,20);

    //设置Y轴坐标范围
    // ui->MyCustomPlot->yAxis->setRange(-1100,1100);

    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->MyCustomPlot->axisRect()->setupFullAxesBox();


    //QFont Ft("Microsoft YaHei");
    //Ft.setPointSize(12);

    //ui->MyCustomPlot->addGraph();
    //ui->MyCustomPlot->graph(0)->setName(tr("Name"));

    ui->MyCustomPlot->xAxis->setLabel(tr("采样点时序"));
    ui->MyCustomPlot->yAxis->setLabel(tr("天平质量/流量/流速"));
    ui->MyCustomPlot->rescaleAxes(true);
    ui->MyCustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);



    ui->MyCustomPlot->replot();

    //-------------------------------------------------------------------
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

        sktBufRev[i].resize(0);
        sktBufSend[i].resize(0);
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

    revdPLC = false;

    plotLoop = 0;
    calOn = false;

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

void FSC_MainWindow::mainLoop()
{
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

    }


    if (j % 5 == 0)
    {

        reqSetPLC(showSetFlowRate, showSetPWM, 1, 2);

        reqFMData(SOCKET_FLOWM1_INDEX);
        //reqFMData(SOCKET_STD_FLOWM_INDEX);

        reqScaleShow();

        plotAddDataAndFresh();

    }//test



    //flushSendBuf();

    showFresh();

}

void FSC_MainWindow::plotAddDataAndFresh(void)
{
    //static uint poltNew = QDateTime::currentDateTime().toTime_t();
    //uint16_t newMS;

    if (calOn)
    {
//        if (plotLoop == 0)
//        {
//            poltNew = QDateTime::currentDateTime().toTime_t();
//        }

        //NewMS = (QDateTime::currentDateTime().toTime_t() - poltNew) * 1000 + static_cast<uint>( QTime::currentTime().msec());

        //plotScaleSumTimeX.append( NewMS );

        plotScaleSumTimeX.append( plotLoop * 0.5 );
        //plotScaleSumValueY.append(showFMSum[0]);


        plotScaleSumValueY.append(showScaleSum);


        //FSCLOG << NewMS  << "   "  << showFMSum[0];

        //FSCLOG << plotLoop * 0.5  << "   "  << showFMSum[0];

        FSCLOG << plotLoop * 0.5  << "   "  << showScaleSum;

        plotFresh();

        if (plotLoop++ > PLOT_VALUE_NUMBER)
        {
            calOn = false;
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

void FSC_MainWindow::plotZero(void)
{
    calOn = false;

    plotScaleSumTimeX.clear();
}

void FSC_MainWindow::PlotReplay(const QString &arg1) {
    //---------------------------曲线-----------------------------
    FSCLOG  << arg1;

    int iGraphIndex = 0;


    //ui->widget->setBackground(QBrush(Qt::black));

    //设定右上角图形标注可见
    ui->MyCustomPlot->legend->setVisible(true);

    //设定右上角图形标注的字体
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    QVector<double> x(101),y(101);

    //图形为y=x^3
    for(int i=0; i<101; i++)
    {
        x[i] = i/5.0-10;
        y[i] = x[i]*x[i]*x[i] * x[i] * x[i];//qPow(x[i],3)
    }

    //添加图形
    //ui->MyCustomPlot->addGraph();

    //设置画笔
    ui->MyCustomPlot->graph(0)->setPen(QPen(Qt::red));

    //设置画刷,曲线和X轴围成面积的颜色
    //ui->MyCustomPlot->graph(0)->setBrush(QBrush(QColor(255,255,0)));

    //设置右上角图形标注名称
    ui->MyCustomPlot->graph(0)->setName("天平");

    //传入数据，setData的两个参数类型为double
    ui->MyCustomPlot->graph(0)->setData(x,y);

    QVector<double> temp(20);
    QVector<double> temp1(20);

    //图形为y = 100*x;
    for(int i=0;i<20;i++)
    {
        temp[i] = i;
        temp1[i] = 1000*i+10;
    }

    //添加图形
    //ui->MyCustomPlot->addGraph();

    //设置画笔
    ui->MyCustomPlot->graph(1)->setPen(QPen(Qt::blue));

    //设置画刷,曲线和X轴围成面积的颜色
    //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));

    //传入数据
    ui->MyCustomPlot->graph(1)->setData(temp,temp1);

    /*-------------------------------------------*/
    //画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
    //动态曲线可以通过另一种设置坐标的方法解决坐标问题：
    //setRange ( double position, double size, Qt::AlignmentFlag alignment )
    //参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询
    /*-------------------------------------------*/

    //设置右上角图形标注名称
    ui->MyCustomPlot->graph(1)->setName("标准流量");












    iGraphIndex = 2;


    QVector<double> t(1);
    QVector<double> tl(1);


    for (iGraphIndex = 2; iGraphIndex <= 4; iGraphIndex++)
    {

        t[0] = 30;
        tl[0] = 0;

        //添加图形
        //ui->MyCustomPlot->addGraph();

        //设置画笔
        ui->MyCustomPlot->graph(iGraphIndex)->setPen(QPen(Qt::black));

        //设置画刷,曲线和X轴围成面积的颜色
        //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));

        //传入数据：
        ui->MyCustomPlot->graph(iGraphIndex)->setData(t,tl);

        /*-------------------------------------------*/
        //画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
        //动态曲线可以通过另一种设置坐标的方法解决坐标问题：
        //setRange ( double position, double size, Qt::AlignmentFlag alignment )
        //参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询
        /*-------------------------------------------*/

        //设置右上角图形标注名称
        //ui->MyCustomPlot->graph(iGraphIndex)->setName(QString::number(iGraphIndex,10));


        if(iGraphIndex == 3) ui->MyCustomPlot->graph(iGraphIndex)->setName(ui->comboBox_PlotSenSel->currentText() + "流量");
        if(iGraphIndex == 4) ui->MyCustomPlot->graph(iGraphIndex)->setName(ui->comboBox_PlotSenSel->currentText() + "流速");

        if(iGraphIndex == 2)
        {

            ui->MyCustomPlot->graph(iGraphIndex)->setName("标准流速");
            ui->MyCustomPlot->graph(iGraphIndex)->setPen(QPen(Qt::blue));
        }
    }

















    //设置X轴文字标注
    ui->MyCustomPlot->xAxis->setLabel("time");


    //设置Y轴文字标注
    ui->MyCustomPlot->yAxis->setLabel("temp/shidu");

    //设置X轴坐标范围
    //ui->MyCustomPlot->xAxis->setRange(-20,20);

    //设置Y轴坐标范围
    // ui->MyCustomPlot->yAxis->setRange(-1100,1100);

    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->MyCustomPlot->axisRect()->setupFullAxesBox();


    //QFont Ft("Microsoft YaHei");
    //Ft.setPointSize(12);

    //ui->MyCustomPlot->addGraph();
    //ui->MyCustomPlot->graph(0)->setName(tr("Name"));

    ui->MyCustomPlot->xAxis->setLabel(tr("采样点时序"));
    ui->MyCustomPlot->yAxis->setLabel(tr("天平质量/流量/流速"));
    ui->MyCustomPlot->rescaleAxes(true);
    ui->MyCustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);



    ui->MyCustomPlot->replot();

    //-------------------------------------------------------------------
}

void FSC_MainWindow::plotFresh(void)
{

    ui->MyCustomPlot->legend->setVisible(true);

    //设定右上角图形标注的字体
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    QVector<double> x(0),y(0);

    ui->MyCustomPlot->graph(0)->setData(plotScaleSumTimeX, plotScaleSumValueY);

    ui->MyCustomPlot->graph(1)->setData(x, y);
    ui->MyCustomPlot->graph(2)->setData(x, y);
    ui->MyCustomPlot->graph(3)->setData(x, y);


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

    revdPLC = true;

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

//            FSCLOG << "Socket write hex: skt-" + QString::number(i) + " " + QString::number(sktBufSend[i].size()) + " " + ByteArrayToHexString(sktBufSend[i]);

            //if (i == 0) FSCLOG << "Socket write hex: skt-" + QString::number(i) + " " + QString::number(sktBufSend[i].size()) + " " + ByteArrayToHexString(sktBufSend[i]);


            sktBufSend[i].resize(0);
        }
    }
}

void FSC_MainWindow::showFresh(void)
{
    ui->lineEdit_scale_show->setEnabled(sktConed[SOCKET_SCALE_INDEX]);
    ui->lineEdit_scale_flow->setEnabled(sktConed[SOCKET_SCALE_INDEX]);

    ui->lineEdit_stdFM_sum->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    ui->lineEdit_stdFM_flow->setEnabled(sktConed[SOCKET_PLC_INDEX]);

    ui->lineEdit_FM_1->setEnabled(sktConed[SOCKET_FLOWM1_INDEX]);
    ui->lineEdit_FM_2->setEnabled(sktConed[SOCKET_FLOWM2_INDEX]);
    ui->lineEdit_FM_3->setEnabled(sktConed[SOCKET_FLOWM3_INDEX]);
    ui->lineEdit_FM_4->setEnabled(sktConed[SOCKET_FLOWM4_INDEX]);
    ui->lineEdit_FM_5->setEnabled(sktConed[SOCKET_FLOWM5_INDEX]);
    ui->lineEdit_FM_6->setEnabled(sktConed[SOCKET_FLOWM6_INDEX]);
    ui->lineEdit_FM_7->setEnabled(sktConed[SOCKET_FLOWM7_INDEX]);
    ui->lineEdit_FM_8->setEnabled(sktConed[SOCKET_FLOWM8_INDEX]);
    ui->lineEdit_FM_9->setEnabled(sktConed[SOCKET_FLOWM9_INDEX]);
    ui->lineEdit_FM_10->setEnabled(sktConed[SOCKET_FLOWM10_INDEX]);
    ui->lineEdit_FM_11->setEnabled(sktConed[SOCKET_FLOWM11_INDEX]);
    ui->lineEdit_FM_12->setEnabled(sktConed[SOCKET_FLOWM12_INDEX]);

    ui->lineEdit_FM_1_flow->setEnabled(sktConed[SOCKET_FLOWM1_INDEX]);
    ui->lineEdit_FM_2_flow->setEnabled(sktConed[SOCKET_FLOWM2_INDEX]);
    ui->lineEdit_FM_3_flow->setEnabled(sktConed[SOCKET_FLOWM3_INDEX]);
    ui->lineEdit_FM_4_flow->setEnabled(sktConed[SOCKET_FLOWM4_INDEX]);
    ui->lineEdit_FM_5_flow->setEnabled(sktConed[SOCKET_FLOWM5_INDEX]);
    ui->lineEdit_FM_6_flow->setEnabled(sktConed[SOCKET_FLOWM6_INDEX]);
    ui->lineEdit_FM_7_flow->setEnabled(sktConed[SOCKET_FLOWM7_INDEX]);
    ui->lineEdit_FM_8_flow->setEnabled(sktConed[SOCKET_FLOWM8_INDEX]);
    ui->lineEdit_FM_9_flow->setEnabled(sktConed[SOCKET_FLOWM9_INDEX]);
    ui->lineEdit_FM_10_flow->setEnabled(sktConed[SOCKET_FLOWM10_INDEX]);
    ui->lineEdit_FM_11_flow->setEnabled(sktConed[SOCKET_FLOWM11_INDEX]);
    ui->lineEdit_FM_12_flow->setEnabled(sktConed[SOCKET_FLOWM12_INDEX]);

    ui->lineEdit_scale_show->setText(QString::number(showScaleSum, 'f', 3));
    ui->lineEdit_scale_flow->setText(QString::number(showScaleFlow, 'f', 3));
    ui->lineEdit_stdFM_sum->setText(QString::number(showSTDFMSum, 'f', 3));
    ui->lineEdit_stdFM_flow->setText(QString::number(showSTDFMFlow, 'f', 3));

    ui->lineEdit_FM_1->setText(QString::number(showFMSum[0], 'f', 3));
    ui->lineEdit_FM_2->setText(QString::number(showFMSum[1], 'f', 3));
    ui->lineEdit_FM_3->setText(QString::number(showFMSum[2], 'f', 3));
    ui->lineEdit_FM_4->setText(QString::number(showFMSum[3], 'f', 3));
    ui->lineEdit_FM_5->setText(QString::number(showFMSum[4], 'f', 3));
    ui->lineEdit_FM_6->setText(QString::number(showFMSum[5], 'f', 3));
    ui->lineEdit_FM_7->setText(QString::number(showFMSum[6], 'f', 3));
    ui->lineEdit_FM_8->setText(QString::number(showFMSum[7], 'f', 3));
    ui->lineEdit_FM_9->setText(QString::number(showFMSum[8], 'f', 3));
    ui->lineEdit_FM_10->setText(QString::number(showFMSum[9], 'f', 3));
    ui->lineEdit_FM_11->setText(QString::number(showFMSum[10], 'f', 3));
    ui->lineEdit_FM_12->setText(QString::number(showFMSum[11], 'f', 3));

    ui->lineEdit_FM_1_flow->setText(QString::number(showFMFlow[0], 'f', 3));
    ui->lineEdit_FM_2_flow->setText(QString::number(showFMFlow[1], 'f', 3));
    ui->lineEdit_FM_3_flow->setText(QString::number(showFMFlow[2], 'f', 3));
    ui->lineEdit_FM_4_flow->setText(QString::number(showFMFlow[3], 'f', 3));
    ui->lineEdit_FM_5_flow->setText(QString::number(showFMFlow[4], 'f', 3));
    ui->lineEdit_FM_6_flow->setText(QString::number(showFMFlow[5], 'f', 3));
    ui->lineEdit_FM_7_flow->setText(QString::number(showFMFlow[6], 'f', 3));
    ui->lineEdit_FM_8_flow->setText(QString::number(showFMFlow[7], 'f', 3));
    ui->lineEdit_FM_9_flow->setText(QString::number(showFMFlow[8], 'f', 3));
    ui->lineEdit_FM_10_flow->setText(QString::number(showFMFlow[9], 'f', 3));
    ui->lineEdit_FM_11_flow->setText(QString::number(showFMFlow[10], 'f', 3));
    ui->lineEdit_FM_12_flow->setText(QString::number(showFMFlow[11], 'f', 3));


    ui->lineEdit_setFlowRate->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    ui->lineEdit_setPWM->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    ui->radioButton_setFlowRate->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    ui->radioButton_setPWM->setEnabled(sktConed[SOCKET_PLC_INDEX]);
    if (!ui->radioButton_setFlowRate->isChecked())
    {
        ui->lineEdit_setFlowRate->setEnabled(false);
    }
    if (!ui->radioButton_setPWM->isChecked())
    {
        ui->lineEdit_setPWM->setEnabled(false);
    }

    ui->tbnVOutOpen->setEnabled(revdPLC);
    ui->tbnVOutClose->setEnabled(revdPLC);
    ui->tbnVForwardIn1Open->setEnabled(revdPLC);
    ui->tbnVForwardIn1Close->setEnabled(revdPLC);
    ui->tbnVForwardIn2Open->setEnabled(revdPLC);
    ui->tbnVForwardIn2Close->setEnabled(revdPLC);
    ui->tbnVReverseIn1Open->setEnabled(revdPLC);
    ui->tbnVReverseIn1Close->setEnabled(revdPLC);
    ui->tbnVReverseIn2Open->setEnabled(revdPLC);
    ui->tbnVReverseIn2Close->setEnabled(revdPLC);

    ui->tbnPumpForwardOn->setEnabled(revdPLC);
    ui->tbnPumpForwardOff->setEnabled(revdPLC);
    ui->tbnPumpReverseOn->setEnabled(revdPLC);
    ui->tbnPumpReverseOff->setEnabled(revdPLC);


    ui->lineEdit_plotTime->setText(QString::number(plotLoop * 0.5, 'f', 1) + "s");
    ui->lineEdit_plotTime->raise();

    ui->tbnSysDevCheck->setVisible(false);
    ui->tbnManualCheckDev->setVisible(false);
}

void FSC_MainWindow::skt_read(int i)
{
    sktBufRev[i].append(fsc_global::sktTcp[i]->readAll());
    //sktBufRev[i] = fsc_global::sktTcp[i]->readAll();

    //FSCLOG << "Socket read hex: " + QString::number(i) + " " + QString::number(sktBufRev[i].size()) + " " + ByteArrayToHexString(sktBufRev[i]);
    //if (i == 0) FSCLOG << "Socket read hex: " + QString::number(i) + " " + QString::number(sktBufRev[i].size()) + " " + ByteArrayToHexString(sktBufRev[i]);

    switch (i)
    {
    case SOCKET_SCALE_INDEX:

        //FSCLOG << "Socket read string: " + QString::number(i) + " " + QString::number(sktBufRev[i].size()) + " " + sktBufRev[i];

        showScaleFlow = sktBufRev[i].toDouble() - showScaleSum;
        showScaleSum = sktBufRev[i].toDouble();

        sktBufRev[i].resize(0);
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

    ui->leFlowSpeed_SensorSpan->setText(QString::number(fsc_global::para_ini.at(index).span_ml_per_min));

    ui->checkBox_100SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[9]);
    ui->checkBox_90SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[8]);
    ui->checkBox_80SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[7]);
    ui->checkBox_70SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[6]);
    ui->checkBox_60SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[5]);
    ui->checkBox_50SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[4]);
    ui->checkBox_40SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[3]);
    ui->checkBox_30SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[2]);
    ui->checkBox_20SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[1]);
    ui->checkBox_10SpanCal->setChecked(fsc_global::para_ini.at(index).span10_cal[0]);

    ui->checkBox_100SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[9]);
    ui->checkBox_90SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[8]);
    ui->checkBox_80SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[7]);
    ui->checkBox_70SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[6]);
    ui->checkBox_60SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[5]);
    ui->checkBox_50SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[4]);
    ui->checkBox_40SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[3]);
    ui->checkBox_30SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[2]);
    ui->checkBox_20SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[1]);
    ui->checkBox_10SpanCheck->setChecked(fsc_global::para_ini.at(index).span10_check[0]);

    ui->checkBox_100SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[9]);
    ui->checkBox_90SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[8]);
    ui->checkBox_80SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[7]);
    ui->checkBox_70SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[6]);
    ui->checkBox_60SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[5]);
    ui->checkBox_50SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[4]);
    ui->checkBox_40SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[3]);
    ui->checkBox_30SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[2]);
    ui->checkBox_20SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[1]);
    ui->checkBox_10SpanModify->setChecked(fsc_global::para_ini.at(index).span10_correct[0]);

}


void FSC_MainWindow::on_lineEdit_setFlowRate_editingFinished()
{
    showSetFlowRate = ui->lineEdit_setFlowRate->text().toDouble();

    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));

}

void FSC_MainWindow::on_lineEdit_setFlowRate_textChanged(const QString &arg1)
{
    (void)arg1;

    showSetFlowRate = ui->lineEdit_setFlowRate->text().toDouble();

    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));
}

void FSC_MainWindow::on_lineEdit_setPWM_editingFinished()
{
    showSetPWM = ui->lineEdit_setPWM->text().toInt();

    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));
}

void FSC_MainWindow::on_lineEdit_setPWM_textChanged(const QString &arg1)
{
    (void)arg1;

    showSetPWM = ui->lineEdit_setPWM->text().toInt();

    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));
}



void FSC_MainWindow::on_tbnScaleZero_clicked()
{
    reqScaleZero();

        scaleTestZero = 1;
}

void FSC_MainWindow::on_tbnCalStart_clicked()
{
    calOn = true;
    plotLoop = 0;
}

void FSC_MainWindow::on_tbnCalTermination_clicked()
{
    calOn = false;
    plotLoop = 0;
    plotScaleSumTimeX.clear();
    plotScaleSumValueY.clear();

}

void FSC_MainWindow::on_tbnPoltClear_clicked()
{
    calOn = false;
    plotLoop = 0;
    plotScaleSumTimeX.clear();
    plotScaleSumValueY.clear();
    plotFresh();
}

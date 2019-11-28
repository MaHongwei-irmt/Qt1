/**********************************************************************************************************
Project:FlowCalibration,流量标定系统
Function：主要功能是对流量计内部流量参数C的标定以及小流速时C的系数的标定。
1.首先是通过点击连接来确定通信端口，由于各部件协议不同，所以无法在同一个端口进行，于是采用双端口进行通信，一个端口是
  TTL USART串口电平，另外一个是RS-485电平，电子天平、水阀的控制器和流量计在485总线上，水阀电机转速的控制在TTL USART
  上面，首先通过电子天平的协议来确定485端口，随后通过电机转速协议来确定TTL电平通信端口。
2.然后点击测量按钮，水泵开始运转，同时读取天平示数，天平变化曲线开始绘制，天平读数、累积质量和流速等都会显示。流速设定功能暂时没有实现
  增加和减少可以控制水泵的转速。
3.流量计参数里面是多个流量计的累积参数，后面是流量常数的确定。
4.地址可以修改各个设备的地址，功能暂时未实现。
5.标定功能也暂时未实现，流量计协议暂时没有加入。
**********************************************************************************************************/
#include "FCMainWindow.h"
#include "ui_FCMainWindow.h"

FCMainWindow::FCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    MySerialPort(new QSerialPort),
    MyTimer(new QTimer(this)),
    MyNextSerialPort(new QSerialPort),
    dlgAddress(new FCAddress),
    ui(new Ui::FCMainWindow)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

FCMainWindow::~FCMainWindow()
{
    delete ui;
}
/***********************************************************************************************************
 函数名：InitUI()
 函数功能：UI界面初始化，字体大小，控件图片等
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::InitUI()
{
   #ifdef Developer
      ui->comboBoxComPort->setVisible(true);
   #else
     //ui->comboBoxComPort->setVisible(false);
          ui->comboBoxComPort->setVisible(true);
   #endif

    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(12);

    ui->MyCustomPlot->addGraph();
    ui->MyCustomPlot->graph(0)->setName(tr("Name"));
    ui->MyCustomPlot->xAxis->setLabel(tr("X----采样点序列"));
    ui->MyCustomPlot->yAxis->setLabel(tr("Y----天平质量/g"));
    ui->MyCustomPlot->rescaleAxes(true);
    ui->MyCustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    ui->leCalculateFlow->setFont(Ft);
    ui->leDutyCycle->setFont(Ft);
    ui->leFlowDisplay_1->setFont(Ft);
    ui->leFlowDisplay_2->setFont(Ft);
    ui->leFlowDisplay_3->setFont(Ft);
    ui->leFlowDisplay_4->setFont(Ft);
    ui->leFlowDisplay_5->setFont(Ft);
    ui->leFlowDisplay_6->setFont(Ft);
    ui->leFlowDisplay_7->setFont(Ft);
    ui->leFlowDisplay_8->setFont(Ft);
    ui->leFlowDisplay_9->setFont(Ft);
    ui->leFlowDisplay_10->setFont(Ft);
    ui->leFlowDisplay_11->setFont(Ft);
    ui->leFlowDisplay_12->setFont(Ft);
    ui->leFlowDisplay_13->setFont(Ft);
    ui->leFlowDisplay_14->setFont(Ft);
    ui->leFlowDisplay_15->setFont(Ft);
    ui->leFlowDisplay_16->setFont(Ft);
    ui->leFlowDisplay_17->setFont(Ft);
    ui->leFlowDisplay_18->setFont(Ft);
    ui->leFlowDisplay_19->setFont(Ft);
    ui->leFlowDisplay_20->setFont(Ft);
    ui->leFlowDisplay_21->setFont(Ft);
    ui->leFlowDisplay_22->setFont(Ft);

    ui->lb1->setFont(Ft);
    ui->lb2->setFont(Ft);
    ui->lb3->setFont(Ft);
    ui->lb4->setFont(Ft);
    ui->lb5->setFont(Ft);
    ui->lb6->setFont(Ft);
    ui->lb7->setFont(Ft);
    ui->lb8->setFont(Ft);
    ui->lb9->setFont(Ft);
    ui->lb10->setFont(Ft);
    ui->lb11->setFont(Ft);
    ui->lb12->setFont(Ft);
    ui->lb13->setFont(Ft);
    ui->lb14->setFont(Ft);
    ui->lb15->setFont(Ft);
    ui->lb16->setFont(Ft);
    ui->lb17->setFont(Ft);
    ui->lb18->setFont(Ft);
    ui->lb19->setFont(Ft);
    ui->lb20->setFont(Ft);
    ui->lb21->setFont(Ft);
    ui->lb22->setFont(Ft);


    ui->label_2->setFont(Ft);
    ui->label_7->setFont(Ft);
    ui->label_8->setFont(Ft);
    ui->label_9->setFont(Ft);
    ui->label_4->setFont(Ft);
    ui->label_5->setFont(Ft);
    ui->label_6->setFont(Ft);
    ui->label_11->setFont(Ft);
    ui->label_13->setFont(Ft);

    ui->leScaleDisplay->setFont(Ft);
    ui->leSumQuality->setFont(Ft);

    ComPort=new QLabel();
    ComPort->setText(tr("端口：无"));
    this->ui->statusBar->addPermanentWidget(ComPort);
    BradRate=new QLabel();
    BradRate->setText(tr("波特率：无"));
    this->ui->statusBar->addPermanentWidget(BradRate);

    NextComPort=new QLabel();
    NextComPort->setText(tr("端口：无"));
    this->ui->statusBar->addPermanentWidget(NextComPort);
    NextBradRate=new QLabel();
    NextBradRate->setText(tr("波特率：无"));
    this->ui->statusBar->addPermanentWidget(NextBradRate);

    ui->tbnPlus->setFont(Ft);
    ui->tbnPlus->setAutoRaise(true);
    ui->tbnPlus->setIcon(ImageFilePlus);
    ui->tbnPlus->setIconSize(ImageFilePlus.size());
    ui->tbnPlus->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnSubtract->setFont(Ft);
    ui->tbnSubtract->setAutoRaise(true);
    ui->tbnSubtract->setIcon(ImageFileSubstract);
    ui->tbnSubtract->setIconSize(ImageFileSubstract.size());
    ui->tbnSubtract->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnConnect->setFont(Ft);
    ui->tbnConnect->setAutoRaise(true);
    ui->tbnConnect->setIcon(ImageFileConnectClose);
    ui->tbnConnect->setIconSize(ImageFileConnectClose.size());
    ui->tbnConnect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnMeasure->setFont(Ft);
    ui->tbnMeasure->setAutoRaise(true);
    ui->tbnMeasure->setIcon(ImageFileStart);
    ui->tbnMeasure->setIconSize(ImageFileStart.size());
    ui->tbnMeasure->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnOpen->setFont(Ft);
    ui->tbnOpen->setAutoRaise(true);
    ui->tbnOpen->setIcon(ImageFileClose);
    ui->tbnOpen->setIconSize(ImageFileClose.size());
    ui->tbnOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnClearWave->setFont(Ft);
    ui->tbnClearWave->setAutoRaise(true);
    ui->tbnClearWave->setIcon(ImageFileClearWaves);
    ui->tbnClearWave->setIconSize(ImageFileClearWaves.size());
    ui->tbnClearWave->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnReset->setFont(Ft);
    ui->tbnReset->setAutoRaise(true);
    ui->tbnReset->setIcon(ImageFileReset);
    ui->tbnReset->setIconSize(ImageFileReset.size());
    ui->tbnReset->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnAddress->setFont(Ft);
    ui->tbnAddress->setAutoRaise(true);
    ui->tbnAddress->setIcon(ImageFileAddress);
    ui->tbnAddress->setIconSize(ImageFileAddress.size());
    ui->tbnAddress->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnFlowSpeedCalibration->setFont(Ft);
    ui->tbnFlowSpeedCalibration->setAutoRaise(true);
    ui->tbnFlowSpeedCalibration->setIcon(ImageFileCalibration);
    ui->tbnFlowSpeedCalibration->setIconSize(ImageFileCalibration.size());
    ui->tbnFlowSpeedCalibration->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->leFlowSpeed->setFont(Ft);
    ui->lbC->setFont(Ft);

    statusBar()->setFont(Ft);
    TextMessage+=tr("提示消息：");
    TextMessage+=tr("界面创建成功!");
//    statusBar()->showMessage(TextMessage,1500);
statusBar()->showMessage(TextMessage,0);

    ui->leDutyCycle->setText(tr("1"));

    ConnectTimes=0;
    ConnectFlag=false;
    MeasureFlag=false;
    FirstMeasureFlag=false;


    ConnectNextTimes=0;
    ConnectNextFlag=false;
    WaveNum=0;
    LastQuality=0;
    SumQuality=0;

    CurrentFlowSpeed = 0;
    SetFlowSpeed = 0;
    SetAdjustTime = 0;
    FlowSpeedFlag = false;
}
/***********************************************************************************************************
 函数名：InitConnect()
 函数功能：控件信号和槽函数的连接
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::InitConnect()
{
    connect(ui->tbnConnect,SIGNAL(clicked(bool)),this,SLOT(ConnectComport()));
   connect(MyTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
   connect(MySerialPort,SIGNAL(readyRead()),this,SLOT(ReadRecData()));
   connect(MyNextSerialPort,SIGNAL(readyRead()),this,SLOT(ReadNextRecData()));
   connect(this,SIGNAL(FindNextComport()),this,SLOT(ConnectNextComport()));
   connect(ui->tbnPlus,SIGNAL(clicked(bool)),this,SLOT(DutyCycleUp()));
   connect(ui->tbnSubtract,SIGNAL(clicked(bool)),this,SLOT(DutyCycleDown()));
   connect(ui->tbnOpen,SIGNAL(clicked(bool)),this,SLOT(Switch()));
   connect(ui->tbnMeasure,SIGNAL(clicked(bool)),this,SLOT(Measure()));
   connect(ui->tbnClearWave,SIGNAL(clicked(bool)),this,SLOT(ClearWaves()));
   connect(ui->tbnReset,SIGNAL(clicked(bool)),this,SLOT(Reset()));
   connect(ui->tbnAddress,SIGNAL(clicked(bool)),this,SLOT(SetAddress()));
   connect(dlgAddress,SIGNAL(SendAddress(QVector<int>)),this,SLOT(RecAddress(QVector<int>)));
   //connect(ui->tbnFlowCalibration,SIGNAL(clicked(bool)),this,SLOT(FlowCalibration()));
   connect(ui->tbnFlowSpeedCalibration,SIGNAL(clicked(bool)),this,SLOT(FlowSpeedCalibration()));


     // connect(ui->tbnConnect, SIGNAL(clicked())

}
/***********************************************************************************************************
 函数名：ConnectComport()
 函数功能：485端口的搜索，会自动进行端口的刷新，连续搜索三次无果够自动退出，并提示用户端口找不到
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ConnectComport()
{
    QString PortName;
    static uint8_t PortCount=0;
    if(ConnectTimes<=3)
    {
        if(ui->tbnConnect->text()==tr("连接"))
        {
            if(PortCount>=ui->comboBoxComPort->count())
            {
                 PortCount=0;
                 Refresh();
                 ConnectTimes++;
            }
            ui->comboBoxComPort->setCurrentIndex(PortCount);
            PortName=ui->comboBoxComPort->currentText().split(' ').first();

              ShowMessage("未找到可用的端口",0);
           // QMessageBox::information(this,"提示","这是一个消息框");
            QMessageBox::information(this, QString::number(PortCount), PortName);


            if(MySerialPort->isOpen())
            {
                MySerialPort->close();
                ShowMessage(MySerialPort->portName()+tr("端口已关闭"),1000);
                ShowParameter(tr("无"),tr("无"));
                return;
            }
            else
            {
                MySerialPort->setPortName(PortName);
                PortCount++;
                if(MySerialPort->open(QIODevice::ReadWrite))
                {
                     //ShowMessage(PortName+tr("端口状态:")+QString::number(MySerialPort->isOpen())+tr(" ")+tr("SerialPort Open Successfully!"),3000);
                     MySerialPort->setBaudRate(9600);
                     MySerialPort->setDataBits(QSerialPort::Data8);
                     MySerialPort->setParity(QSerialPort::NoParity);
                     MySerialPort->setStopBits(QSerialPort::OneStop);
                     MySerialPort->setFlowControl(QSerialPort::NoFlowControl);
                     QByteArray Indentifier;
                     Indentifier.resize(2);
                     Indentifier[0]=0x1B;
                     Indentifier[1]=0x70;
                     WriteBytes(Indentifier);
                     MyTimer->start(300);
                     ConnectFlag=true;
                }
                else
                {
                    ShowMessage(PortName+tr("打开失败，端口可能被占用！")+tr("串口状态:")+QString::number(MySerialPort->isOpen()),2000);
                    return;
                }
            }
        }
        else
        {

            MySerialPort->close();
            //ui->tbnConnect->setText(tr("连接"));
            //ui->tbnConnect->setIcon(ImageFileConnectClose);
            PortCount=0;
            ShowMessage(MySerialPort->portName()+tr("已断开"),1500);
            ShowParameter(tr("无"),tr("无"));
            emit FindNextComport();
        }
    }
    else
    {
        MyTimer->stop();
        ConnectFlag=0;
        ConnectTimes=0;
        ShowMessage(tr("PC端口未响应，请检查设备是否插入！"),1000);
        ShowMessage(tr("PC端口未响应，请检查设备是否插入！"), 0);
    }
}
/***********************************************************************************************************
 函数名：ConnectNextComport()
 函数功能：USART-TTL端口的搜索，会自动进行端口的刷新，连续搜索三次无果够自动退出，并提示用户端口找不到
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ConnectNextComport()
{
    QString PortName;
    static uint8_t NextPortCount=0;
    if(ConnectNextTimes<=3)
    {
        if(ui->tbnConnect->text()==tr("连接"))
        {
            if(NextPortCount>=ui->comboBoxComPort->count())
            {
                 NextPortCount=0;
                 Refresh();
                 ConnectNextTimes++;
            }
            ui->comboBoxComPort->setCurrentIndex(NextPortCount);
            PortName=ui->comboBoxComPort->currentText().split(' ').first();
            if(MyNextSerialPort->isOpen())
            {
                MyNextSerialPort->close();
                  ShowMessage(MyNextSerialPort->portName()+tr("端口已关闭"),1000);
                  ShowNextParameter(tr("无"),tr("无"));
                return;
            }
            else
            {
                MyNextSerialPort->setPortName(PortName);
                NextPortCount++;
                if(MyNextSerialPort->open(QIODevice::ReadWrite))
                {
                     //ShowMessage(PortName+tr("端口状态:")+QString::number(MySerialPort->isOpen())+tr(" ")+tr("SerialPort Open Successfully!"),3000);
                     MyNextSerialPort->setBaudRate(9600);
                     MyNextSerialPort->setDataBits(QSerialPort::Data8);
                     MyNextSerialPort->setParity(QSerialPort::NoParity);
                     MyNextSerialPort->setStopBits(QSerialPort::OneStop);
                     MyNextSerialPort->setFlowControl(QSerialPort::NoFlowControl);
                     QByteArray Indentifier;
                     Indentifier.resize(4);
                     Indentifier[0]=0x44;
                     Indentifier[1]=0x30;
                     Indentifier[2]=0x30;
                     Indentifier[3]=0x30;
                     NextWriteBytes(Indentifier);
                     MyTimer->start(300);
                     ConnectNextFlag=true;
                }
                else
                {
                    ShowMessage(PortName+tr("打开失败，端口可能被占用！")+tr("串口状态:")+QString::number(MySerialPort->isOpen()),2000);
                    return;
                }
            }
        }
        else
        {

            MyNextSerialPort->close();
            ui->tbnConnect->setText(tr("连接"));
            ui->tbnConnect->setIcon(ImageFileConnectClose);
            NextPortCount=0;
            ShowMessage(MyNextSerialPort->portName()+tr("已断开"),1500);
            ShowNextParameter(tr("无"),tr("无"));

        }
    }
    else
    {
        MyTimer->stop();
        ConnectNextFlag=false;
        ConnectNextTimes=0;
        ShowMessage(tr("PC端口未响应，请检查设备是否插入！"),1000);
    }
}
/***********************************************************************************************************
 函数名：ShowMessage()
 函数功能：状态栏显示提示消息（左侧）
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ShowMessage(QString Message,uint16_t ShowTime)
{
    statusBar()->showMessage(Message,ShowTime);
}
/***********************************************************************************************************
 函数名：ShowParameter()
 函数功能：状态栏显示提示485端口和波特率参数（右侧）
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ShowParameter(QString ComPortData,QString BradRateData)
{
    ComPort->setText(tr("端口：")+ComPortData);
    this->ui->statusBar->addPermanentWidget(ComPort);
    BradRate->setText(tr("波特率：")+BradRateData);
    this->ui->statusBar->addPermanentWidget(BradRate);
}
/***********************************************************************************************************
 函数名：ShowNextParameter()
 函数功能：状态栏显示提示USART-TTL端口和波特率参数（右侧）
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ShowNextParameter(QString ComPortData,QString BradRateData)
{
    NextComPort->setText(tr("端口：")+ComPortData);
    this->ui->statusBar->addPermanentWidget(NextComPort);
    NextBradRate->setText(tr("波特率：")+BradRateData);
    this->ui->statusBar->addPermanentWidget(NextBradRate);
}
/***********************************************************************************************************
 函数名：Refresh()
 函数功能：端口的刷新
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::Refresh()
{
    QList <QSerialPortInfo> InfoList = QSerialPortInfo::availablePorts();
    ui->comboBoxComPort->clear();
    if(InfoList.isEmpty())
        ShowMessage(tr("未找到可用的端口"),1500);
    else
    {

//        ShowMessage(tr("未找到可用的端口"),1500);

        foreach(QSerialPortInfo Info,InfoList)
            ui->comboBoxComPort->addItem(Info.portName() + ' ' + Info.description());
    }
}
/***********************************************************************************************************
 函数名：WriteBytes(QByteArray Data)
 函数功能：485端口数据的写入
 函数参数：QByteArray Data，写入数据
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::WriteBytes(QByteArray Data)
{
    if(MySerialPort->isOpen())
    {
        MySerialPort->write(Data);
    }
    else
    {
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未打开，不能发送数据！"),2000);
    }
}
/***********************************************************************************************************
 函数名：NextWriteBytes(QByteArray Data)
 函数功能：USART-TTL端口数据的写入
 函数参数：QByteArray Data，写入数据
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::NextWriteBytes(QByteArray Data)
{
    if(MyNextSerialPort->isOpen())
    {
        MyNextSerialPort->write(Data);
    }
    else
    {
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未打开，不能发送数据！"),2000);
    }
}
/***********************************************************************************************************
 函数名：ReadRecData（）
 函数功能：读485端口的数据
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ReadRecData()
{
    while(MySerialPort->waitForReadyRead(10));
    QByteArray RecData=MySerialPort->readAll();  //读取接收数据
    RunCommand(RecData);

}
/***********************************************************************************************************
 函数名：ReadNextRecData（）
 函数功能：读USART-TTL端口的数据
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ReadNextRecData()
{
    while(MyNextSerialPort->waitForReadyRead(10));
    QByteArray RecData=MyNextSerialPort->readAll();  //读取接收数据
    RunNextCommand(RecData);
}
/***********************************************************************************************************
 函数名：TimerUpdate()
 函数功能：定时器更新函数，负责端口的自动搜索和天平示数的读取
 函数参数：QByteArray Data，写入数据
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::TimerUpdate()
{
    if(ConnectFlag)
    {
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未响应!"),1000);
        MySerialPort->close();
        emit ui->tbnConnect->click();
    }
    if(ConnectNextFlag)
    {
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未响应!"),1000);
        MyNextSerialPort->close();
        emit FindNextComport();
    }
    if(MeasureFlag)
    {
        static bool ChangeFlag=true;

        if(ChangeFlag)
        {
            QByteArray Indentifier;
            Indentifier.resize(2);
            Indentifier[0]=0x1B;
            Indentifier[1]=0x70;
            WriteBytes(Indentifier);
        }
        else
        {

        }
        ChangeFlag=!ChangeFlag;
    }
}
/***********************************************************************************************************
 函数名：RunCommand()
 函数功能：处理485端口数据，包括天平、485控制器以及流量计协议（暂时未加入）
 函数参数：QByteArray Data，处理数据
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::RunCommand(QByteArray RecData)
{
    switch (RecData[0]) {
    case 0x20:
    {
       if(ConnectFlag)
       {
           qDebug()<<"连接成功！";
           MyTimer->stop();
           ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口打开成功！"),5000);
           ShowParameter(ui->comboBoxComPort->currentText().split(' ').first(),tr("115200"));
           //ui->tbnConnect->setText(tr("断开"));
           //ui->tbnConnect->setIcon(ImageFileConnectOpen);
           ConnectFlag=false;
           ConnectTimes=0;
           emit FindNextComport();
       }
       else
       {
           double Quality=0;

           if(RecData[3]!=0x20)
           {
               Quality+=(RecData[3]-0x30)*1000;
           }
           if(RecData[4]!=0x20)
           {
               Quality+=(RecData[4]-0x30)*100;
           }
           if(RecData[5]!=0x20)
           {
               Quality+=(RecData[5]-0x30)*10;
           }
           if(RecData[6]!=0x20)
           {
               Quality+=(RecData[6]-0x30);
           }
           if(RecData[8]!=0x20)
           {
               Quality+=(double)(RecData[8]-0x30)/10.0;
           }
           ui->leScaleDisplay->setText(QString::number(Quality));
           ui->MyCustomPlot->graph(0)->addData(WaveNum,Quality);
           ui->MyCustomPlot->replot();
           ui->MyCustomPlot->rescaleAxes(true);
           WaveNum+=1; 
           CurrentFlowSpeed=(Quality-LastQuality)*60;
           ui->leCalculateFlow->setText(QString::number(CurrentFlowSpeed));
           if(FlowSpeedFlag)
           {
               if(SetAdjustTime==5)
               {
                   SetAdjustTime=0;
                   SetFlowSpeed=ui->leFlowSpeed->text().toDouble();
                   if((SetFlowSpeed-CurrentFlowSpeed)>180)
                   {
                       SetDutyCyeleAddValue(15);
                   }
                   else if(((SetFlowSpeed-CurrentFlowSpeed)>120)&((SetFlowSpeed-CurrentFlowSpeed)<180))
                   {
                       SetDutyCyeleAddValue(10);
                   }
                   else if(((SetFlowSpeed-CurrentFlowSpeed)>60)&((SetFlowSpeed-CurrentFlowSpeed)<120))
                   {
                       SetDutyCyeleAddValue(5);
                   }
                   else if(((SetFlowSpeed-CurrentFlowSpeed)>-120)&((SetFlowSpeed-CurrentFlowSpeed)<-60))
                   {
                      SetDutyCyeleAddValue(-5);
                   }
                   else if(((SetFlowSpeed-CurrentFlowSpeed)>-180)&((SetFlowSpeed-CurrentFlowSpeed)<-120))
                   {
                      SetDutyCyeleAddValue(-10);
                   }
                   else if(((SetFlowSpeed-CurrentFlowSpeed)>-180))
                   {
                      SetDutyCyeleAddValue(-15);
                   }
                   else
                   {

                   }
               }
               else
               {
                   SetAdjustTime++;
               }
           }
           if(!FirstMeasureFlag)
           {
               FirstMeasureFlag=false;
               SumQuality+=(Quality-LastQuality);
               ui->leSumQuality->setText(QString::number(SumQuality));
           }
           LastQuality=Quality;
       }
    }
        break;
    case 0x2D:
    {
        if(ConnectFlag)
        {
            qDebug()<<"连接成功！";
            MyTimer->stop();
            ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口打开成功！"),5000);
            ShowParameter(ui->comboBoxComPort->currentText().split(' ').first(),tr("115200"));
            //ui->tbnConnect->setText(tr("断开"));
            //ui->tbnConnect->setIcon(ImageFileConnectOpen);
            ConnectFlag=false;
            ConnectTimes=0;
            emit FindNextComport();
        }
        else
        {
            double Quality=0;

            if(RecData[3]!=0x20)
            {
                Quality+=(RecData[3]-0x30)*1000;
            }
            if(RecData[4]!=0x20)
            {
                Quality+=(RecData[4]-0x30)*100;
            }
            if(RecData[5]!=0x20)
            {
                Quality+=(RecData[5]-0x30)*10;
            }
            if(RecData[6]!=0x20)
            {
                Quality+=(RecData[6]-0x30);
            }
            if(RecData[8]!=0x20)
            {
                Quality+=(double)(RecData[8]-0x30)/10.0;
            }
            Quality=Quality*(-1);
            ui->leScaleDisplay->setText(QString::number(Quality));
            ui->MyCustomPlot->graph(0)->addData(WaveNum,Quality);
            ui->MyCustomPlot->replot();
            ui->MyCustomPlot->rescaleAxes(true);
            WaveNum+=1;
            ui->leCalculateFlow->setText(QString::number((Quality-LastQuality)*60));
            if(!FirstMeasureFlag)
            {
                FirstMeasureFlag=false;
                SumQuality+=(Quality-LastQuality);
                ui->leSumQuality->setText(QString::number(SumQuality));
            }
            LastQuality=Quality;
        }
    }
        break;
    case 0x22:
    {
         if(RecData[2]==0x11)
         {
             ShowMessage(tr("放水成功"),1000);
         }
         else if(RecData[2]==0x12)
         {
             ShowMessage(tr("放水停止"),1000);
         }
    }
        break;
    default:
        break;
    }
}
/***********************************************************************************************************
 函数名：RunNextCommand()
 函数功能：处理USART--TTL端口数据，负责水泵电机的转速控制
 函数参数：QByteArray Data，处理数据
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::RunNextCommand(QByteArray RecData)
{
    switch (RecData[0])
    {
    case 0x44:
    {
        if(ConnectNextFlag==true)
        {
            qDebug()<<"连接成功！";
            MyTimer->stop();
            ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口打开成功！"),5000);
            ShowNextParameter(ui->comboBoxComPort->currentText().split(' ').first(),tr("115200"));
            ui->tbnConnect->setText(tr("断开"));
            ui->tbnConnect->setIcon(ImageFileConnectOpen);
            ConnectNextFlag=false;
            ConnectNextTimes=0;
            DutyCycleDown();
        }
        else
        {
            ShowMessage(tr("参数设置成功"),1000);
        }
    }
        break;
    default:
        break;
    }
}
/***********************************************************************************************************
 函数名：DutyCycleUp()
 函数功能：电机转速占空比的控制，增加
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::DutyCycleUp()
{
    QByteArray Indentifier;
    qint8 DutyCycle = ui->leDutyCycle->text().toInt();
    DutyCycle+=1;
    if(DutyCycle>99)
        DutyCycle=99;
    ui->leDutyCycle->setText(QString::number(DutyCycle));
    Indentifier[0]=0x44;
    Indentifier[1]=0x30+DutyCycle/100;
    Indentifier[2]=0x30+DutyCycle/10;
    Indentifier[3]=0x30+DutyCycle%10;
    NextWriteBytes(Indentifier);
}
/***********************************************************************************************************
 函数名：DutyCycleDown()
 函数功能：电机转速占空比的控制，减少
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::DutyCycleDown()
{
    QByteArray Indentifier;
    qint8 DutyCycle = ui->leDutyCycle->text().toInt();
    DutyCycle-=1;
    if(DutyCycle<1)
      DutyCycle=1;
    ui->leDutyCycle->setText(QString::number(DutyCycle));

    Indentifier[0]=0x44;
    Indentifier[1]=0x30+DutyCycle/100;
    Indentifier[2]=0x30+DutyCycle/10;
    Indentifier[3]=0x30+DutyCycle%10;
    NextWriteBytes(Indentifier);
}
/***********************************************************************************************************
 函数名：Switch()
 函数功能：水阀的开端控制，通过485总线控制
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::Switch()
{
    QByteArray Indentifier;
    Indentifier.resize(8);
    Indentifier[0]=0x33;
    Indentifier[1]=0x00;

    Indentifier[3]=0x00;
    Indentifier[4]=0x00;
    Indentifier[5]=0x00;
    if(ui->tbnOpen->text()==tr("放水"))
    {
        Indentifier[2]=0x12;
        Indentifier[6]=0x01;
        Indentifier[7]=0x46;
        ui->tbnOpen->setText(tr("停止"));
        ui->tbnOpen->setIcon(ImageFileOpen);
    }
    else if(ui->tbnOpen->text()==tr("停止"))
    {
        Indentifier[2]=0x11;
        Indentifier[6]=0x01;
        Indentifier[7]=0x45;
        ui->tbnOpen->setText(tr("放水"));
        ui->tbnOpen->setIcon(ImageFileClose);
    }
    WriteBytes(Indentifier);
}
/***********************************************************************************************************
 函数名：Measure()
 函数功能：测量按钮槽函数，负责电机水泵转速控制
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::Measure()
{
     QByteArray Indentifier;
     qint8 DutyCycle = ui->leDutyCycle->text().toInt();
     if(ui->tbnMeasure->text()==tr("测量"))
     {
         ui->tbnMeasure->setText(tr("停止"));
         ui->tbnMeasure->setIcon(ImageFileStop);
         Indentifier[0]=0x44;
         Indentifier[1]=0x30+DutyCycle/100;
         Indentifier[2]=0x30+DutyCycle/10;
         Indentifier[3]=0x30+DutyCycle%10;
         NextWriteBytes(Indentifier);
         MeasureFlag=true;
         MyTimer->start(500);
         FirstMeasureFlag=true;
     }
     else
     {
         ui->tbnMeasure->setText(tr("测量"));
         ui->tbnMeasure->setIcon(ImageFileStart);
         DutyCycle=1;
         Indentifier[0]=0x44;
         Indentifier[1]=0x30+DutyCycle/100;
         Indentifier[2]=0x30+DutyCycle/10;
         Indentifier[3]=0x30+DutyCycle%10;
         NextWriteBytes(Indentifier);
         MeasureFlag=false;
         MyTimer->stop();
     }
}
/***********************************************************************************************************
 函数名：ClearWaves()
 函数功能：波形的清除
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::ClearWaves()
{
    ui->MyCustomPlot->graph(0)->data()->clear();
    ui->MyCustomPlot->replot();
    ui->leCalculateFlow->setText(tr(""));
    ui->leSumQuality->setText(tr(""));
    ui->leScaleDisplay->setText(tr(""));
    WaveNum=0;
    SumQuality=0;
    LastQuality=0;
}
/***********************************************************************************************************
 函数名：Reset()
 函数功能：电平的清零
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::Reset()
{
    QByteArray Indentifier;
    Indentifier[0]=0x1B;
    Indentifier[1]=0x74;
    WriteBytes(Indentifier);
}
/***********************************************************************************************************
 函数名：SetAddress()
 函数功能：设置相关传感器的地址
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::SetAddress()
{
    dlgAddress->exec();
}
void FCMainWindow::RecAddress(QVector<int> TempAddress)
{
    FlowAddress=TempAddress;
    qDebug("%d",FlowAddress[0]);
    qDebug("%d",FlowAddress[1]);
    qDebug("%d",FlowAddress[2]);
    qDebug("%d",FlowAddress[3]);
    qDebug("%d",FlowAddress[4]);
    qDebug() << FlowAddress[1];
//    qDebug<<FlowAddress[2];
//    qDebug<<FlowAddress[3];
//    qDebug<<FlowAddress[4];
}
void FCMainWindow::FlowCalibration()
{

}
/***********************************************************************************************************
 函数名：SetDutyCyeleAddValue()
 函数功能：设置水泵电机占空比值
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::SetDutyCyeleAddValue(int AddValue)
{
    QByteArray Indentifier;
    qint8 DutyCycle = ui->leDutyCycle->text().toInt();
    DutyCycle+=AddValue;
    if(DutyCycle<1)
      DutyCycle=1;
    else if(DutyCycle>99)
      DutyCycle=99;
    ui->leDutyCycle->setText(QString::number(DutyCycle));

    Indentifier[0]=0x44;
    Indentifier[1]=0x30+DutyCycle/100;
    Indentifier[2]=0x30+DutyCycle/10;
    Indentifier[3]=0x30+DutyCycle%10;
    NextWriteBytes(Indentifier);
}
/***********************************************************************************************************
 函数名：FlowSpeedCalibration()
 函数功能：流量标定系统流速标定按钮槽函数，负责流速的标定。
 函数参数：无
 函数返回值：无
************************************************************************************************************/
void FCMainWindow::FlowSpeedCalibration()
{
    MyTimer->start(500);
    FlowSpeedFlag=true;
    MeasureFlag=true;
}

//void FCMainWindow::on_comboBoxComPort_editTextChanged(const QString &arg1)
//{

//}

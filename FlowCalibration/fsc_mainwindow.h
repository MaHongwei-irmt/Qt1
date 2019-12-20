#ifndef FSC_MAINWINDOW_H
#define FSC_MAINWINDOW_H

#include <QMainWindow>
#include "dialog_checkdev.h"
#include <QtNetwork>
#include <QSignalMapper>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPrinter>

QByteArray  HexStringToByteArray(QString HexString);
QString     ByteArrayToHexString(QByteArray &ba);
uint16_t    Checksum_computeChecksum(const char *buffer, int length);

namespace Ui {
class FSC_MainWindow;
}

#define FSCLOG  qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")

#define SCALE_MAX_LIMITED  4000

#define FLOWMETER_NUMBER    12

#define SOCKET_NUMBER   17

#define FM_STATION_ADDRESS      4

#define SOCKET_PLC_INDEX        0
#define SOCKET_SCALE_INDEX      1
#define SOCKET_STD_FLOWM_INDEX  2

#define SOCKET_FLOWM1_INDEX     3
#define SOCKET_FLOWM2_INDEX     4
#define SOCKET_FLOWM3_INDEX     5
#define SOCKET_FLOWM4_INDEX     6
#define SOCKET_FLOWM5_INDEX     7
#define SOCKET_FLOWM6_INDEX     8
#define SOCKET_FLOWM7_INDEX     9
#define SOCKET_FLOWM8_INDEX     10
#define SOCKET_FLOWM9_INDEX     11
#define SOCKET_FLOWM10_INDEX    12
#define SOCKET_FLOWM11_INDEX    13
#define SOCKET_FLOWM12_INDEX    14

#define SOCKET_TCP_RETRY_CON_TIMEOUT    5

#define DATA_WRITE_OK   1
#define DATA_READ_OK    2
#define DATA_TIMEOUT      3
#define DATA_READ_TIMEOUT      3

#define FM_REQ_TIMEOUT      3

#define CAL_STATE_STOP          0
#define CAL_START               1
#define CAL_START_BALANCE       2
#define CAL_SCALE_ZERO          3
#define CAL_SCALE_ZERO_BALANCE  4
#define CAL_OPEN_VALVE          5
#define CAL_OPEN_PUMP           6
#define CAL_PLOT_START          7

#define CAL_START_OUT_VALVE_OPEN                11
#define CAL_START_FIRST_FORWARD_FILL_WATER      12
#define CAL_START_FIRST_REVERSE_FILL_WATER      13
#define CAL_PROCESS                             14
#define CAL_PROCESS_END                         15
#define CAL_WAIT_WATER_EMPTY                    16

#define ONE_CAL_EMPTY                       0
#define ONE_CAL_START                       1
#define ONE_CAL_START_END                   2
#define ONE_CAL_GOING                       3
#define ONE_CAL_POST_PROCESS                4

#define MAX_SPAN            10000
#define MIN_SPAN            5000

#define PLOT_VALUE_NUMBER   60 * 2 * 2  // 2min / 500ms

#define CAL_CURRENT_STAT_NEED_EXECUTE   1
#define CAL_CURRENT_STAT_DOING          2
#define CAL_CURRENT_STAT_DONE           4

#define CAL_INFO_MAX_STR_NUM            50000

#define START_CAL_DIRECT_FORWARD        1
#define START_CAL_DIRECT_REVERSE        2
#define START_CAL_TYPE_CAL          1
#define START_CAL_TYPE_CORRECT      2
#define START_CAL_TYPE_CHECK        3

#define SPAN_NUMBER                 10

#define PRINT_PLC_STATE_WRITE       QString::number(plcStateWrite, 16) + " " + QString::number(plcStateWrite, 2)
#define PRINT_PLC_STATE_READ        QString::number(plcStateRead, 16) + " " + QString::number(plcStateRead, 2)

#define MAIN_LOOP_CYCLE             250
#define MS_500                      500
#define MS_1S                       1000

#define POLL_SCALE_CYCLE            (MAIN_LOOP_CYCLE * 3)
#define POLL_FM_CYCLE               (MAIN_LOOP_CYCLE * 2)
#define POLL_PLC_CYCLE              (MAIN_LOOP_CYCLE * 2)

#define VALVE_EXCHANGE_DELAY        1500
#define PUMP_START_DELAY            2000

#define CAL_MAX_STEP                60

#define RECORD_FILE_TO_BE_CONTINUE  "未完. 后续步骤进行中..."

class calLink
{
public:
    int     stepNum = 0;

    int     direct = 0;
    int     type = 0;
    int     spanPercent = 0;
    double  span = 0;


};

class calStep
{
public:

    QString type_name;

    double  span_ml_per_min;

    int     spanCal[SPAN_NUMBER];
    int     spanCheck[SPAN_NUMBER];
    int     spanCorrect[SPAN_NUMBER];

    int     spanCalReverse[SPAN_NUMBER];
    int     spanCheckReverse[SPAN_NUMBER];
    int     spanCorrectReverse[SPAN_NUMBER];

    int     stepTotal;
    int     stepCurrent;

    int     startDirect;
    int     startType;
    int     startSpanPercent;
    double  startSpan;

};

class oneCalTag
{
public:

    bool    pause = false;

    char m_padding [3];

    int     state = ONE_CAL_EMPTY;
    int     step = 0;

    int     stepRecord = 0;

    double  calSpan = 0;
    int     calSpanPercent = 0;
    int     calDirect = 0;
    int     calTpye = 0;
    uint    calTime = 0;

    int     plotPos = 0;
    int     plotSelectedFMIndex = 0;
    QString plotSelectedFMStr = "";

    QVector<double> plotTimeX;
    QVector<double> plotScaleSumValue;
    QVector<double> plotSTDFMSumValue;
    QVector<double> plotSTDFMRateValue;
    QVector<double> plotFMSumValue[FLOWMETER_NUMBER];
    QVector<double> plotFMRateValue[FLOWMETER_NUMBER];

    char m_padding2 [4];

    double  finalScaleSumValue;
    double  finalSTDFMSumValue;
    double  finalSTDFMRateValue;
    double  finalFMSumValue[FLOWMETER_NUMBER];
    double  finalFMRateValue[FLOWMETER_NUMBER];

    calStep     stepBak;
    QDateTime   calDateTime;
    uint        calProcStartTime = 0;
    uint        calProcEndTime = 0;
};

class FSC_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FSC_MainWindow(QWidget *parent = nullptr);
    ~FSC_MainWindow();

    void socketParaInit(void);
    void ParaInit(void);
    void delayMSec(int msec);
    void SocketConnectTry(void);

    bool            showInfoUpdata = true;
    QTextBrowser    *txtBrow_showInfo = nullptr;
    bool            showCalTable = false;
    QString         calTableName = "";
    Qt::CheckState  autoPrinter = Qt::Unchecked;
    bool            autoPrinterBool = false;
    QString         printerName = "";

private slots:
    void skt_connect_suc(int i);
    void skt_connect_dis(int i);
    void skt_error(int i);
    void skt_read(int i);
    void startUp();
    void mainLoop();
    void startCal(void);
    void startSTFM(void);
    void startSocketConnect(int i);

    void on_tbnSysDevCheck_clicked();

    void on_tbnManualCheckDev_clicked();

    void on_comboBox_PlotSenSel_currentIndexChanged(const QString &arg1);

    void on_comboBox_SensorTypeName_currentIndexChanged(int index);

    void on_lineEdit_setPWM_textChanged(const QString &arg1);

    void on_lineEdit_setFlowRate_textChanged(const QString &arg1);

    void on_tbnScaleZero_clicked();

    void on_tbnCalStart_clicked();

    void on_tbnCalTermination_clicked();

    void on_tbnPoltClear_clicked();

    void on_pushButton_debugReturn_clicked();

    void buttonDebug_clicked(int i);

    void on_pushButton_debugOff_clicked();

    void on_tbnCalStepNext_clicked();

    void on_tbnModifyFMTypePara_clicked();

    void on_tbnCalStepPre_clicked();

    void on_tbnCalPause_clicked();

    void on_tbnVOutOpen_clicked();

    void on_tbnVForwardIn1Open_clicked();

    void on_tbnVForwardIn2Open_clicked();

    void on_tbnVReverseIn1Open_clicked();

    void on_tbnVReverseIn2Open_clicked();

    void on_tbnPump1ForwardOn_clicked();

    void on_tbnPump1ReverseOn_clicked();

    void on_tbnVOutClose_clicked();

    void on_tbnVForwardIn1Close_clicked();

    void on_tbnVForwardIn2Close_clicked();

    void on_tbnVReverseIn1Close_clicked();

    void on_tbnVReverseIn2Close_clicked();

    void on_tbnPump1ForwardOff_clicked();

    void on_tbnPump1ReverseOff_clicked();

    void on_tbnParaAdd_clicked();

    void on_tbnParaErase_clicked();

    void on_tbnPump2ForwardOn_clicked();

    void on_tbnPump2ReverseOn_clicked();

    void on_tbnPump2ForwardOff_clicked();

    void on_tbnPump2ReverseOff_clicked();

    void on_radioButton_setFlowRate_clicked();

    void on_radioButton_setPWM_clicked();

    void on_tbnCalManual_clicked();

    void on_tbnFMCalTable_clicked();

    void on_pushButton_showInfo_clicked();

    void on_textBrow_calInfo_textChanged();

private:
    Ui::FSC_MainWindow *ui;

    void uiReInit(void);

    void paraWrite(void);
    void PlotInit(void);
    void DataInit(void);
    void dataInit_calStepInit(void);
    void calStepInfoFresh(void);
    void calStepInfoFreshOnUI(void);

    void calRunLink_calStepInfoInit(void);
    int  stepCal_dir_type_span(int *dir, int *type, int *spanPercent, double *span, int stepNum);
    void SocketDataInit(void);
    void SocketConnect(void);

    int  startCal_dir_type_span(int *dir, int *type, int *spanPercent, double *spanCal);

    void PlotReplay(const QString &arg1);

    void socketCommunication(void);
    void reqFMSumRateMsg(QByteArray *buf, int station);
    bool preParseFMMsg(int indexSkt);
    bool parseFMSumRateMsg(int indexSkt);
    bool parsePLC(int indexSkt);
    bool parsePLCNoSTFM(int indexSkt);

    void reqScaleShow(void);
    void reqScaleZero(void);
    void reqFMData(int indexFM);
    void reqSetPLC(void);
    void reqSetPLCWithSTFM(void);
    void writePLC(void);
    bool checkPlc(void);

    void flushSendBuf(void);
    void showFresh(void);
    void showPlotFresh(void);
    void showPlcFresh(void);

    bool checkWaterEmpty(void);
    bool calProc(void);
    bool fillOneCal(oneCalTag *calTag);
    void calSingle(oneCalTag *calTag);
    void calGoing(oneCalTag *calTag);
    void calPlot(oneCalTag *calTag);
    void calDoing(oneCalTag *calTag);
    void calStop(oneCalTag *calTag);
    void calFaultStop(oneCalTag *calTag);

    void calGoingInfoLab(oneCalTag *calTag);
    void calGoingInfoLabRemove(void);

    void makeCalRecordPrint(oneCalTag *calTag);

    void plotAddDataAndFresh(void);
    void plotFresh(void);

    void printInfo(QString str);
    void printInfoWithTime(QString str);

    void plcDataInit(void);
    void openForwardValveAll(void);
    void openReverseValveAll(void);
    void openForwardValve1(void);
    void openForwardValve2(void);
    void openReverseValve1(void);
    void openReverseValve2(void);
    void openOutValve(void);
    void closeForwardValveAll(void);
    void closeReverseValveAll(void);
    void closeForwardValve1(void);
    void closeForwardValve2(void);
    void closeReverseValve1(void);
    void closeReverseValve2(void);
    void closeOutValve(void);
    void pump1On(void);
    void pump1Off(void);
    void pump2On(void);
    void pump2Off(void);
    bool statePump1(void);
    bool statePump2(void);
    bool stateForwardV1(void);
    bool stateForwardV2(void);
    bool stateReverseV1(void);
    bool stateReverseV2(void);

    bool        sktConed[SOCKET_NUMBER];
    QByteArray  sktBufSend[SOCKET_NUMBER];
    QByteArray  sktBufRev[SOCKET_NUMBER];
    bool        sktRespondOk[SOCKET_NUMBER];
    uint        sktReqTime[SOCKET_NUMBER];

    int         sktDataState[SOCKET_NUMBER];
    uint        sktDataWriteTime[SOCKET_NUMBER];

    bool            debugSkt[SOCKET_NUMBER];
    QPushButton     *buttonDebug[SOCKET_NUMBER];
    QSignalMapper   *buttonDebugMapper;

    QSignalMapper * sktConMapper;
    QSignalMapper * sktDisconMapper;
    QSignalMapper * sktErrMapper;
    QSignalMapper * sktReadMapper;

    QTimer  *mainLoopTimer = nullptr;
    QTimer  *startUpTimer = nullptr;
    QTimer  *calTimer = nullptr;
    QTimer  *STFMTimer = nullptr;
    int     pollCycleSTFM = 250;

    bool    socketWellDone  =   false;

    uint    sktConCommandTime[SOCKET_NUMBER];

    char    stationSTDFM;
    char    stationFM[FLOWMETER_NUMBER];

    double  showScaleSum;
    double  showScaleFlow;
    double  showSTDFMSum;
    double  showSTDFMFlow;
    double  showFMSum[FLOWMETER_NUMBER];
    double  showFMFlow[FLOWMETER_NUMBER];

    double  showSetFlowRate;
    int     showSetPWM;
    bool    flowRateOrPWM = true;

    bool    revdSketPLC;

    QVector<double> plotScaleSumTimeX;
    QVector<double> plotScaleSumValueY;

    QVector<double> plotSTDFMSumTimeX;
    QVector<double> plotSTDFMSumValueY;

    QVector<double> plotSTDFMFlowTimeX;
    QVector<double> plotSTDFMFlowValueY;

    QVector<double> plotFMSumTimeX;
    QVector<double> plotFMSumValueY;

    QVector<double> plotFMFlowTimeX;
    QVector<double> plotFMFlowValueY;

    int     plotPosNumber = PLOT_VALUE_NUMBER;
    int     plotLoop;
    int     calOn;
    uint    calOnTime;

    calStep currentStep;

    bool    scaleTestZero;

    bool        firstCal = true;
    oneCalTag   oneCal;
    oneCalTag   allCal[CAL_MAX_STEP];
    bool        allCalNeedToReport = false;
    bool        allCalAvailable[CAL_MAX_STEP];
    bool        allCalEnd = false;

    bool        calManualDoing   =   false;

    QVector<calLink> calRunLink;

    QCheckBox   *checkBox_spanCal[SPAN_NUMBER];
    QCheckBox   *checkBox_spanCheck[SPAN_NUMBER];
    QCheckBox   *checkBox_spanCorrect[SPAN_NUMBER];
    QCheckBox   *checkBox_spanCalReverse[SPAN_NUMBER];
    QCheckBox   *checkBox_spanCheckReverse[SPAN_NUMBER];
    QCheckBox   *checkBox_spanCorrectReverse[SPAN_NUMBER];

    QLineEdit   *lineEdit_FMSum[FLOWMETER_NUMBER];
    QLineEdit   *lineEdit_FMFlow[FLOWMETER_NUMBER];

    uint16_t    plcStateWrite = 0xffff;   // 泵一 泵二 bit6 放水阀 反向进水阀2 反向进水阀1 正向进水阀2 正向进水阀1 :靠近泵为1#阀
    uint16_t    plcStateRead = 0;
    int         plcRWErr = 0;
    double      plcRevFlowRate;
    uint16_t    plcRevPWM;
    uint16_t    plcPidPWM = 0;
};

class fsc_para_ini
{
public:

    QString type_name;

    double span_ml_per_min;

    bool    spanCal[SPAN_NUMBER];
    bool    spanCheck[SPAN_NUMBER];
    bool    spanCorrect[SPAN_NUMBER];
    bool    spanCalReverse[SPAN_NUMBER];
    bool    spanCheckReverse[SPAN_NUMBER];
    bool    spanCorrectReverse[SPAN_NUMBER];

    QVector <uint> new_cal_pos;
    QVector <uint> new_check_pos;
    QVector <uint> new_correct_pos;

};

class fsc_global
{
public:

    static QVector <fsc_para_ini> para_ini;

    static QTcpSocket *sktTcp[SOCKET_NUMBER];
    static QString ip_PLC;
    static QString ip_RS_Server;
    static quint16 port_number[SOCKET_NUMBER];
    static QString ip[SOCKET_NUMBER];
};

#endif // FSC_MAINWINDOW_H

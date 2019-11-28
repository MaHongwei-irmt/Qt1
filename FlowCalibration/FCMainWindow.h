#ifndef FCMAINWINDOW_H
#define FCMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPixmap>
#include <QTimer>
#include <FCAddress.h>
namespace Ui {
class FCMainWindow;
}

class FCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FCMainWindow(QWidget *parent = 0);
    ~FCMainWindow();
    void InitUI();
    void InitConnect();
    void ShowMessage(QString,uint16_t);
    void ShowParameter(QString,QString);
    void Refresh();
    void WriteBytes(QByteArray);
    void RunCommand(QByteArray);
    void ShowNextParameter(QString,QString);
    void RunNextCommand(QByteArray);
    void NextWriteBytes(QByteArray);
    void SetDutyCyeleAddValue(int);

signals:
    void FindNextComport();
private slots:
    void ConnectComport();
    void ReadRecData();
    void TimerUpdate();
    void ConnectNextComport();
    void ReadNextRecData();
    void DutyCycleUp();
    void DutyCycleDown();
    void Switch();
    void Measure();
    void ClearWaves();
    void Reset();
    void SetAddress();
    void RecAddress(QVector<int>);
    void FlowCalibration();
    void FlowSpeedCalibration();
//    void on_comboBoxComPort_editTextChanged(const QString &arg1);

private:
    Ui::FCMainWindow *ui;

    QSerialPort     *MySerialPort     = nullptr;
    QSerialPort     *MyNextSerialPort = nullptr;
    QTimer           *MyTimer         = nullptr;

    FCAddress *dlgAddress   = nullptr;

    QLabel *BradRate;
    QLabel *ComPort;

    QLabel *NextBradRate;
    QLabel *NextComPort;

    QString TextMessage;
    quint8 ConnectTimes;
    bool ConnectFlag;

    bool MeasureFlag;
    bool FirstMeasureFlag;

    quint32 WaveNum;
    double LastQuality;
    double SumQuality;
    quint8 ConnectNextTimes;
    bool ConnectNextFlag;
    QVector<int> FlowAddress;

    double CurrentFlowSpeed;
    double SetFlowSpeed;
    quint8 SetAdjustTime;
    bool FlowSpeedFlag;

    QPixmap ImageFileOpen=tr(":/Image/Open.png");
    QPixmap ImageFileClose=tr(":/Image/Close.png");
    QPixmap ImageFileStart=tr(":/Image/Stop.png");
    QPixmap ImageFileStop=tr(":/Image/Start.png");
    QPixmap ImageFileConnectOpen=tr(":/Image/ConnectOpen.png");
    QPixmap ImageFileConnectClose=tr(":/Image/ConnectClose.png");
    QPixmap ImageFilePlus=tr(":/Image/Plus.png");
    QPixmap ImageFileSubstract=tr(":/Image/Substract.png");
    QPixmap ImageFileClearWaves=tr(":/Image/ClearWaves.png");
    QPixmap ImageFileReset=tr(":/Image/Reset.png");
    QPixmap ImageFileAddress=tr(":/Image/Address.png");
    QPixmap ImageFileCalibration=tr(":/Image/Calibration.png");
};

#endif // FCMAINWINDOW_H

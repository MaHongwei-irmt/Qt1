#include "dialog_fmdata.h"
#include "ui_dialog_fmdata.h"
#include "fsc_mainwindow.h"
#include "dialog_sensoraddr.h"

Dialog_fmdata::Dialog_fmdata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_fmdata)
{
    ui->setupUi(this);

    this->setWindowTitle("传感器参数");

    int x = 85;
    int y = 143;
    int interval = 58;

    int nPrecision = 3;

    int intervalToKf2 = 90;

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        lineEdit_SET_KF1[i] = new QLineEdit(this);
        lineEdit_SET_KF1[i]->setFixedSize(55, 20);

        lineEdit_SET_KF1[i]->move(x + i * interval, y);
        lineEdit_SET_KF1[i]->show();

        label_SET_KF1[i] = new QLabel(this);

        label_SET_KF1[i]->setFixedSize(30, 20);

        label_SET_KF1[i]->move(x + i * interval + 5, y - 25);
        label_SET_KF1[i]->show();
        label_SET_KF1[i]->setText(QString().sprintf("%d", i - 9));



        pDfValidator[i] = new QDoubleValidator(0.001, 900.000, nPrecision, lineEdit_SET_KF1[i]);
        pDfValidator[i]->setNotation(QDoubleValidator::StandardNotation);

        lineEdit_SET_KF1[i]->setValidator(pDfValidator[i]);
        lineEdit_SET_KF1[i]->setMaxLength(7);



        lineEdit_SET_KF2[i] = new QLineEdit(this);
        lineEdit_SET_KF2[i]->setFixedSize(55, 20);

        lineEdit_SET_KF2[i]->move(x + i * interval, y + intervalToKf2);
        lineEdit_SET_KF2[i]->show();

        label_SET_KF2[i] = new QLabel(this);

        label_SET_KF2[i]->setFixedSize(30, 20);

        label_SET_KF2[i]->move(x + i * interval + 5, y - 25 + intervalToKf2);
        label_SET_KF2[i]->show();
        label_SET_KF2[i]->setText(QString().sprintf("%d", i - 9));

        lineEdit_SET_KF2[i]->setValidator(pDfValidator[i]);
        lineEdit_SET_KF2[i]->setMaxLength(7);

     }

    for (int i = 0; i < XUNYIN_SET_FLOW_RANGE_NUM; i++)
    {
        lineEdit_FLOW_RANGE[i] = new QLineEdit(this);
        lineEdit_FLOW_RANGE[i]->setFixedSize(55, 20);

        lineEdit_FLOW_RANGE[i]->move(x + i * interval, y + intervalToKf2 * 2);
        lineEdit_FLOW_RANGE[i]->show();

        label_FLOW_RANGE[i] = new QLabel(this);

        label_FLOW_RANGE[i]->setFixedSize(30, 20);

        label_FLOW_RANGE[i]->move(x + i * interval + 5, y - 25 + intervalToKf2 * 2);
        label_FLOW_RANGE[i]->show();
        label_FLOW_RANGE[i]->setText(QString().sprintf("%d", i + 1));

        lineEdit_FLOW_RANGE[i]->setValidator(pDfValidator[i]);
        lineEdit_FLOW_RANGE[i]->setMaxLength(7);
    }

    ui->label_fmName->setText(QString().sprintf("%d#流量传感器参数", fmIdx));

    for (int i = 0; i < XUNYIN_SET_ZERO_CAL_NUM; i++)
    {
        lineEdit_ZERO_CAL[i] = new QLineEdit(this);
        lineEdit_ZERO_CAL[i]->setFixedSize(55, 20);

        lineEdit_ZERO_CAL[i]->move(x + i * interval, y + intervalToKf2 * 3);
        lineEdit_ZERO_CAL[i]->show();

        label_ZERO_CAL[i] = new QLabel(this);

        label_ZERO_CAL[i]->setFixedSize(30, 20);

        label_ZERO_CAL[i]->move(x + i * interval + 5, y - 25 + intervalToKf2 * 3);
        label_ZERO_CAL[i]->show();
        label_ZERO_CAL[i]->setText(QString().sprintf("%d", i + 1));

        lineEdit_ZERO_CAL[i]->setValidator(pDfValidator[i]);
        lineEdit_ZERO_CAL[i]->setMaxLength(7);
    }

}

Dialog_fmdata::~Dialog_fmdata()
{
    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        delete pDfValidator[i];
    }

    delete ui;
}


void Dialog_fmdata::setFmName(int i)
{
    if (i < 0 || i > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return;
    }

    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    fmIdx = i;
    fmData = mainWin->fmData[i];

    ui->label_fmName->setText(QString().sprintf("%d#流量传感器参数", fmIdx + 1));

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        lineEdit_SET_KF1[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueSET_KF1[i]) ));
        lineEdit_SET_KF2[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueSET_KF2[i]) ));
    }

    for (int i = 0; i < XUNYIN_SET_FLOW_RANGE_NUM; i++)
    {
        lineEdit_FLOW_RANGE[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueFLOW_RANGE[i]) ));
    }

    for (int i = 0; i < XUNYIN_SET_ZERO_CAL_NUM; i++)
    {
        lineEdit_ZERO_CAL[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueZERO_CAL[i]) ));
    }
}

void Dialog_fmdata::on_tbnRead_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    mainWin->dialog_readSET_KF1(fmIdx);

    fmData = mainWin->fmData[fmIdx];

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        lineEdit_SET_KF1[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueSET_KF1[i]) ));
    }
}

void Dialog_fmdata::on_tbnWrite_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        fmData.fm_valueSET_KF1[i] = lineEdit_SET_KF1[i]->text().toFloat();
    }

    mainWin->fmData[fmIdx] = fmData;

    mainWin->dialog_writeSET_KF1(fmIdx);
}

void Dialog_fmdata::closeEvent(QCloseEvent *event)
{
    (void)(*event);

    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    if (mainWin->dialog_fmRegister->isVisible())
    {
        return;
    }

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        mainWin->sktPause[i + SOCKET_FLOWM1_INDEX] = false;
    }

}

void Dialog_fmdata::on_tbnRead_SET_KF2_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    mainWin->dialog_readSET_KF2(fmIdx);

    fmData = mainWin->fmData[fmIdx];

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        lineEdit_SET_KF2[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueSET_KF2[i]) ));
    }
}

void Dialog_fmdata::on_tbnWrite_SET_KF2_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    for (int i = 0; i < XUNYIN_SET_KF_NUM; i++)
    {
        fmData.fm_valueSET_KF2[i] = lineEdit_SET_KF2[i]->text().toFloat();
    }

    mainWin->fmData[fmIdx] = fmData;

    mainWin->dialog_writeSET_KF2(fmIdx);
}

void Dialog_fmdata::on_tbnRead_FLOW_RANHE_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    mainWin->dialog_readFLOW_RANGE(fmIdx);

    fmData = mainWin->fmData[fmIdx];

    for (int i = 0; i < XUNYIN_SET_FLOW_RANGE_NUM; i++)
    {
        lineEdit_FLOW_RANGE[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueFLOW_RANGE[i]) ));
    }
}

void Dialog_fmdata::on_tbnWrite_FLOW_RANHE_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    for (int i = 0; i < XUNYIN_SET_FLOW_RANGE_NUM; i++)
    {
        fmData.fm_valueFLOW_RANGE[i] = lineEdit_FLOW_RANGE[i]->text().toFloat();
    }

    mainWin->fmData[fmIdx] = fmData;

    mainWin->dialog_writeFLOW_RANGE(fmIdx);
}

void Dialog_fmdata::on_tbnRead_ZERO_CAL_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    mainWin->dialog_readZERO_CAL(fmIdx);

    fmData = mainWin->fmData[fmIdx];

    for (int i = 0; i < XUNYIN_SET_ZERO_CAL_NUM; i++)
    {
        lineEdit_ZERO_CAL[i]->setText(QString().sprintf("%0.3f", static_cast<double>(fmData.fm_valueZERO_CAL[i]) ));
    }
}

void Dialog_fmdata::on_tbnWrite_ZERO_CAL_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    for (int i = 0; i < XUNYIN_SET_ZERO_CAL_NUM; i++)
    {
        fmData.fm_valueZERO_CAL[i] = lineEdit_ZERO_CAL[i]->text().toFloat();
    }

    mainWin->fmData[fmIdx] = fmData;

    mainWin->dialog_writeZERO_CAL(fmIdx);
}

void Dialog_fmdata::on_tbnFMRegister_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    Dialog_SensorAddr *fmRegister = static_cast<Dialog_SensorAddr*>(mainWin->dialog_fmRegister);

    fmRegister->show();
    fmRegister->move(fmRegister->geometry().x(), fmRegister->geometry().y() + 100);


    fmRegister->setFmName(fmIdx);
}

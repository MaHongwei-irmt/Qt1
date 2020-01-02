#include "dialog_fmdata.h"
#include "ui_dialog_fmdata.h"
#include "fsc_mainwindow.h"

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
     }

    ui->label_fmName->setText(QString().sprintf("%d#流量传感器参数", fmIdx));

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

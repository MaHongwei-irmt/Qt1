#include "fsc_mainwindow.h"

#include "dialog_checkdev.h"
#include "ui_dialog_checkdev.h"
#include "dialog_sensoraddr.h"


Dialog_CheckDev::Dialog_CheckDev(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_CheckDev)
{
    ui->setupUi(this);
}

Dialog_CheckDev::~Dialog_CheckDev()
{
    delete ui;
}

void Dialog_CheckDev::on_pushButton_12_clicked()
{
    Dialog_SensorAddr *SensorAddr = new Dialog_SensorAddr();

    SensorAddr->exec();
}

void Dialog_CheckDev::on_pushButton_4_clicked()
{



}

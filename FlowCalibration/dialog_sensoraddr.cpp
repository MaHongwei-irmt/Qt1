#include "dialog_sensoraddr.h"
#include "ui_dialog_sensoraddr.h"
#include <QSettings>

Dialog_SensorAddr::Dialog_SensorAddr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SensorAddr)
{
    ui->setupUi(this);

    QSettings *configIni = new QSettings("addr.ini", QSettings::IniFormat);

    model = new QStandardItemModel(this);

    model->setColumnCount(5);

    model->setHeaderData(0, Qt::Horizontal, "寄存器地址");
    model->setHeaderData(1, Qt::Horizontal, "名称     ");
    model->setHeaderData(2, Qt::Horizontal, "类型     ");
    model->setHeaderData(3, Qt::Horizontal, "数据     ");
    model->setHeaderData(4, Qt::Horizontal, "描述     ");

    int i = 1;
    while (configIni->value("sensor_address/address_" + QString::number(i, 10)).toString() != "")
    {
        model->setItem(i - 1, 0, new QStandardItem(configIni->value("sensor_address/address_" + QString::number(i, 10)).toString() + " "));
        model->setItem(i - 1, 1, new QStandardItem(configIni->value("sensor_address/name_" + QString::number(i, 10)).toString() + " "));
        model->setItem(i - 1, 2, new QStandardItem(configIni->value("sensor_address/type_" + QString::number(i, 10)).toString() + " "));
        model->setItem(i - 1, 3, new QStandardItem("数据"));
        model->setItem(i - 1, 4, new QStandardItem(configIni->value("sensor_address/description_" + QString::number(i, 10)).toString() + " "));

        i++;
    }

    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 70);
    ui->tableView->setColumnWidth(4, 200);

    delete configIni;
}

Dialog_SensorAddr::~Dialog_SensorAddr()
{
    delete ui;
}

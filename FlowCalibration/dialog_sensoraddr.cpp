#include "dialog_sensoraddr.h"
#include "ui_dialog_sensoraddr.h"
#include <QSettings>
#include "fsc_mainwindow.h"

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

    rowAll = i - 1;

    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 180);
    ui->tableView->setColumnWidth(2, 140);
    ui->tableView->setColumnWidth(3, 70);
    ui->tableView->setColumnWidth(4, 240);

    delete configIni;

}

Dialog_SensorAddr::~Dialog_SensorAddr()
{
    delete ui;
}


void Dialog_SensorAddr::setFmName(int i)
{
    if (i < 0 || i > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return;
    }

    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    fmIdx = i;
    fmData = mainWin->fmData[i];

    ui->label_fmName->setText(QString().sprintf("%d#流量传感器寄存器", fmIdx + 1));


    QModelIndex index;
    QVariant data;
    QString str;
    int iAddr = 0;


    for (int k = 0; k <= rowAll; k++)
    {


        index = model->index(k, 0);
        data = model->data(index);

        str = data.toString();
        iAddr = str.toInt(Q_NULLPTR,16);

        index = model->index(k, 3);


        switch(iAddr)
        {
        case 0x80:
            data = fmData.fm_0x80_GAP_PLS_ADC_START;

            break;

        case 0x81:
            data = fmData.fm_0x81_NUM_PLS;

            break;

        case 0x82:
            data = fmData.fm_0x82_GAP_UPS_DNS;

            break;

        case 0x83:
            data = fmData.fm_0x83_GAP_UPS_UPS;

            break;

        case 0x84:
            data = fmData.fm_0x84_GAIN_CONTROL;

            break;

        case 0x85:
            data = fmData.fm_0x85_METER_CONSTANT;

            break;

        case 0x86:
            data = fmData.fm_0x86_XT2_FREQ;

            break;

        case 0x87:
            data = fmData.fm_0x87_ADC_SAMP_FREQ;

            break;

        case 0x88:
            data = fmData.fm_0x88_SIG_SAMP_FREQ;

            break;

        case 0x89:
            data = fmData.fm_0x89_ADC_OVERSAMPLING;

            break;

        case 0x8a:
            data = fmData.fm_0x8A_DELTA_TOF_OFFSET;

            break;

        case 0x8b:
            data = fmData.fm_0x8B_ABS_TOF_ADDITIONAL;

            break;

        case 0x8c:
            data = fmData.fm_0x8C_CAPTURE_DURATION;

            break;

        case 0x8d:
            data = fmData.fm_0x8D_PARAM1;

            break;

        case 0x8e:
            data = fmData.fm_0x8E_PARAM2;

            break;

        case 0x8f:
            data = fmData.fm_0x8F_PARAM3;

            break;

        case 0x90:
            data = fmData.fm_0x90_PARAM4;

            break;

        case 0x91:
            data = fmData.fm_0x91_PARAM7;

            break;

        case 0x92:
            data = fmData.fm_0x92_PARAM8;

            break;

        case 0x93:
            data = fmData.fm_0x93_PARAM9;

            break;

        case 0x94:
            data = fmData.fm_0x94_PARAM10;

            break;

        case 0x96:
            data = fmData.fm_0x96_APP_UPDATE_REQ;

            break;

        case 0x97:
            data = fmData.fm_0x97_APP_REQ_DATA[0];

            break;

        case 0x98:
            data = fmData.fm_0x98_APP_REQ_CAPTURES1[0];

            break;

        case 0x99:
            data = fmData.fm_0x99_APP_VERSION;

            break;

        case 0x9b:
            data = fmData.fm_0x9B_F1_F2[0];

            break;

        case 0x9e:
            data = fmData.fm_0x9E_PARAM5;

            break;

        case 0x9f:
            data = fmData.fm_0x9F_PARAM6;

            break;

        case 0xe0:
            data = fmData.fm_0xe0_AUTO_GAIN_ENABLE;

            break;

        case 0xe1:
            data = fmData.fm_0xe1_FLOW_RANGE[0];

            break;

        case 0xe2:
            data = fmData.fm_0xe2_RUNNING_AVG;

            break;

        case 0xe3:
            data = fmData.fm_0xe3_SET_DAC[0];

            break;

        case 0xe4:
            data = fmData.fm_0xe4_SET_KF;

            break;

        case 0xe5:
            data = fmData.fm_0xe5_SET_KF1[0];

            break;

        case 0xe6:
            data = fmData.fm_0xe6_SET_KF2[0];

            break;

        case 0xe7:
            data = fmData.fm_0xe7_SET_SNSR_TYP;

            break;

        case 0xe8:
            data = fmData.fm_0xe8_LNCAL_EN;

            break;

        case 0xe9:
            data = fmData.fm_0xe9_STA_UP;

            break;

        case 0xea:
            data = fmData.fm_0xeA_RESET;

            break;

        case 0xeb:
            data = fmData.fm_0xeB_ZERO_CAL[0];

            break;

        case 0xec:
            data = fmData.fm_0xeC_AUTO_GAIN_UP;

            break;

        case 0xed:
            data = fmData.fm_0xeD_ADDR;

            break;

        case 0xef:
            data = fmData.fm_0xef_APP_REQ_CAPTURES2[0];

            break;

        case 0xf0:
            data = fmData.fm_0xf0_APP_REQ_CAPTURES3[0];

            break;

        case 0xf1:
            data = fmData.fm_0xf1_APP_REQ_CAPTURES4[0];

            break;


        }

        model->setData(index, data);


    }



}


void Dialog_SensorAddr::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}

void Dialog_SensorAddr::on_pushButton_readRegister_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());


    QAbstractItemModel *model = ui->tableView->model ();
    QModelIndex index;
    QVariant data;
    QString str;
    int iAddr = 0;


    mainWin->printInfoWithTime("读取 ");

    index = model->index(row, 0);
    data = model->data(index);

    str = data.toString();
    iAddr = str.toInt(Q_NULLPTR,16);

    mainWin->printInfo(QString().sprintf(" 0x%x ", iAddr));


    index = model->index(row, 1);
    data = model->data(index);

    mainWin->printInfo(data.toString());

    mainWin->printInfo(" ");


    index = model->index(row, 2);
    data = model->data(index);

    mainWin->printInfo(data.toString());

    mainWin->printInfo(" ");


    index = model->index(row, 4);
    data = model->data(index);

    mainWin->printInfo(data.toString());


    switch(iAddr)
    {
    case    0x80:
    case    0x81:
    case    0x82:
    case    0x83:
    case    0x84:
    case    0x86:
    case    0x87:
    case    0x88:
    case    0x89:
    case    0x8c:
    case    0x96:
    case    0x99:
    case    0xe0:
    case    0xe2:
    case    0xe7:
    case    0xe8:
    case    0xe9:
    case    0xea:
    case    0xec:
    case    0xed:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 1);

        break;

    case    0x85:
    case    0x8a:
    case    0x8b:
    case    0x8d:
    case    0x8e:
    case    0x8f:
    case    0x90:
    case    0x9e:
    case    0x9f:
    case    0x91:
    case    0x92:
    case    0x93:
    case    0x94:
    case    0x9b:
    case    0xe3:
    case    0xe4:
    case    0xeb:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 2);

        break;

    case    0xef:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 125);

        break;

    case    0xf1:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 125);

        break;

    case    0xf0:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 125);

        break;

    case    0xe5:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 19);

        break;

    case    0xe6:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 19);

        break;

    case    0xe1:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 10);

        break;

    case    0x98:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 125);

        break;

    case    0x97:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 108);

        break;
    }


    return;
}

void Dialog_SensorAddr::on_pushButton_freshTable_clicked()
{
    setFmName(fmIdx);
}

void Dialog_SensorAddr::on_pushButton_writeRegister_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    QAbstractItemModel *model = ui->tableView->model ();
    QModelIndex index;
    QVariant data;
    QString str;
    int iAddr = 0;


    mainWin->printInfoWithTime("写入 ");

    index = model->index(row, 0);
    data = model->data(index);

    str = data.toString();
    iAddr = str.toInt(Q_NULLPTR,16);

    mainWin->printInfo(QString().sprintf(" 0x%x ", iAddr));


    index = model->index(row, 1);
    data = model->data(index);

    mainWin->printInfo(data.toString());

    mainWin->printInfo(" ");


    index = model->index(row, 2);
    data = model->data(index);

    if ((data.toString()).indexOf("_rw", true) < 0)
    {
            mainWin->printInfo("此寄存器只读 不可写入");
            return;
    }

    mainWin->printInfo(data.toString());

    mainWin->printInfo(" ");


    index = model->index(row, 4);
    data = model->data(index);

    mainWin->printInfo(data.toString());

    mainWin->printInfo(" :");


    index = model->index(row, 3);
    data = model->data(index);

    mainWin->printInfo(data.toString());


    if (!mainWin->checkMessageBoxIsOrNo("确定要写入数据进寄存器吗？"))
    {
        return;
    }


    switch(iAddr)
    {
    case    0x80:
    case    0x81:
    case    0x82:
    case    0x83:
    case    0x84:
    case    0x86:
    case    0x87:
    case    0x88:
    case    0x89:
    case    0x8c:
    case    0x96:
    case    0x99:
    case    0xe0:
    case    0xe2:
    case    0xe7:
    case    0xe8:
    case    0xe9:
    case    0xea:
    case    0xec:
    case    0xed:

        mainWin->dialog_writeSingleByAddress(fmIdx, static_cast<uchar>(iAddr), static_cast<uint16_t>(data.toInt()));

        break;

    case    0x85:
    case    0x8a:
    case    0x8b:
    case    0x8d:
    case    0x8e:
    case    0x8f:
    case    0x90:
    case    0x9e:
    case    0x9f:
    case    0x91:
    case    0x92:
    case    0x93:
    case    0x94:
    case    0x9b:
    case    0xe3:
    case    0xe4:
    case    0xeb:

        mainWin->dialog_readByAddressAndNum(fmIdx, static_cast<uchar>(iAddr), 2);

        break;

    default:

        break;
    }

}


void Dialog_SensorAddr::closeEvent(QCloseEvent *event)
{
    (void)(*event);

    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    if (mainWin->dialog_fmData->isVisible())
    {
        return;
    }

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        mainWin->sktPause[i + SOCKET_FLOWM1_INDEX] = false;
    }

}

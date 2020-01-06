#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"
#include "dialog_fmdata.h"


#define DELAY_REPEAT_PROCESS    static uint lastTime = 0;  \
    if (lastTime + 4 > QDateTime::currentDateTime().toTime_t()) \
    {           \
        return;     \
    }       \
    lastTime = QDateTime::currentDateTime().toTime_t();

#define PREFIX_PROCESS          if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX) return; \
    sktPause[fmIdx + SOCKET_FLOWM1_INDEX] = true;

#define SUFFIX_PROCESS          delayMSec(FM_PROCESS_WAIT_DELAY * 20);   \
    fmSendMsg[fmIdx].clear();   \
    fmRevMsg[fmIdx].clear();

bool FSC_MainWindow::checkMessageBoxIsOrNo(QString str)
{
    QMessageBox:: StandardButton result = QMessageBox::information(nullptr, "确认", str, \
                           QMessageBox::Yes|QMessageBox::No, QMessageBox::No);

    switch (result)
    {
    case QMessageBox::Yes:
        return true;

    case QMessageBox::No:
        return false;

    default:
        break;
    }

    return false;
}

void FSC_MainWindow::on_tbnCalManual_clicked()
{
    static uint lastTime = 0;

    ui->groupBox_manual->setEnabled(false);

    calGoingInfoLabRemove();

    if (!calManualDoing)
    {
        if (lastTime + 6 < QDateTime::currentDateTime().toTime_t())
        {
            lastTime = QDateTime::currentDateTime().toTime_t();
            fmResetAll(&oneCal);

            pump1Off();
            pump2Off();
            writePLC();
            delayMSec(1000);

            printInfoWithTime("请放空天平上容器->关闭放水阀->天平去皮清零->在手动控制区设定流量或设定占空比->正向或反向进水启动水泵");

            calManualDoing = true;
        }
    }
}

void FSC_MainWindow::plotAddDataAndFresh(void)
{

    if (calOn == CAL_PLOT_START)
    {

        plotScaleSumTimeX.append( plotLoop * 0.5 );
        plotSTDFMSumTimeX.append( plotLoop * 0.5 );
        plotSTDFMFlowTimeX.append( plotLoop * 0.5 );
        plotFMSumTimeX.append( plotLoop * 0.5 );
        plotFMFlowTimeX.append( plotLoop * 0.5 );

        plotScaleSumValueY.append(showScaleSum);
        plotSTDFMSumValueY.append(showSTDFMSum);
        plotSTDFMFlowValueY.append(showSTDFMFlow);
        plotFMSumValueY.append(showFMSum[0] + 100);
        plotFMFlowValueY.append(showFMFlow[0]);

        for(int i = 0; i < FLOWMETER_NUMBER; i++)
        {
            plotFMSumValueY_bak[i].append(showFMSum[i] + 100);
            plotFMFlowValueY_bak[i].append(showFMFlow[i]);
        }

        plotFMSumValueY = plotFMSumValueY_bak[ui->comboBox_PlotSenSel->currentIndex()];
        plotFMFlowValueY = plotFMFlowValueY_bak[ui->comboBox_PlotSenSel->currentIndex()];

        plotFresh();

        if (plotLoop++ > plotPosNumber || showScaleSum > SCALE_MAX_LIMITED)
        {
            calOn = CAL_STATE_STOP;

            pump1Off();
            pump2Off();
            writePLC();
            delayMSec(1000);
            printInfoWithTime("进水过程结束，请手动填入并写入参数值");

            ui->groupBox_manual->setEnabled(true);
        }
    }
}


void FSC_MainWindow::plotFresh(void)
{
    ui->MyCustomPlot->legend->setVisible(true);
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    ui->MyCustomPlot->graph(0)->setData(plotScaleSumTimeX, plotScaleSumValueY);
    ui->MyCustomPlot->graph(1)->setData(plotSTDFMSumTimeX, plotSTDFMSumValueY);
    ui->MyCustomPlot->graph(2)->setData(plotSTDFMFlowTimeX, plotSTDFMFlowValueY);
    ui->MyCustomPlot->graph(3)->setData(plotFMSumTimeX, plotFMSumValueY);
    ui->MyCustomPlot->graph(4)->setData(plotFMFlowTimeX, plotFMFlowValueY);

    ui->MyCustomPlot->rescaleAxes(true);
    ui->MyCustomPlot->replot();

}

void FSC_MainWindow::on_tbnCalManualWrite_clicked()
{
    if (!checkMessageBoxIsOrNo("确定要写入标定参数吗？"))
    {
        return;
    }

    DELAY_REPEAT_PROCESS

    printInfoWithTime("写入标定参数");

    int     fmIdx = ui->comboBox_PlotSenSel->currentIndex();
    double  data = ui->lineEdit_calParameter->text().toDouble();

    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return;
    }

    sktPause[fmIdx + SOCKET_FLOWM1_INDEX] = true;
    delayMSec(FM_PROCESS_WAIT_DELAY * 20);

    sendMsg_writeSingle_byAddr(fmIdx, PROTOCOL_XUNYIN_MODBUS_ADDR_GAIN_CONTROL, static_cast<uint16_t>(data));
    delayMSec(FM_PROCESS_WAIT_DELAY * 20);

    fmSendMsg[fmIdx].clear();
    fmRevMsg[fmIdx].clear();

    sktPause[fmIdx + SOCKET_FLOWM1_INDEX] = false;

}

void FSC_MainWindow::on_tbnCalManualCorrectWrite_clicked()
{
    if (!checkMessageBoxIsOrNo("确定要写入修正参数吗？"))
    {
        return;
    }

    DELAY_REPEAT_PROCESS

    printInfoWithTime("写入修正参数");


    int     fmIdx = ui->comboBox_PlotSenSel->currentIndex();
    //double  data = ui->lineEdit_calParameter->text().toDouble();

    if (fmIdx < 0 || fmIdx > SOCKET_FLOWM12_INDEX - SOCKET_FLOWM1_INDEX)
    {
        return;
    }

    sktPause[fmIdx + SOCKET_FLOWM1_INDEX] = true;
    delayMSec(FM_PROCESS_WAIT_DELAY * 20);

    sendMsg_writeSET_KF1(fmIdx);
    delayMSec(FM_PROCESS_WAIT_DELAY * 20);

    fmSendMsg[fmIdx].clear();
    fmRevMsg[fmIdx].clear();

    sktPause[fmIdx + SOCKET_FLOWM1_INDEX] = false;
}

void FSC_MainWindow::action_rightKeyCorrectMenu(int fmIdx)
{
    Dialog_fmdata   *pfmData = static_cast<Dialog_fmdata*>(dialog_fmData);

    dialog_fmData->show();
    dialog_fmData->move(dialog_fmData->geometry().x(), dialog_fmData->geometry().y() + 50);


    pfmData->setFmName(fmIdx);

    sktPause[fmIdx + SOCKET_FLOWM1_INDEX] = true;
}

void FSC_MainWindow::dialog_readZERO_CAL(int fmIdx)
{
    DELAY_REPEAT_PROCESS

    printInfoWithTime("读取ZERO_CAL");

    PREFIX_PROCESS

    sendMsg_readZERO_CAL(fmIdx);

    SUFFIX_PROCESS
}
void FSC_MainWindow::dialog_writeZERO_CAL(int fmIdx)
{
    DELAY_REPEAT_PROCESS

    printInfoWithTime("写入ZERO_CAL");

    PREFIX_PROCESS

    sendMsg_writeZERO_CAL(fmIdx);

    SUFFIX_PROCESS
}

void FSC_MainWindow::dialog_readFLOW_RANGE(int fmIdx)
{
    DELAY_REPEAT_PROCESS

    printInfoWithTime("读取FLOW_RANGE参数");

    PREFIX_PROCESS

    sendMsg_readFLOW_RANGE(fmIdx);

    SUFFIX_PROCESS
}
void FSC_MainWindow::dialog_writeFLOW_RANGE(int fmIdx)
{
    if (!checkMessageBoxIsOrNo("确定要写入FLOW_RANGE参数吗？"))
    {
        return;
    }

    DELAY_REPEAT_PROCESS

    printInfoWithTime("写入FLOW_RANGE参数");

    PREFIX_PROCESS

    sendMsg_writeFLOW_RANGE(fmIdx);

    SUFFIX_PROCESS
}


void FSC_MainWindow::dialog_readSET_KF1(int fmIdx)
{
    DELAY_REPEAT_PROCESS

    printInfoWithTime("读取修正参数");

    PREFIX_PROCESS

    sendMsg_readSET_KF1(fmIdx);

    SUFFIX_PROCESS
}
void FSC_MainWindow::dialog_readSET_KF2(int fmIdx)
{
    DELAY_REPEAT_PROCESS

    printInfoWithTime("读取SET_KF2参数");

    PREFIX_PROCESS

    sendMsg_readSET_KF2(fmIdx);

    SUFFIX_PROCESS
}
void FSC_MainWindow::dialog_writeSET_KF1(int fmIdx)
{
    if (!checkMessageBoxIsOrNo("确定要写入修正参数吗？"))
    {
        return;
    }

    DELAY_REPEAT_PROCESS

    printInfoWithTime("写入修正参数");

    PREFIX_PROCESS

    sendMsg_writeSET_KF1(fmIdx);

    SUFFIX_PROCESS
}
void FSC_MainWindow::dialog_writeSET_KF2(int fmIdx)
{
    if (!checkMessageBoxIsOrNo("确定要写入SET_KF2参数吗？"))
    {
        return;
    }

    DELAY_REPEAT_PROCESS

    printInfoWithTime("写入SET_KF2参数");

    PREFIX_PROCESS

    sendMsg_writeSET_KF2(fmIdx);

    SUFFIX_PROCESS
}


void FSC_MainWindow::dialog_readByAddressAndNum(int fmIdx, uchar addr, char num)
{
    //DELAY_REPEAT_PROCESS

    printInfoWithTime("读取寄存器");

    PREFIX_PROCESS

    sendMsg_read_byAddr(fmIdx, addr, num);

    SUFFIX_PROCESS
}


void FSC_MainWindow::dialog_writeSingleByAddress(int fmIdx, uchar addr, uint16_t data)
{
    //DELAY_REPEAT_PROCESS

    printInfoWithTime("写寄存器");

    PREFIX_PROCESS

    sendMsg_writeSingle_byAddr(fmIdx, addr, data);

    SUFFIX_PROCESS
}

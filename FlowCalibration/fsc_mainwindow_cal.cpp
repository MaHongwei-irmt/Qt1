#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::startCal(void)
{
    QString str;

    switch (calOn)
    {
    case CAL_START:
        calGoingInfoLabRemove();

        printInfoWithTime("启动标定->清空曲线图->打开放水阀");
        on_tbnPoltClear_clicked();
        openOutValve();
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY);

        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();

        if(calOn)
        {
            calOn = CAL_START_OUT_VALVE_OPEN;
        }
        calOnTime = QDateTime::currentDateTime().toTime_t();

        allCalNeedToReport = false;
        for(int i = 0; i < CAL_MAX_STEP; i++)
        {
            allCalAvailable[i]  = false;
        }

        break;

    case CAL_START_OUT_VALVE_OPEN:

        if (firstCal)
        {
            showSetPWM = 30;
            on_radioButton_setPWM_clicked();
            printInfoWithTime("->正向进水管路充水");

            if (calOn != CAL_STATE_STOP)
            {
                on_tbnPump1ForwardOn_clicked();
            }

            calOnTime = QDateTime::currentDateTime().toTime_t();

            delayMSec(PUMP_START_DELAY);

            if (calOn != CAL_STATE_STOP)
            {
                on_tbnPump2ForwardOn_clicked();
            }

            printInfoWithTime("->充水10秒...");
            if(calOn)
            {
                calOn = CAL_START_FIRST_FORWARD_FILL_WATER;
            }
        }
        else
        {
            if(calOn)
            {
                calOn = CAL_WAIT_WATER_EMPTY;
            }
        }

        break;

    case CAL_START_FIRST_FORWARD_FILL_WATER:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 9)
        {
            calOnTime = QDateTime::currentDateTime().toTime_t();
            printInfoWithTime("->关闭水泵->反向进水管路充水");

            pump1Off();
            pump2Off();
            writePLC();
            delayMSec(PUMP_START_DELAY);

            closeForwardValveAll();
            closeReverseValveAll();
            writePLC();

            delayMSec(PUMP_START_DELAY);


            if (calOn != CAL_STATE_STOP)
            {
                on_tbnPump1ReverseOn_clicked();
            }

            calOnTime = QDateTime::currentDateTime().toTime_t();

            delayMSec(PUMP_START_DELAY);

            if (calOn != CAL_STATE_STOP)
            {
                on_tbnPump2ReverseOn_clicked();
            }

            printInfoWithTime("->充水10秒...");
            if(calOn)
            {
                calOn = CAL_START_FIRST_REVERSE_FILL_WATER;
            }

        }

        break;

    case CAL_START_FIRST_REVERSE_FILL_WATER:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 9)
        {
            firstCal = false;

            calOnTime = QDateTime::currentDateTime().toTime_t();
            printInfoWithTime("->关闭水泵->等待放水结束..");

            pump1Off();
            pump2Off();
            writePLC();
            delayMSec(PUMP_START_DELAY);

            on_radioButton_setFlowRate_clicked();

            if(calOn)
            {
                calOn = CAL_WAIT_WATER_EMPTY;
            }

        }

        break;

    case CAL_WAIT_WATER_EMPTY:

        if(checkWaterEmpty())
        {
            printInfoWithTime("->天平上容器水放空->关闭放水阀->天平清零");
            closeOutValve();
            writePLC();
            delayMSec(VALVE_EXCHANGE_DELAY);
            reqScaleZero();
            fmResetAll(&oneCal);

            delayMSec(VALVE_EXCHANGE_DELAY);

            if(calOn)
            {
                calOn = CAL_PROCESS;
            }

        }

        break;

    case CAL_PROCESS:

        if (!calProc())
        {
            if(calOn)
            {
                calOn = CAL_PROCESS_END;
            }
        }

        break;

    case CAL_PROCESS_END:

        allCalEnd = true;
        makeCalRecordPrint(&oneCal);

        printInfoWithTime("本次全部结束.");

        calOn = CAL_STATE_STOP;

        break;


    }

}

bool FSC_MainWindow::calProc(void)
{
    //bool ret = fillOneCal(&oneCal);
    bool ret = fillOneCalLink(&oneCal);

    if (ret)
    {
        //calSingle(&oneCal);
        calSingleLink(&oneCal);
    }

    return ret;
}

bool FSC_MainWindow::fillOneCalLink(oneCalTag *calTag)
{
    if (calTag->state == ONE_CAL_EMPTY)
    {
        if ( currentStep.stepTotal == 0 || currentStep.stepCurrent == currentStep.stepTotal)
        {
            allCalEnd = true;

            if(allCalNeedToReport)
            {
                makeCalRecordPrint(calTag);
            }
            else
            {
                printInfoWithTime("无数据生成报表");
            }

            printInfoWithTime("本次全部结束");
            calOn = CAL_STATE_STOP;
            return false;
        }

        if ((calTag->step + 1)  > currentStep.stepTotal)
        {
            calTag->step = 0;
            currentStep.stepCurrent = 0;

            return false;
        }

        if (calTag->step > 0)
        {
            currentStep.stepCurrent++;
            if (currentStep.stepCurrent > currentStep.stepTotal)
            {
                currentStep.stepCurrent = currentStep.stepTotal;
            }

            curShowStep++;
            if (curShowStep > calRunLink.size())
            {
                curShowStep = calRunLink.size();
            }
        }

        calTag->stepNum = calRunLink[curShowStep].stepNum;
        calTag->calDirect = calRunLink[curShowStep].direct;
        calTag->calTpye = calRunLink[curShowStep].type;
        calTag->calSpanPercent = calRunLink[curShowStep].spanPercent;
        calTag->calSpan = calRunLink[curShowStep].span;

        calTag->step = currentStep.stepCurrent + 1;

        if (calTag->step  > currentStep.stepTotal)
        {
            printInfoWithTime("步数错误，重新选择");

            calTag->step = 0;
            currentStep.stepCurrent = 0;

            return false;
        }

        calTag->state = ONE_CAL_START;

        calTag->calTime = QDateTime::currentDateTime().toTime_t();

        calTag->plotTimeX.clear();
        calTag->plotScaleSumValue.clear();
        calTag->plotSTDFMSumValue.clear();
        calTag->plotSTDFMRateValue.clear();

        for (int i = 0; i < FLOWMETER_NUMBER; i++)
        {
            calTag->plotFMSumValue[i].clear();
            calTag->plotFMRateValue[i].clear();
        }
    }

    return true;
}

void FSC_MainWindow::calSingleLink(oneCalTag *calTag)
{
    QString str;

    if (calTag->step < 0 || calTag->step > calRunLink.size())
    {
        return;
    }

    if (calTag->state == ONE_CAL_START)
    {
        str += QString().sprintf("第(%d)步开始->", calRunLink[calTag->step - 1].stepNum + 1);

        if (calTag->calDirect == START_CAL_DIRECT_FORWARD)
        {
            str += "正向->";
        }
        else if (calTag->calDirect == START_CAL_DIRECT_REVERSE)
        {
            str += "反向->";
        }

        if (calTag->calTpye == START_CAL_TYPE_CAL)
        {
            str += "标定->";
        }
        else if (calTag->calTpye == START_CAL_TYPE_CORRECT)
        {
            str += "修正->";
        }
        else if (calTag->calTpye == START_CAL_TYPE_CHECK)
        {
             str += "验证->";
        }

        str += QString().sprintf("%d", calTag->calSpanPercent);
        str += "%量程->天平清零->";
        str += QString().sprintf("%0.3fml/min->", calTag->calSpan);
        printInfoWithTime(str);

        reqScaleZero();
        fmResetAll(&oneCal);

        calTag->state = ONE_CAL_START_END;

        calTag->calTime = QDateTime::currentDateTime().toTime_t();
    }

    if (calTag->state == ONE_CAL_START_END)
    {
        if(QDateTime::currentDateTime().toTime_t()- calTag->calTime > 2)
        {
            //if (showScaleSum > 2)
            if (showScaleSum < 2)
            {
                printInfoWithTime("->天平再次清零");
                reqScaleZero();
                fmResetAll(&oneCal);

                calTag->calTime = QDateTime::currentDateTime().toTime_t();

            }
            else
            {
                showSetFlowRate = calTag->calSpan;
                on_radioButton_setFlowRate_clicked();

                calTag->stepRecord = calTag->step;
                calGoingInfoLab(calTag);

                if (calTag->calDirect == START_CAL_DIRECT_FORWARD)
                {
                    if (showSetFlowRate > MAX_SPAN)
                    {
                        printInfoWithTime("->关闭反向进水阀");
                        closeReverseValveAll();
                        writePLC();
                        delayMSec(VALVE_EXCHANGE_DELAY);

                        printInfo("->打开正向进水阀");
                        openForwardValveAll();
                        writePLC();
                        delayMSec(VALVE_EXCHANGE_DELAY);

                        printInfoWithTime("->启动1#泵->启动2#泵");

                        if (calOn != CAL_STATE_STOP)
                        {
                            pump1On();
                            pump2On();
                            writePLC();
                        }
                    }
                    else if (showSetFlowRate > MIN_SPAN)
                    {
                        if (calOn != CAL_STATE_STOP)
                        {
                            on_tbnPump2ForwardOn_clicked();
                        }

                    }
                    else
                    {
                        if (calOn != CAL_STATE_STOP)
                        {
                            on_tbnPump1ForwardOn_clicked();
                        }
                    }

                }
                else if (calTag->calDirect == START_CAL_DIRECT_REVERSE)
                {
                    if (showSetFlowRate > MAX_SPAN)
                    {
                        printInfoWithTime("->关闭正向进水阀");
                        closeForwardValveAll();
                        writePLC();
                        delayMSec(VALVE_EXCHANGE_DELAY);

                        printInfo("->打开反向进水阀");
                        openReverseValveAll();
                        writePLC();
                        delayMSec(VALVE_EXCHANGE_DELAY);

                        printInfoWithTime("->启动1#泵->启动2#泵");
                        if (calOn != CAL_STATE_STOP)
                        {
                            pump1On();
                            pump2On();
                            writePLC();
                        }

                    }
                    else if (showSetFlowRate > MIN_SPAN)
                    {
                        if (calOn != CAL_STATE_STOP)
                        {
                            on_tbnPump2ReverseOn_clicked();
                        }
                    }
                    else
                    {
                        if (calOn != CAL_STATE_STOP)
                        {
                            on_tbnPump1ReverseOn_clicked();
                        }
                    }
                }

                calTag->state = ONE_CAL_GOING;

                oneCal.plotSelectedFMIndex  = ui->comboBox_PlotSenSel->currentIndex();
                oneCal.plotSelectedFMStr  = ui->comboBox_PlotSenSel->currentText();

                calTag->calTime = QDateTime::currentDateTime().toTime_t();
                calTag->calProcStartTime = QDateTime::currentDateTime().toTime_t();
                calTag->calProcEndTime = QDateTime::currentDateTime().toTime_t();

            }
        }
    }

    if (calTag->state == ONE_CAL_GOING)
    {
        calGoing(calTag);
    }

    if(calTag->state == ONE_CAL_POST_PROCESS)
    {
        calDoing(calTag);

        calTag->state = ONE_CAL_EMPTY;
    }
}

void FSC_MainWindow::calGoing(oneCalTag *calTag)
{
    (void) *calTag;

    calTag->plotTimeX.append(calTag->plotPos * 0.5);

    calTag->plotScaleSumValue.append(showScaleSum);
    calTag->plotSTDFMSumValue.append(showSTDFMSum);
    calTag->plotSTDFMRateValue.append(showSTDFMFlow);

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        calTag->plotFMSumValue[i].append(showFMSum[i]);
        calTag->plotFMRateValue[i].append(showFMFlow[i]);
    }

    calPlot(calTag);

    if (calTag->plotPos++ > plotPosNumber || showScaleSum > SCALE_MAX_LIMITED)
    {
        calTag->state = ONE_CAL_POST_PROCESS;

        printInfoWithTime("本步结束->停泵->关闭进水阀");

        openOutValve();
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY / 2);

        calTag->calProcEndTime = QDateTime::currentDateTime().toTime_t();
        delayMSec(PUMP_START_DELAY / 2);

        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();


        calTag->finalScaleSumValue = showScaleSum;
        calTag->finalSTDFMSumValue = showSTDFMSum;
        calTag->finalSTDFMRateValue = showSTDFMFlow;

        for (int i = 0; i < FLOWMETER_NUMBER; i++)
        {
            calTag->finalFMSumValue[i] = showFMSum[i];
            calTag->finalFMRateValue[i] = showFMFlow[i];
        }

    }

}

void FSC_MainWindow::calPlot(oneCalTag *calTag)
{
    ui->MyCustomPlot->graph(3)->setName(calTag->plotSelectedFMStr + "流量");
    ui->MyCustomPlot->graph(4)->setName(calTag->plotSelectedFMStr + "流速");

    ui->MyCustomPlot->legend->setVisible(true);
    ui->MyCustomPlot->legend->setFont(QFont("Helvetica", 8));

    ui->MyCustomPlot->graph(0)->setData(calTag->plotTimeX, calTag->plotScaleSumValue);
    ui->MyCustomPlot->graph(1)->setData(calTag->plotTimeX, calTag->plotSTDFMSumValue);
    ui->MyCustomPlot->graph(2)->setData(calTag->plotTimeX, calTag->plotSTDFMRateValue);

    ui->MyCustomPlot->graph(3)->setData(calTag->plotTimeX, calTag->plotFMSumValue[calTag->plotSelectedFMIndex]);
    ui->MyCustomPlot->graph(4)->setData(calTag->plotTimeX, calTag->plotFMRateValue[calTag->plotSelectedFMIndex]);

    ui->MyCustomPlot->rescaleAxes(true);
    ui->MyCustomPlot->replot();

}

void FSC_MainWindow::calDoing(oneCalTag *calTag)
{
    if (!calTag) return;

    if (calTag->step < 1) return;

    if (calTag->calTpye == START_CAL_TYPE_CAL)
    {
        fmCalibration(calTag);
    }
    else if (calTag->calTpye == START_CAL_TYPE_CORRECT)
    {
        fmCorrect(calTag);
    }
    else if (calTag->calTpye == START_CAL_TYPE_CHECK)
    {
         fmCheck(calTag);
    }

    if (calTag->stepRecord >= 0 && calTag->stepRecord < CAL_MAX_STEP)
    {
        calTag->stepBak = currentStep;

        if (calTag->stepRecord < 1) return;

        calTag->calDateTime = QDateTime::currentDateTime();
        allCal[calTag->stepRecord - 1] = *calTag;

    }

    allCalNeedToReport = true;
    allCalAvailable[calTag->step - 1]  = true;
    allCalEnd = false;


    printInfoWithTime("->打开放水阀");
    on_tbnPoltClear_clicked();
    openOutValve();
    pump1Off();
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);

    if(checkWaterEmpty())
    {
        printInfoWithTime("->天平上容器水放空->关闭放水阀");
        closeOutValve();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);
    }

    calTag->state = ONE_CAL_EMPTY;
    calTag->plotPos = 0;

    if (calTag->pause)
    {
        calOn = CAL_STATE_STOP;
        calTag->pause = false;
    }
}

void FSC_MainWindow::waitFmCommuication(oneCalTag *calTag, int idFmProcess)
{
    bool    ret = true;
    int     k_timeout[FLOWMETER_NUMBER];

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        k_timeout[i] = 0;

        if (showFMSum[i] < 1.0 ||  std::isnan(showFMFlow[i]))
        {
            continue;
        }

        fmReadWriteSelect[i] = idFmProcess;
        fmRWflag[i] = 1;

        if (idFmProcess == RESET_FM_READ_WRITE)
        {
            calTag->reset_result[i] = 0;
        }
        {
            calTag->result[i] = 0;
        }
    }

    while(ret)
    {
        delayMSec(FM_PROCESS_WAIT_DELAY);
        ret = false;

        for (int i = 0; i < FLOWMETER_NUMBER; i++)
        {
            if (k_timeout[i]++ > FM_PROCESS_RW_TIMEOUT)
            {
                if (showFMSum[i] < 1.0 ||  std::isnan(showFMFlow[i]))
                {
                    continue;
                }

                if (idFmProcess == RESET_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计Reset 失败", i + 1));
                    calTag->reset_result[i] = TISH_STEP_FAULT;
                }
                else if (idFmProcess == CALIBRATION_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计标定失败", i + 1));
                }
                else if (idFmProcess == CORRECT_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计修正失败", i + 1));
                }

                if (idFmProcess != RESET_FM_READ_WRITE)
                {
                    calTag->result[i] = TISH_STEP_FAULT;
                }

                fmRWTimer[i]->stop();
            }
            else if (fmReadWriteSelect[i] == TISH_STEP_SUCCEED)
            {
                if (idFmProcess == CALIBRATION_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计标定数据写入成功", i + 1));
                }
                else if (idFmProcess == CORRECT_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计修正数据写入成功", i + 1));
                }

                if (idFmProcess == RESET_FM_READ_WRITE)
                {
                    calTag->reset_result[i] = TISH_STEP_SUCCEED;
                }
                else
                {
                    calTag->result[i] = TISH_STEP_SUCCEED;
                }

                fmRWTimer[i]->stop();
            }
            else if(fmReadWriteSelect[i] == TISH_STEP_READ_FAULT)
            {
                if (idFmProcess == CALIBRATION_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计标定数据读取失败", i + 1));
                }
                else if (idFmProcess == CORRECT_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计修正数据读取失败", i + 1));
                }

                calTag->result[i] = TISH_STEP_READ_FAULT;

                fmRWTimer[i]->stop();
            }
            else if(fmReadWriteSelect[i] == TISH_STEP_WRITE_FAULT)
            {

                if (idFmProcess == RESET_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计Reset失败", i + 1));
                    calTag->reset_result[i] = TISH_STEP_WRITE_FAULT;
                }
                else if (idFmProcess == CALIBRATION_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计标定数据写入失败", i + 1));
                }
                else if (idFmProcess == CORRECT_FM_READ_WRITE)
                {
                    printInfoWithTime(QString().sprintf("%d#流量计修正数据写入失败", i + 1));
                }

                if (idFmProcess != RESET_FM_READ_WRITE)
                {
                    calTag->result[i] = TISH_STEP_WRITE_FAULT;
                }

                fmRWTimer[i]->stop();
            }
            else
            if (fmReadWriteSelect[i] == idFmProcess)
            {
                ret = true;
            }
        }
    }

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        fmReadWriteSelect[i] = 0;
        fmRWflag[i] = 0;
    }

    resetFmCommunicationWithStfm();
}

void FSC_MainWindow::waitStfmCommuication(oneCalTag *calTag, int idFmProcess)
{
    bool    ret = true;
    int     k = 0;

    stfmReadWriteSelect = RESET_FM_READ_WRITE;
    stfmRWflag = 1;
    calTag->reset_stfm_result = 0;

    while(ret)
    {
        delayMSec(FM_PROCESS_WAIT_DELAY);

        ret = false;

        if (k++ > FM_PROCESS_RW_TIMEOUT)
        {
            printInfoWithTime("标准流量计Reset 失败");
            calTag->reset_stfm_result = TISH_STEP_FAULT;

            stfmRWTimer->stop();
        }
        else if (stfmReadWriteSelect == TISH_STEP_SUCCEED)
        {
            calTag->reset_stfm_result = TISH_STEP_SUCCEED;

            stfmRWTimer->stop();
        }
        else if(stfmReadWriteSelect == TISH_STEP_WRITE_FAULT)
        {
            printInfoWithTime(QString().sprintf("标准流量计Reset失败"));
            calTag->reset_stfm_result = TISH_STEP_WRITE_FAULT;

            stfmRWTimer->stop();
        }
        else if (stfmReadWriteSelect == idFmProcess)
        {
            ret = true;
        }

    }

    stfmReadWriteSelect = 0;
    stfmRWflag = 0;

    resetFmCommunicationWithStfm();
}

void FSC_MainWindow::fmResetAll(oneCalTag *calTag)
{
    waitStfmCommuication(calTag, RESET_FM_READ_WRITE);
    waitFmCommuication(calTag, RESET_FM_READ_WRITE);
 }

void FSC_MainWindow::fmCalibration(oneCalTag *calTag)
{
    //double d = 0;

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        if (calTag->finalFMSumValue[i] < 1.0 ||  std::isnan(calTag->finalFMSumValue[i]))
        {
            continue;
        }

        //d = calTag->finalFMSumValue[i] - calTag->finalScaleSumValue;


        /*...*/

        fmReadWriteSelect[i] = CALIBRATION_FM_READ_WRITE;
        fmRWflag[i] = 1;

        calTag->result[i] = 0;

    }
    printInfoWithTime("流量计标定数据指令开始");

    waitFmCommuication(calTag, CALIBRATION_FM_READ_WRITE);
}

void FSC_MainWindow::fmCorrect(oneCalTag *calTag)
{
    //double d = 0;


    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        if (calTag->finalFMSumValue[i] < 1.0 ||  std::isnan(calTag->finalFMSumValue[i]))
        {
            continue;
        }

        //d = calTag->finalFMSumValue[i] - calTag->finalScaleSumValue;


        /*...*/

        fmReadWriteSelect[i] = CORRECT_FM_READ_WRITE;
        fmRWflag[i] = 1;

        calTag->result[i] = 0;

    }
    printInfoWithTime("流量计修正数据指令开始");

    waitFmCommuication(calTag, CORRECT_FM_READ_WRITE);
}

void FSC_MainWindow::fmCheck(oneCalTag *calTag)
{
    (void) calTag;

    printInfoWithTime("验证结束");
}

void FSC_MainWindow::calStop(oneCalTag *calTag)
{

    calTag->state = ONE_CAL_EMPTY;
    calTag->plotPos = 0;

    calTag->step = 0;
    currentStep.stepCurrent = 0;
    curShowStep = 0;

    pump1Off();
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);


    pump1Off();
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY / 2);

    closeForwardValveAll();
    closeReverseValveAll();
    writePLC();

    calOn = CAL_STATE_STOP;
}

void FSC_MainWindow::makeCalRecordPrint(oneCalTag *cal)
{
    if (cal == nullptr) return;
    if (!allCalNeedToReport) return;

    QString fileName = "流量标定报表" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    calTableName = fileName;

    QTextStream stream(&file);

    QString str =  "报表生成时间：" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n";
    str += "型号：" + cal->stepBak.type_name + "   量程：" + QString::number(cal->stepBak.span_ml_per_min) + "ml/min   \
            水温：25°C    共" + QString::number(currentStep.stepTotal) + "步\n" ;
    str += "\n";

    for(int i_step = 0; i_step < CAL_MAX_STEP; i_step++)
    {
        if (allCalAvailable[i_step])
        {
            str += "第(" + QString::number(allCal[i_step].stepNum + 1) + ")步" + "   " + \
                    QString::number(allCal[i_step].calSpanPercent) + "%量程("\
                    + QString::number((allCal[i_step].calSpanPercent * allCal[i_step].stepBak.span_ml_per_min / 100), 'f',3) \
                    + "ml/min) ";


            if (allCal[i_step].calDirect == START_CAL_DIRECT_FORWARD)
            {
                str += "正向 ";
            }
            else if (allCal[i_step].calDirect == START_CAL_DIRECT_REVERSE)
            {
                str += "反向 ";
            }

            if (allCal[i_step].calTpye == START_CAL_TYPE_CAL)
            {
                str += "标定 ";
            }
            else if (allCal[i_step].calTpye == START_CAL_TYPE_CORRECT)
            {
                str += "修正 ";
            }
            else if (allCal[i_step].calTpye == START_CAL_TYPE_CHECK)
            {
                str += "验证 ";
            }

            str += "时间：" + allCal[i_step].calDateTime.toString("yyyy-MM-dd hh:mm:ss   ");

            str += "用时：" + QString::number(allCal[i_step].calProcEndTime - allCal[i_step].calProcStartTime);

            str += "s\n";

            if (allCal[i_step].reset_stfm_result == TISH_STEP_WRITE_FAULT)
            {
                str += "标准流量计Reset失败!\n";
            }

            str += "天平称重：" + QString::number(allCal[i_step].finalScaleSumValue, 'f', 3) + "g\n";
            str += "标准流量计累计流量：" + QString::number(allCal[i_step].finalSTDFMSumValue, 'f', 3) + "ml\n";
            str += "标准流量计最后流速：" + QString::number(allCal[i_step].finalSTDFMRateValue, 'f', 3) + "ml/min\n";

            double d = 0;
            for (int i_plot_point = 0; i_plot_point < allCal[i_step].plotSTDFMRateValue.size(); i_plot_point++)
            {
                d += allCal[i_step].plotSTDFMRateValue[i_plot_point];
            }
            d /= allCal[i_step].plotSTDFMRateValue.size();

            str += "标准流量计平均流速：" + QString::number(d, 'f', 3) + "ml/min\n";
            str += "\n";

            for (int k_fm = 0; k_fm < FLOWMETER_NUMBER; k_fm++)
            {
                if (allCal[i_step].finalFMSumValue[k_fm] < 1.0 ||  std::isnan(allCal[i_step].finalFMSumValue[k_fm]))
                {
                    continue;
                }

                if (allCal[i_step].reset_result[k_fm] ==  TISH_STEP_WRITE_FAULT)
                {
                    str += QString().sprintf("%d#流量计Rese失败!\n", k_fm + 1);
                }
                else if (allCal[i_step].reset_result[k_fm] ==  TISH_STEP_FAULT)
                {
                    str += QString().sprintf("%d#流量计Reset 失败!\n", k_fm + 1);
                }

                str += QString::number(k_fm + 1) + "#流量计累计流量：" + QString::number(allCal[i_step].finalFMSumValue[k_fm], 'f', 3) + "ml\n";
                str += QString::number(k_fm + 1) + "#流量计最后流速：" + QString::number(allCal[i_step].finalFMRateValue[k_fm], 'f', 3) + "ml/min\n";

                d = 0;
                for (int j_plot_point = 0; j_plot_point < allCal[i_step].plotFMRateValue[k_fm].size(); j_plot_point++)
                {
                    d += allCal[i_step].plotFMRateValue[k_fm][j_plot_point];
                }
                d /= allCal[i_step].plotFMRateValue[k_fm].size();

                str += QString::number(k_fm + 1) + "#流量计平均流速：" + QString::number(d, 'f', 3) + "ml/min\n";

                d = allCal[i_step].finalFMSumValue[k_fm] - allCal[i_step].finalScaleSumValue;
                str += "累计流量与天平误差：" + QString::number(d, 'f', 3) + "ml(g)\n";
                str += "误差百分比：" + QString::number(d / allCal[i_step].finalScaleSumValue * 100, 'f', 3) + "%   ";

                if (allCal[i_step].calTpye == START_CAL_TYPE_CAL)
                {
                    str += "标定";
                    if (allCal[i_step].result[k_fm] == TISH_STEP_SUCCEED)
                    {
                        str += "成功 ";
                    }
                    else if (allCal[i_step].result[k_fm] == TISH_STEP_READ_FAULT)
                    {
                        str += "读取失败! ";
                    }
                    else if (allCal[i_step].result[k_fm] == TISH_STEP_WRITE_FAULT)
                    {
                        str += "写入失败! ";
                    }
                    else if (allCal[i_step].result[k_fm] == TISH_STEP_FAULT)
                    {
                        str += "失败! ";
                    }
                }
                else if (allCal[i_step].calTpye == START_CAL_TYPE_CORRECT)
                {
                    str += "修正";
                    if (allCal[i_step].result[k_fm] == TISH_STEP_SUCCEED)
                    {
                        str += "成功 ";
                    }
                    else if (allCal[i_step].result[k_fm] == TISH_STEP_READ_FAULT)
                    {
                        str += "读取失败! ";
                    }
                    else if (allCal[i_step].result[k_fm] == TISH_STEP_WRITE_FAULT)
                    {
                        str += "写入失败! ";
                    }
                    else if (allCal[i_step].result[k_fm] == TISH_STEP_FAULT)
                    {
                        str += "失败! ";
                    }
                }
                else if (allCal[i_step].calTpye == START_CAL_TYPE_CHECK)
                {
                    str += "验证完成 ";
                }

                str += "\n\n";
            }
        }
    }

    if (allCalEnd)
    {
        str += "结束";

        QString pdfFileName = "流量标定报表" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".pdf";
        QPrinter ptr;
        ptr.setOutputFileName(pdfFileName);

        QTextDocument td;
        td.setPlainText(str);
        td.print(&ptr);
    }
    else
    {
        str += RECORD_FILE_TO_BE_CONTINUE;
    }

    QByteArray bUtf8 = str.toUtf8();

    stream << bUtf8;

    file.close();

    if (allCalEnd && printerName.size() > 0 && autoPrinter)
    {
        QPrinter ptr;
        ptr.setPrinterName(printerName);

        QTextDocument td;
        td.setPlainText(str);
        td.print(&ptr);
    }

    printInfoWithTime("生成报表");
}

void FSC_MainWindow::showPlotFresh(void)
{
    if(calOn == CAL_PLOT_START)
    {
        ui->lineEdit_plotTime->setText(QString::number(plotLoop * 0.5, 'f', 1) + "s");
    }
    else
    {
        ui->lineEdit_plotTime->setText(QString::number(oneCal.plotPos * 0.5, 'f', 1) + "s");
    }

}

bool FSC_MainWindow::checkWaterEmpty(void)
{
    return true;
}

void FSC_MainWindow::on_comboBox_PlotSenSel_currentIndexChanged(const QString &arg1)
{
    if (plotFMSumValueY_bak[0].size() > 0)
    {
        plotFMSumValueY = plotFMSumValueY_bak[ui->comboBox_PlotSenSel->currentIndex()];
        plotFMFlowValueY = plotFMFlowValueY_bak[ui->comboBox_PlotSenSel->currentIndex()];

        plotFresh();

        return;
    }

    oneCal.plotSelectedFMIndex  = ui->comboBox_PlotSenSel->currentIndex();
    oneCal.plotSelectedFMStr  = arg1;

    calPlot(&oneCal);
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

void FSC_MainWindow::calFaultStop(oneCalTag *calTag)
{
    calOn = CAL_STATE_STOP;
    printInfoWithTime(" 故障！标定暂停");

    calTag->state = ONE_CAL_EMPTY;
    calTag->plotPos = 0;

    if (calTag->step > 0)
    {
        calTag->step--;
    }
    if (currentStep.stepCurrent)
    {
        currentStep.stepCurrent--;;
    }

    pump1Off();
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);

    closeForwardValveAll();
    closeReverseValveAll();
    writePLC();
}

void FSC_MainWindow::calGoingInfoLab(oneCalTag *calTag)
{
    QString str;

    str = "型号:";
    str += currentStep.type_name;
    str += " 量程：";
    str += QString::number(currentStep.span_ml_per_min, 'f', 1);
    str += "  第(";
    if(calTag->stepRecord > 0)
    {
        str += QString::number(calTag->stepRecord);
    }
    else
    {
        str +="1";
    }
    str += ")步 ";
    str += QString::number(calTag->calSpanPercent);
    str += "%量程:";
    str += QString::number(calTag->calSpan, 'f', 1);

    if (calTag->calDirect == START_CAL_DIRECT_FORWARD)
    {
        str += " 正向";
    }
    else if (calTag->calDirect == START_CAL_DIRECT_REVERSE)
    {
        str += " 反向";
    }

    if (calTag->calTpye == START_CAL_TYPE_CAL)
    {
        str += " 标定";
    }
    else if (calTag->calTpye == START_CAL_TYPE_CORRECT)
    {
        str += " 修正";
    }
    else if (calTag->calTpye == START_CAL_TYPE_CHECK)
    {
         str += " 验证";
    }

    ui->label_calInfo->setText(str);
    ui->label_calInfo->setVisible(true);
}

void FSC_MainWindow::calGoingInfoLabRemove(void)
{
    ui->label_calInfo->setVisible(false);;
}



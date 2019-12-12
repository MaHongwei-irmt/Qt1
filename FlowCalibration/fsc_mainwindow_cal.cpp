#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

void FSC_MainWindow::startCal(void)
{
    //static bool firstCal = true;
    static bool firstCal = false;
    QString str;


    switch (calOn)
    {
    case CAL_START:

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

        break;

    case CAL_START_OUT_VALVE_OPEN:

        if (firstCal)
        {
            showSetPWM = 30;
            on_radioButton_setPWM_clicked();
            printInfoWithTime("->正向进水管路充水");
            on_tbnPump1ForwardOn_clicked();
            calOnTime = QDateTime::currentDateTime().toTime_t();

            delayMSec(PUMP_START_DELAY);
            on_tbnPump2ForwardOn_clicked();
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
            on_tbnPump1ReverseOn_clicked();
            calOnTime = QDateTime::currentDateTime().toTime_t();

            delayMSec(PUMP_START_DELAY);
            on_tbnPump2ReverseOn_clicked();
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

        makeCalRecordPrint(&oneCal);
        printInfoWithTime("本次全部结束.");

        calOn = CAL_STATE_STOP;

        break;


    }

}

bool FSC_MainWindow::calProc(void)
{
    bool ret = fillOneCal(&oneCal);

    if (ret)
    {
        calSingle(&oneCal);
    }

    return ret;
}

bool FSC_MainWindow::fillOneCal(oneCalTag *calTag)
{
    if (calTag->state == ONE_CAL_EMPTY)
    {
        if ( currentStep.stepTotal == 0 || currentStep.stepCurrent == currentStep.stepTotal)
        {
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
        }

        startCal_dir_type_span(&(calTag->calDirect), &(calTag->calTpye), &(calTag->calSpanPercent), &(calTag->calSpan) );

        calTag->step = currentStep.stepCurrent + 1;

        calTag->state = ONE_CAL_START;

        calTag->calTime = QDateTime::currentDateTime().toTime_t();
    }

    return true;
}

void FSC_MainWindow::calSingle(oneCalTag *calTag)
{
    QString str;

    if (calTag->state == ONE_CAL_START)
    {
        str += QString().sprintf("第(%d)步开始->", calTag->step);

        if (calTag->calDirect == START_CAL_DIRECT_FORWARD)
        {
            str += "打开正向进水阀->";
        }
        else if (calTag->calDirect == START_CAL_DIRECT_REVERSE)
        {
            str += "打开反向进水阀->";
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
                calTag->calTime = QDateTime::currentDateTime().toTime_t();

            }
            else
            {
                showSetFlowRate = calTag->calSpan;
                on_radioButton_setFlowRate_clicked();

                if (calTag->calDirect == START_CAL_DIRECT_FORWARD)
                {
                    if (showSetFlowRate > MAX_SPAN)
                    {
                        on_tbnPump1ForwardOn_clicked();
                        on_tbnPump2ForwardOn_clicked();
                    }
                    else if (showSetFlowRate > MIN_SPAN)
                    {
                         on_tbnPump2ForwardOn_clicked();
                    }
                    else
                    {
                        on_tbnPump1ForwardOn_clicked();
                    }

                }
                else if (calTag->calDirect == START_CAL_DIRECT_REVERSE)
                {
                    if (showSetFlowRate > MAX_SPAN)
                    {
                        on_tbnPump1ReverseOn_clicked();
                        on_tbnPump2ReverseOn_clicked();
                    }
                    else if (showSetFlowRate > MIN_SPAN)
                    {
                         on_tbnPump2ReverseOn_clicked();
                    }
                    else
                    {
                        on_tbnPump1ReverseOn_clicked();
                    }
                }

                calTag->state = ONE_CAL_GOING;

                //calOn = CAL_PLOT_START;
                oneCal.plotSelectedFMIndex  = ui->comboBox_PlotSenSel->currentIndex();
                oneCal.plotSelectedFMStr  = ui->comboBox_PlotSenSel->currentText();

                calTag->calTime = QDateTime::currentDateTime().toTime_t();
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

    if (calTag->plotPos++ > PLOT_VALUE_NUMBER)
    {
        calTag->state = ONE_CAL_POST_PROCESS;

        printInfoWithTime("本步结束->停泵->关闭进水阀");

        openOutValve();
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY);

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

    allCal[calTag->step - 1] = *calTag;

    allCalNeedToReport = true;
    allCalAvailable[calTag->step - 1]  = true;

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
    calTag->plotTimeX.clear();
    calTag->plotScaleSumValue.clear();
    calTag->plotSTDFMSumValue.clear();
    calTag->plotSTDFMRateValue.clear();

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        calTag->plotFMSumValue[i].clear();
        calTag->plotFMRateValue[i].clear();
    }

    if (calTag->pause)
    {
        calOn = CAL_STATE_STOP;
        calTag->pause = false;
    }

}

void FSC_MainWindow::calStop(oneCalTag *calTag)
{
    calTag->state = ONE_CAL_EMPTY;
    calTag->plotPos = 0;
    calTag->plotTimeX.clear();
    calTag->plotScaleSumValue.clear();
    calTag->plotSTDFMSumValue.clear();
    calTag->plotSTDFMRateValue.clear();

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        calTag->plotFMSumValue[i].clear();
        calTag->plotFMRateValue[i].clear();
    }

    calTag->step = 0;
    currentStep.stepCurrent = 0;

    pump1Off();
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);

    closeForwardValveAll();
    closeReverseValveAll();
    writePLC();
}

void FSC_MainWindow::makeCalRecordPrint(oneCalTag *calTag)
{
    if (!calTag) return;

    allCalNeedToReport = false;
    for(int i = 0; i < CAL_MAX_STEP; i++)
    {
        allCalAvailable[i]  = false;
    }


    printInfoWithTime("生成报表");
}

void FSC_MainWindow::showPlotFresh(void)
{
    ui->lineEdit_plotTime->setText(QString::number(oneCal.plotPos * 0.5, 'f', 1) + "s");
}

bool FSC_MainWindow::checkWaterEmpty(void)
{
    return true;
}

void FSC_MainWindow::on_comboBox_PlotSenSel_currentIndexChanged(const QString &arg1)
{
    oneCal.plotSelectedFMIndex  = ui->comboBox_PlotSenSel->currentIndex();
    oneCal.plotSelectedFMStr  = arg1;

    calPlot(&oneCal);
}

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
            if (currentStep.stepCurrent > currentStep.stepTotal)
            {
                currentStep.stepCurrent = currentStep.stepTotal;
            }
        }

        startCal_dir_type_span(&(calTag->calDirect), &(calTag->calTpye), &(calTag->calSpanPercent), &(calTag->calSpan) );

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
                        pump1On();
                        pump2On();
                        writePLC();

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
                        printInfoWithTime("->关闭正向进水阀");
                        closeForwardValveAll();
                        writePLC();
                        delayMSec(VALVE_EXCHANGE_DELAY);

                        printInfo("->打开反向进水阀");
                        openReverseValveAll();
                        writePLC();
                        delayMSec(VALVE_EXCHANGE_DELAY);

                        printInfoWithTime("->启动1#泵->启动2#泵");
                        pump1On();
                        pump2On();
                        writePLC();

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

                oneCal.plotSelectedFMIndex  = ui->comboBox_PlotSenSel->currentIndex();
                oneCal.plotSelectedFMStr  = ui->comboBox_PlotSenSel->currentText();

                calTag->calTime = QDateTime::currentDateTime().toTime_t();
                calTag->calProcStartTime = QDateTime::currentDateTime().toTime_t();

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

    if (calTag->stepRecord >= 0 && calTag->stepRecord < CAL_MAX_STEP)
    {
        calTag->stepBak = currentStep;

        if (calTag->stepRecord < 1) return;

        calTag->calDateTime = QDateTime::currentDateTime();
        allCal[calTag->stepRecord - 1] = *calTag;

    }

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
    if (calTag == nullptr) return;
    if (!allCalNeedToReport) return;

    QString fileName = "流量标定报表" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream stream(&file);

    QString str =  "报表生成时间：" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n";

    str += "型号：" + calTag->stepBak.type_name + "   量程：" + QString::number(calTag->stepBak.span_ml_per_min) + "ml/min   水温：25°C\n" ;
    str += "\n";

    oneCalTag *calTagTmp;
    for(int i = 0; i < CAL_MAX_STEP; i++)
    {
        calTagTmp = calTag + i;
        if (allCalAvailable[i])
        {
            str += "第(" + QString::number(i + 1) + ")步" + "   " + \
                    QString::number(calTagTmp->calSpanPercent) + "%量程("\
                    + QString::number((calTagTmp->calSpanPercent * calTag->stepBak.span_ml_per_min / 100), 'f',3) \
                    + "ml/min) ";


            if (calTagTmp->calDirect == START_CAL_DIRECT_FORWARD)
            {
                str += "正向 ";
            }
            else if (calTagTmp->calDirect == START_CAL_DIRECT_REVERSE)
            {
                str += "反向 ";
            }

            if (calTagTmp->calTpye == START_CAL_TYPE_CAL)
            {
                str += "标定 ";
            }
            else if (calTagTmp->calTpye == START_CAL_TYPE_CORRECT)
            {
                str += "修正 ";
            }
            else if (calTagTmp->calTpye == START_CAL_TYPE_CHECK)
            {
                str += "验证 ";
            }

            str += "时间：" + calTagTmp->calDateTime.toString("yyyy-MM-dd hh:mm:ss   ");

            str += "用时：" + QString::number(calTagTmp->calProcEndTime - calTagTmp->calProcStartTime);

            str += "s\n";

            str += "天平称重：" + QString::number(calTagTmp->finalScaleSumValue, 'f', 3) + "g\n";
            str += "标准流量计累计流量：" + QString::number(calTagTmp->finalSTDFMSumValue, 'f', 3) + "ml\n";
            str += "标准流量计最后流速：" + QString::number(calTagTmp->finalSTDFMRateValue, 'f', 3) + "ml/min\n";

            double d = 0;
            for (int i = 0; i < calTagTmp->plotSTDFMRateValue.size(); i++)
            {
                d += calTagTmp->plotSTDFMRateValue[i];
            }
            d /= calTagTmp->plotSTDFMRateValue.size();

            str += "标准流量计平均流速：" + QString::number(d, 'f', 3) + "ml/min\n";
            str += "\n";

            for (int i = 0; i < FLOWMETER_NUMBER; i++)
            {
                if (calTagTmp->finalFMSumValue[i] < 1.0 ||  std::isnan(calTagTmp->finalFMSumValue[i])) continue;

                str += QString::number(i + 1) + "#待检流量计累计流量：" + QString::number(calTagTmp->finalFMSumValue[i], 'f', 3) + "ml\n";
                str += QString::number(i + 1) + "#待检流量计最后流速：" + QString::number(calTagTmp->finalFMSumValue[i], 'f', 3) + "ml/min\n";

                d = 0;
                for (int j = 0; j < calTagTmp->plotFMRateValue[i].size(); j++)
                {
                    d += calTagTmp->plotFMRateValue[i][j];
                }
                d /= calTagTmp->plotFMRateValue[i].size();

                str += QString::number(i + 1) + "#待检流量计平均流速：" + QString::number(d, 'f', 3) + "ml/min\n";

                d = calTagTmp->finalFMSumValue[i] - calTagTmp->finalScaleSumValue;
                str += "与天平误差：" + QString::number(d, 'f', 3) + "ml/min\n";
                str += "误差百分比：" + QString::number(d / calTagTmp->finalScaleSumValue * 100, 'f', 3) + "% 已";

                if (calTagTmp->calTpye == START_CAL_TYPE_CAL)
                {
                    str += "标定";
                }
                else if (calTagTmp->calTpye == START_CAL_TYPE_CORRECT)
                {
                    str += "修正";
                }
                else if (calTagTmp->calTpye == START_CAL_TYPE_CHECK)
                {
                    str += "验证";
                }

                str += "完成\n\n";
            }

        }
    }

    str += "结束";

    QByteArray bUtf8 = str.toUtf8();

    stream << bUtf8;

    file.close();

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
    oneCal.plotSelectedFMIndex  = ui->comboBox_PlotSenSel->currentIndex();
    oneCal.plotSelectedFMStr  = arg1;

    calPlot(&oneCal);
}

void FSC_MainWindow::on_tbnCalManual_clicked()
{
    calGoingInfoLabRemove();

    if (!calManualDoing)
    {
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(1000);

        printInfoWithTime("请放空天平上容器->关闭放水阀->天平去皮清零->在手动控制区设定流量或设定占空比->正向或反向进水启动水泵");

        calManualDoing = true;

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

        plotFresh();

        if (plotLoop++ > plotPosNumber)
        {
            calOn = CAL_STATE_STOP;

            pump1Off();
            pump2Off();
            writePLC();
            delayMSec(1000);
            printInfoWithTime("进水过程结束，请手动填入并写入参数值");
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
    calTag->plotTimeX.clear();
    calTag->plotScaleSumValue.clear();
    calTag->plotSTDFMSumValue.clear();
    calTag->plotSTDFMRateValue.clear();

    for (int i = 0; i < FLOWMETER_NUMBER; i++)
    {
        calTag->plotFMSumValue[i].clear();
        calTag->plotFMRateValue[i].clear();
    }

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



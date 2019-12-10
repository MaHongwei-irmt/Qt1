#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"

/*
void FSC_MainWindow::startCal(void)
{
    QString str;

//    if (calOn)
//    {
//        cal
//        calProcess();
//    }

    switch (calOn)
    {
    case CAL_START:

        {
            on_tbnPoltClear_clicked();

            str = ui->textBrow_calInfo->toPlainText();
            str += "\r\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
            str += "启动标定->";
            str += "清空曲线图->";
            str += "关闭所有阀门->";
            str += "稳定3秒...\r\n";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_START_BALANCE;
            calOnTime = QDateTime::currentDateTime().toTime_t();
        }

        break;

    case CAL_START_BALANCE:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 2)
        {

            on_tbnScaleZero_clicked();

            str = ui->textBrow_calInfo->toPlainText();
            str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
            str += "天平清零->";
            str += "稳定3秒...\r\n";

            ui->textBrow_calInfo->setText(str);


            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_SCALE_ZERO;
            calOnTime = QDateTime::currentDateTime().toTime_t();
        }

        break;

    case CAL_SCALE_ZERO:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 2)
        {

            calOn = CAL_SCALE_ZERO_BALANCE;
            calOnTime = QDateTime::currentDateTime().toTime_t();

        }

        break;

    case CAL_SCALE_ZERO_BALANCE:

        {
            startCal_dir_type_span(&currentStep.startDirect, &currentStep.startType, \
                                   &currentStep.startSpanPercent, &currentStep.startSpan);

            str = ui->textBrow_calInfo->toPlainText();
            str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");

            str += QString().sprintf("(%d)->", currentStep.stepCurrent + 1);

            if (currentStep.startDirect == START_CAL_DIRECT_FORWARD)
            {
                str += "打开正向进水阀->";
            }
            else if (currentStep.startDirect == START_CAL_DIRECT_REVERSE)
            {
                str += "打开反向进水阀->";
            }

            if (currentStep.startType == START_CAL_TYPE_CAL)
            {
                str += "标定->";
            }
            else if (currentStep.startType == START_CAL_TYPE_CORRECT)
            {
                str += "修正->";
            }
            else if (currentStep.startType == START_CAL_TYPE_CHECK)
            {
                 str += "验证->";
            }

            str += QString().sprintf("%d", currentStep.startSpanPercent);
            str += "%量程->";
            str += QString().sprintf("%0.3fml/min->", currentStep.startSpan);

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_OPEN_VALVE;
            calOnTime = QDateTime::currentDateTime().toTime_t();

            break;

    }

    case CAL_OPEN_VALVE:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 1)
        {

            str = ui->textBrow_calInfo->toPlainText();
            str += "启动水泵->";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_OPEN_PUMP;
            calOnTime = QDateTime::currentDateTime().toTime_t();

        }

        break;


    case CAL_OPEN_PUMP:

        {
            str = ui->textBrow_calInfo->toPlainText();
            str += "开始绘图...\r\n";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_PLOT_START;
            calOnTime = QDateTime::currentDateTime().toTime_t();

            PlotReplay(ui->comboBox_PlotSenSel->currentText());

        }

        break;

    }

}

*/

void FSC_MainWindow::startCal(void)
{
    static bool firstCal = true;
    QString str;

    switch (calOn)
    {
    case CAL_START:

        printInfoWithTime("启动标定->清空曲线图->打开放水阀");
        on_tbnPoltClear_clicked();
        //on_tbnVOutOpen_clicked();
        printInfoWithTime("放水阀打开");
        openOutValve();
        delayMSec(VALVE_EXCHANGE_DELAY);
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
        calOn = CAL_START_OUT_VALVE_OPEN;
        calOnTime = QDateTime::currentDateTime().toTime_t();

        break;

    case CAL_START_OUT_VALVE_OPEN:

        calOnTime = QDateTime::currentDateTime().toTime_t();
        if (firstCal)
        {
            firstCal = false;
            showSetPWM = 30;
            on_radioButton_setPWM_clicked();
            printInfoWithTime("->正向进水管路充水");
            on_tbnPump1ForwardOn_clicked();
            delayMSec(PUMP_START_DELAY);
            on_tbnPump2ForwardOn_clicked();
            printInfoWithTime("->充水10秒...");
            calOn = CAL_START_FIRST_FORWARD_FILL_WATER;
        }
        else
        {
            calOn = CAL_START_FIRST_REVERSE_FILL_WATER_END;
        }

        break;

    case CAL_START_FIRST_FORWARD_FILL_WATER:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 9)
        {
            calOnTime = QDateTime::currentDateTime().toTime_t();
            printInfoWithTime("->反向进水管路充水");
            on_tbnPump1ForwardOff_clicked();
            delayMSec(PUMP_START_DELAY);
            on_tbnPump2ForwardOff_clicked();
            delayMSec(PUMP_START_DELAY);
            on_tbnPump1ReverseOn_clicked();
            delayMSec(PUMP_START_DELAY);
            on_tbnPump2ReverseOn_clicked();
            printInfoWithTime("->充水10秒...");
            calOn = CAL_START_FIRST_REVERSE_FILL_WATER;

        }

        break;

    case CAL_START_FIRST_REVERSE_FILL_WATER:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 9)
        {
            calOnTime = QDateTime::currentDateTime().toTime_t();
            on_tbnPump1ReverseOff_clicked();
            delayMSec(PUMP_START_DELAY);
            on_tbnPump2ReverseOff_clicked();
            delayMSec(PUMP_START_DELAY);
            on_radioButton_setFlowRate_clicked();
            printInfoWithTime("->等待放水结束..");
            calOn = CAL_START_FIRST_REVERSE_FILL_WATER_END;

        }

        break;

    case CAL_START_FIRST_REVERSE_FILL_WATER_END:

        //printInfoWithTime("->等待放水结束..");
        //on_tbnPump1ReverseOn_clicked();
//        calOn = CAL_START_FIRST_REVERSE_FILL_WATER_END;
//        calOnTime = QDateTime::currentDateTime().toTime_t();

        break;

    case CAL_SCALE_ZERO_BALANCE:

        {
            startCal_dir_type_span(&currentStep.startDirect, &currentStep.startType, \
                                   &currentStep.startSpanPercent, &currentStep.startSpan);

            str = ui->textBrow_calInfo->toPlainText();
            str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");

            str += QString().sprintf("(%d)->", currentStep.stepCurrent + 1);

            if (currentStep.startDirect == START_CAL_DIRECT_FORWARD)
            {
                str += "打开正向进水阀->";
            }
            else if (currentStep.startDirect == START_CAL_DIRECT_REVERSE)
            {
                str += "打开反向进水阀->";
            }

            if (currentStep.startType == START_CAL_TYPE_CAL)
            {
                str += "标定->";
            }
            else if (currentStep.startType == START_CAL_TYPE_CORRECT)
            {
                str += "修正->";
            }
            else if (currentStep.startType == START_CAL_TYPE_CHECK)
            {
                 str += "验证->";
            }

            str += QString().sprintf("%d", currentStep.startSpanPercent);
            str += "%量程->";
            str += QString().sprintf("%0.3fml/min->", currentStep.startSpan);

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_OPEN_VALVE;
            calOnTime = QDateTime::currentDateTime().toTime_t();

            break;

    }

    case CAL_OPEN_VALVE:

        if(QDateTime::currentDateTime().toTime_t()- calOnTime > 1)
        {

            str = ui->textBrow_calInfo->toPlainText();
            str += "启动水泵->";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_OPEN_PUMP;
            calOnTime = QDateTime::currentDateTime().toTime_t();

        }

        break;


    case CAL_OPEN_PUMP:

        {
            str = ui->textBrow_calInfo->toPlainText();
            str += "开始绘图...\r\n";

            ui->textBrow_calInfo->setText(str);

            ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);

            calOn = CAL_PLOT_START;
            calOnTime = QDateTime::currentDateTime().toTime_t();

            PlotReplay(ui->comboBox_PlotSenSel->currentText());

        }

        break;

    }

}





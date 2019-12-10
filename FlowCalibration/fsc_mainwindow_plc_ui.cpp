#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"
#include "fsc_plc.h"

void FSC_MainWindow::on_tbnVOutOpen_clicked()
{
    printInfoWithTime("放水阀打开");
    openOutValve();
    writePLC();
}

void FSC_MainWindow::on_tbnVOutClose_clicked()
{
    printInfoWithTime("放水阀关闭");
    closeOutValve();
    writePLC();
}

void FSC_MainWindow::on_tbnVForwardIn1Open_clicked()
{
    printInfoWithTime("1#正向进水阀打开");
    openForwardValve1();
    writePLC();
}

void FSC_MainWindow::on_tbnVForwardIn2Open_clicked()
{
    printInfoWithTime("2#正向进水阀打开");
    openForwardValve2();
    writePLC();
}

void FSC_MainWindow::on_tbnVReverseIn1Open_clicked()
{
    printInfoWithTime("1#反向进水阀打开");
    openReverseValve1();
    writePLC();
}

void FSC_MainWindow::on_tbnVReverseIn2Open_clicked()
{
    printInfoWithTime("2#反向进水阀打开");
    openReverseValve2();
    writePLC();
}

void FSC_MainWindow::on_tbnVForwardIn1Close_clicked()
{
    if(statePump1() || statePump2())
    {
        printInfoWithTime("先停止泵！");

        return;
    }
    printInfoWithTime("1#正向进水阀关闭");
    closeForwardValve1();
    writePLC();
}

void FSC_MainWindow::on_tbnVForwardIn2Close_clicked()
{
    if(statePump1() || statePump2())
    {
        printInfoWithTime("先停止泵！");

        return;
    }

    printInfoWithTime("2#正向进水阀关闭");
    closeForwardValve2();
    writePLC();
}

void FSC_MainWindow::on_tbnVReverseIn1Close_clicked()
{
    if(statePump1() || statePump2())
    {
        printInfoWithTime("先停止泵！");

        return;
    }

    printInfoWithTime("1#反向进水阀关闭");
    closeReverseValve1();
    writePLC();

}

void FSC_MainWindow::on_tbnVReverseIn2Close_clicked()
{
    if(statePump1() || statePump2())
    {
        printInfoWithTime("先停止泵！");

        return;
    }

    printInfoWithTime("2#反向进水阀关闭");
    closeReverseValve2();
    writePLC();
}

void FSC_MainWindow::on_tbnPump1ForwardOn_clicked()
{
    if (statePump2() && (stateForwardV1() || stateForwardV2()))
    {
        printInfoWithTime("启动1#泵");
        pump1On();
        writePLC();
        return;
    }

    if ( (statePump2() || statePump1()) && (stateReverseV1() || stateReverseV2()))
    {
        printInfoWithTime("停止所有水泵");
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY);

        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);

        printInfoWithTime("->打开正向进水阀");
        openForwardValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);
    }
    else
    {
        printInfoWithTime("->关闭反向进水阀");
        closeReverseValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

        printInfoWithTime("打开正向进水阀");
        openForwardValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);
    }

    printInfoWithTime("->启动1#泵");
    pump1On();
    writePLC();
}

void FSC_MainWindow::on_tbnPump1ReverseOn_clicked()
{
    if (statePump2() && (stateReverseV1() || stateReverseV2()))
    {
        printInfoWithTime("启动1#泵");
        pump1On();
        writePLC();
        return;
    }

    if ( (statePump2() || statePump1()) && (stateForwardV1() || stateForwardV2()))
    {
        printInfoWithTime("停止所有水泵");
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY);


        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);

        printInfoWithTime("->打开反向进水阀");
        openReverseValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

    }
    else
    {
        printInfoWithTime("->关闭正向进水阀");
        closeForwardValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

        printInfoWithTime("->打开反向进水阀");
        openReverseValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);
    }

    printInfoWithTime("->启动1#泵");
    pump1On();
    writePLC();
}

void FSC_MainWindow::on_tbnPump1ForwardOff_clicked()
{
    printInfoWithTime("停止1#泵");
    pump1Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);

    if(!statePump1() && !statePump2())
    {
        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
    }

}

void FSC_MainWindow::on_tbnPump1ReverseOff_clicked()
{
    printInfoWithTime("停止1#泵");
    pump1Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);


    if(!statePump1() && !statePump2())
    {
        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
    }

}


void FSC_MainWindow::on_tbnPump2ForwardOn_clicked()
{
    if (statePump1() && (stateForwardV1() || stateForwardV2()))
    {
        printInfoWithTime("启动2#泵");
        pump2On();
        writePLC();
        return;
    }

    if ( (statePump2() || statePump1()) && (stateReverseV1() || stateReverseV2()))
    {
        printInfoWithTime("停止所有水泵");
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY);


        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);

        printInfoWithTime("->打开正向进水阀");
        openForwardValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

    }
    else
    {
        printInfoWithTime("->关闭反向进水阀");
        closeReverseValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

        printInfoWithTime("打开正向进水阀");
        openForwardValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);

    }

    printInfoWithTime("->启动2#泵");
    pump2On();
    writePLC();
}

void FSC_MainWindow::on_tbnPump2ReverseOn_clicked()
{
    if (statePump1() && (stateReverseV1() || stateReverseV2()))
    {
        printInfoWithTime("启动2#泵");
        pump2On();
        writePLC();
        return;
    }

    if ( (statePump2() || statePump1()) && (stateForwardV1() || stateForwardV2()))
    {
        printInfoWithTime("停止所有水泵");
        pump1Off();
        pump2Off();
        writePLC();
        delayMSec(PUMP_START_DELAY);


        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);

        printInfoWithTime("->打开反向进水阀");
        openReverseValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

    }
    else
    {
        printInfoWithTime("->关闭正向进水阀");
        closeForwardValveAll();
        writePLC();
        delayMSec(PUMP_START_DELAY);

        printInfoWithTime("打开反向进水阀");
        openReverseValveAll();
        writePLC();
        delayMSec(VALVE_EXCHANGE_DELAY);
    }

    printInfoWithTime("->启动2#泵");
    pump2On();
    writePLC();
}

void FSC_MainWindow::on_tbnPump2ForwardOff_clicked()
{
    printInfoWithTime("停止2#泵");
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);

    if(!statePump1() && !statePump2())
    {
        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
    }
}

void FSC_MainWindow::on_tbnPump2ReverseOff_clicked()
{
    printInfoWithTime("停止2#泵");
    pump2Off();
    writePLC();
    delayMSec(PUMP_START_DELAY);

    if(!statePump1() && !statePump2())
    {
        printInfoWithTime("->关闭所有进水阀");
        closeForwardValveAll();
        closeReverseValveAll();
        writePLC();
    }
}


void FSC_MainWindow::on_lineEdit_setFlowRate_textChanged(const QString &arg1)
{
    (void)arg1;

    showSetFlowRate = ui->lineEdit_setFlowRate->text().toDouble();

    writePLC();
}

void FSC_MainWindow::on_lineEdit_setPWM_textChanged(const QString &arg1)
{
    (void)arg1;

    showSetPWM = ui->lineEdit_setPWM->text().toInt();

    writePLC();
}

void FSC_MainWindow::on_radioButton_setFlowRate_clicked()
{
    writePLC();
}

void FSC_MainWindow::on_radioButton_setPWM_clicked()
{
    writePLC();
}

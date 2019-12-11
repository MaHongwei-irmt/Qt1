#include "fsc_mainwindow.h"
#include "ui_fsc_mainwindow.h"
#include "fsc_plc.h"

void FSC_MainWindow::showPlcFresh(void)
{
    ui->tbnVOutOpen->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVOutClose->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn1Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn1Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn2Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVForwardIn2Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn1Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn1Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn2Open->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnVReverseIn2Close->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );

    ui->tbnPump1ForwardOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump1ForwardOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump1ReverseOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump1ReverseOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );

    ui->tbnPump2ForwardOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump2ForwardOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump2ReverseOn->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );
    ui->tbnPump2ReverseOff->setEnabled(sktConed[SOCKET_PLC_INDEX] && (calOn == CAL_STATE_STOP) );


    ui->lineEdit_setFlowRate->setEnabled(revdSketPLC);
    ui->lineEdit_setPWM->setEnabled(revdSketPLC);
    ui->radioButton_setFlowRate->setEnabled(revdSketPLC);
    ui->radioButton_setPWM->setEnabled(revdSketPLC);
    if (!ui->radioButton_setFlowRate->isChecked())
    {
        ui->lineEdit_setFlowRate->setEnabled(false);
    }
    if (!ui->radioButton_setPWM->isChecked())
    {
        ui->lineEdit_setPWM->setEnabled(false);
    }

    ui->radioButton_setFlowRate->setChecked(flowRateOrPWM);
    ui->radioButton_setPWM->setChecked(!flowRateOrPWM);

    if( (statePump1() || statePump2()))
    {
        ui->tbnVForwardIn1Open->setEnabled(false);
        ui->tbnVForwardIn1Close->setEnabled(false);
        ui->tbnVForwardIn2Open->setEnabled(false);
        ui->tbnVForwardIn2Close->setEnabled(false);
        ui->tbnVReverseIn1Open->setEnabled(false);
        ui->tbnVReverseIn1Close->setEnabled(false);
        ui->tbnVReverseIn2Open->setEnabled(false);
        ui->tbnVReverseIn2Close->setEnabled(false);
    }


    if ( statePump1() && stateForwardV1() && stateForwardV2() && !stateReverseV1() && !stateReverseV2())
    {
        ui->label_pump1State->setText("1#泵正向进水");

    }
    if ( statePump1() && stateReverseV1() && stateReverseV2() && !stateForwardV1() && !stateForwardV2())
    {
        ui->label_pump1State->setText("1#泵反向进水");
    }

    if ( statePump2() && stateForwardV1() && stateForwardV2() && !stateReverseV1() && !stateReverseV2())
    {
        ui->label_pump2State->setText("2#泵正向进水");
    }
    if ( statePump2() && stateReverseV1() && stateReverseV2() && !stateForwardV1() && !stateForwardV2())
    {
        ui->label_pump2State->setText("2#泵反向进水");
    }

    if (!statePump1())
    {
        ui->label_pump1State->setText("1#泵停止");
    }
    if (!statePump2())
    {
        ui->label_pump2State->setText("2#泵停止");
    }


    if( (statePump1() || statePump2()) && ( stateForwardV1() || stateForwardV2()) \
            && (stateReverseV1() || stateReverseV2()) )
    {
        printInfoWithTime("报警：阀位不对，关闭泵！");
        pump1Off();
        pump2Off();
    }

    if( (statePump1() || statePump2()) && ( !stateForwardV1() || !stateForwardV2()) \
            && (!stateReverseV1() || !stateReverseV2()) )
    {
        printInfoWithTime("报警：阀位不对，关闭泵！");
        pump1Off();
        pump2Off();
    }

    ui->revFlowRate->setText(QString::number(static_cast<double>(plcRevFlowRate), 'f', 0));
    ui->revPWM->setText(QString::number(plcRevPWM));
    ui->labelplcState->setText(PRINT_PLC_STATE_WRITE);
    ui->labelplcStateRead->setText(PRINT_PLC_STATE_READ);

    ui->label_stateVOutOn->setVisible( (plcStateRead >> PLC_OUT_VALVE1_OFFSET) & 0x01);
    ui->label_stateVOutOff->setVisible( !((plcStateRead >> PLC_OUT_VALVE1_OFFSET) & 0x01));

    ui->label_stateVFInOn1->setVisible(stateForwardV1());
    ui->label_stateVFInOff1->setVisible(!stateForwardV1());

    ui->label_stateVFInOn2->setVisible(stateForwardV2());
    ui->label_stateVFInOff2->setVisible(!stateForwardV2());

    ui->label_stateVRInOn1->setVisible(stateReverseV1());
    ui->label_stateVRInOff1->setVisible(!stateReverseV1());

    ui->label_stateVRInOn2->setVisible(stateReverseV2());
    ui->label_stateVRInOff2->setVisible(!stateReverseV2());

}

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
    flowRateOrPWM = true;
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));
    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    writePLC();
}

void FSC_MainWindow::on_radioButton_setPWM_clicked()
{
    flowRateOrPWM = false;
    ui->lineEdit_setPWM->setText(QString::number(showSetPWM));
    ui->lineEdit_setFlowRate->setText(QString::number(showSetFlowRate, 'f', 0));
    writePLC();
}



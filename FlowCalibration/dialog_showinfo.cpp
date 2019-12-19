#include "dialog_showinfo.h"
#include "ui_dialog_showinfo.h"
#include "fsc_mainwindow.h"

#include <QTimer>
#include <QFileDialog>
#include <QTextCodec>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>



Dialog_showinfo::Dialog_showinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_showinfo)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = this->windowFlags()  | Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint );

    ui->groupBox->setStyleSheet("QGroupBox{border:none;padding:0px;spacing:0;margin:0px;}");

    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    if (mainWin->showCalTable)
    {
        this->setWindowTitle("查看报表");

        ui->groupBox->setVisible(true);

        autoPrinter = mainWin->autoPrinter;
        ui->checkBox_printer->setCheckState(autoPrinter);

        if (mainWin->infoPrinter != nullptr)
        {
            printer = mainWin->printer;
            infoPrinter = mainWin->infoPrinter;


            ui->lineEdit_printer->setText(infoPrinter->printerName());
        }

        mainWin->showCalTable = false;
        showCalTable();
    }
    else
    {
        ui->groupBox->setVisible(false);
        ui->textBrow_calInfo->setText(mainWin->txtBrow_showInfo->toPlainText());

        freshTimer = new QTimer(this);
        connect(freshTimer, SIGNAL(timeout()), this, SLOT(showInfo()));
        freshTimer->start(1000);
    }
}

Dialog_showinfo::~Dialog_showinfo()
{
    if (freshTimer)
    {
        freshTimer->stop();
        delete freshTimer;
    }

    delete ui;
}

void Dialog_showinfo::showInfo(void)
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());
    if (mainWin->showInfoUpdata)
    {
        ui->textBrow_calInfo->setText(mainWin->txtBrow_showInfo->toPlainText());
        mainWin->showInfoUpdata = false;
    }
}

void Dialog_showinfo::showCalTable(void)
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());
    QFile file(mainWin->calTableName);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {

        QByteArray t = file.readAll();

        QTextCodec *codec=QTextCodec::codecForLocale();
        ui->textBrow_calInfo->setText(codec->toUnicode(t));

        file.close();
    }
}

void Dialog_showinfo::on_pushButton_showCalTable_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("选择报表文件"),"",tr("TXT(*.txt)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);

    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {

        QByteArray t = file.readAll();

        QTextCodec *codec=QTextCodec::codecForLocale();
        ui->textBrow_calInfo->setText(codec->toUnicode(t));

        file.close();
    }
}

void Dialog_showinfo::on_pushButton_print_clicked()
{
//    if (printer == nullptr)
//    {
//        on_pushButton_printerSelect_clicked();
//    }

//    if (printer)
//    {
//        QTextDocument td;
//        td.setPlainText(ui->textBrow_calInfo->toPlainText());
//        if (td.toPlainText().size() > 0)
//        {
//            td.print(printer);
//        }
//    }


//    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

//    QPrintDialog printDialog;
//    if (printDialog.exec() == QDialog::Accepted)
//    {
//        printer = printDialog.printer();
//        infoPrinter = new QPrinterInfo(*printer);

//        mainWin->printer = printer;
//        mainWin->infoPrinter = infoPrinter;

//        ui->lineEdit_printer->setText(infoPrinter->printerName());


//                QTextDocument td;
//                td.setPlainText(ui->textBrow_calInfo->toPlainText());
//                if (td.toPlainText().size() > 0)
//                {
//                    td.print(printer);
//                }
//    }

    printer = new QPrinter();


        if (printer)
        {

            QString str = infoPrinter->printerName();

            printer->setPrinterName(str);
            QTextDocument td;
            td.setPlainText(ui->textBrow_calInfo->toPlainText());
            if (td.toPlainText().size() > 0)
            {
                td.print(printer);
            }
        }


}

void Dialog_showinfo::on_checkBox_printer_stateChanged(int arg1)
{
    (void)arg1;
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    autoPrinter = ui->checkBox_printer->checkState();
    mainWin->autoPrinter = ui->checkBox_printer->checkState();
}

void Dialog_showinfo::on_pushButton_printerSelect_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    QPrintDialog printDialog;
    if (printDialog.exec() == QDialog::Accepted)
    {
        printer = printDialog.printer();
        infoPrinter = new QPrinterInfo(*printer);

        mainWin->printer = printer;
        mainWin->infoPrinter = infoPrinter;

        ui->lineEdit_printer->setText(infoPrinter->printerName());
    }
}

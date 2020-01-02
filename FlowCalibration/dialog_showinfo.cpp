#include "dialog_showinfo.h"
#include "ui_dialog_showinfo.h"
#include "fsc_mainwindow.h"

#include <QTimer>
#include <QFileDialog>
#include <QTextCodec>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QMessageBox>


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
        this->setWindowTitle("报表窗");

        ui->groupBox->setVisible(true);

        autoPrinter = mainWin->autoPrinter;
        ui->checkBox_printer->setCheckState(autoPrinter);

        printerName = mainWin->printerName;
        ui->lineEdit_printer->setText(mainWin->printerName);

        autoPrinterBool = mainWin->autoPrinterBool;

        mainWin->showCalTable = false;
        showCalTable();
    }
    else
    {
        ui->groupBox->setVisible(false);
        ui->textBrow_calInfo->setText(mainWin->txtBrow_showInfo->toPlainText());

        freshTimer = new QTimer(this);
        connect(freshTimer, SIGNAL(timeout()), this, SLOT(showInfo()));
        freshTimer->start(800);
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
    //if (mainWin->showInfoUpdata)
    {
        ui->textBrow_calInfo->setText(mainWin->txtBrow_showInfo->toPlainText());
        mainWin->showInfoUpdata = false;

        ui->textBrow_calInfo->moveCursor(ui->textBrow_calInfo->textCursor().End);
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

        if (ui->textBrow_calInfo->toPlainText().contains(RECORD_FILE_TO_BE_CONTINUE))
        {
            QFile::remove(mainWin->calTableName);
        }
    }
    else
    {
        ui->textBrow_calInfo->setText("无报表");
    }
}

void Dialog_showinfo::on_pushButton_showCalTable_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("select record file"),"",tr("TXT(*.txt)"));
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
    if (printerName.size() > 0)
    {
        QPrinter ptr;
        ptr.setPrinterName(printerName);

        QTextDocument td;
        td.setPlainText(ui->textBrow_calInfo->toPlainText());

        if (td.toPlainText().size() > 10)
        {
            td.print(&ptr);
        }
    }
    else
    {
        QMessageBox::information(nullptr, "打印", "请先选择打印机.", QMessageBox::Yes);
    }
}

void Dialog_showinfo::on_checkBox_printer_stateChanged(int arg1)
{
    (void)arg1;
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    autoPrinter = ui->checkBox_printer->checkState();
    if (autoPrinter == Qt::Unchecked)
    {
        autoPrinterBool = false;
    }
    else
    {
        autoPrinterBool = true;
    }

    mainWin->autoPrinter = ui->checkBox_printer->checkState();
    mainWin->autoPrinterBool = autoPrinterBool;

    QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);
    configIni->setValue("IP_ADDRESS/PRINTER_AUTO",  autoPrinterBool);
    delete configIni;
}

void Dialog_showinfo::on_pushButton_printerSelect_clicked()
{
    FSC_MainWindow *mainWin = static_cast<FSC_MainWindow*>(parentWidget());

    QPrintDialog printDialog;
    if (printDialog.exec() == QDialog::Accepted)
    {

        QPrinter        *printer = nullptr;
        QPrinterInfo    *infoPrinter = nullptr;

        printer = printDialog.printer();
        infoPrinter = new QPrinterInfo(*printer);
        printerName = infoPrinter->printerName();

        mainWin->printerName = infoPrinter->printerName();

        ui->lineEdit_printer->setText(infoPrinter->printerName());

        QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);
        configIni->setValue("IP_ADDRESS/PRINTER_NAME",  printerName);
        delete configIni;
    }

}

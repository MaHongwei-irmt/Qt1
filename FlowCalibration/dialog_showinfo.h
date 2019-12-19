#ifndef DIALOG_SHOWINFO_H
#define DIALOG_SHOWINFO_H

#include <QDialog>
#include <QPrinter>

namespace Ui {
class Dialog_showinfo;
}

class Dialog_showinfo : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_showinfo(QWidget *parent = nullptr);
    ~Dialog_showinfo();

private slots:
    void showInfo(void);

    void on_pushButton_showCalTable_clicked();

    void on_pushButton_print_clicked();

    void on_checkBox_printer_stateChanged(int arg1);

    void on_pushButton_printerSelect_clicked();

private:
    Ui::Dialog_showinfo *ui;

    QTimer          *freshTimer = nullptr;
    QPrinter        *printer = nullptr;
    QPrinterInfo    *infoPrinter = nullptr;
    Qt::CheckState  autoPrinter = Qt::Unchecked;

    void showCalTable(void);

};

#endif // DIALOG_SHOWINFO_H
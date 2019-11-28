#ifndef DIALOG_CHECKDEV_H
#define DIALOG_CHECKDEV_H

#include <QDialog>

namespace Ui {
class Dialog_CheckDev;
}

class Dialog_CheckDev : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_CheckDev(QWidget *parent = nullptr);
    ~Dialog_CheckDev();

private slots:
    void on_pushButton_12_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Dialog_CheckDev *ui;
};

#endif // DIALOG_CHECKDEV_H

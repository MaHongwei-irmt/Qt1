#ifndef DIALOG_SENSORADDR_H
#define DIALOG_SENSORADDR_H

#include <QDialog>
#include <QStandardItemModel>
#include "fsc_pub.h"


namespace Ui {
class Dialog_SensorAddr;
}

class Dialog_SensorAddr : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SensorAddr(QWidget *parent = nullptr);
    ~Dialog_SensorAddr();

    int         fmIdx = 0;
    cls_fmData  fmData;



    void setFmName(int i);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_readRegister_clicked();

    void on_pushButton_freshTable_clicked();

    void on_pushButton_writeRegister_clicked();

private:
    Ui::Dialog_SensorAddr *ui;

    QStandardItemModel* model;

    int row = 0;
    int rowAll = 0;


protected:
     void closeEvent(QCloseEvent *event);
};

#endif // DIALOG_SENSORADDR_H

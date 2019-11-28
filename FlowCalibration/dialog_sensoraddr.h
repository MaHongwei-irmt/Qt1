#ifndef DIALOG_SENSORADDR_H
#define DIALOG_SENSORADDR_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class Dialog_SensorAddr;
}

class Dialog_SensorAddr : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SensorAddr(QWidget *parent = nullptr);
    ~Dialog_SensorAddr();

private:
    Ui::Dialog_SensorAddr *ui;

    QStandardItemModel* model;

};

#endif // DIALOG_SENSORADDR_H

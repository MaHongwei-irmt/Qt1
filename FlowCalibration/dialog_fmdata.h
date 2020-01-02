#ifndef DIALOG_FMDATA_H
#define DIALOG_FMDATA_H

#include <QDialog>
#include "fsc_pub.h"
#include <QLineEdit>
#include <QLabel>
#include <QDoubleValidator>


namespace Ui {
class Dialog_fmdata;
}

class Dialog_fmdata : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_fmdata(QWidget *parent = nullptr);
    ~Dialog_fmdata();

    int         fmIdx = 0;
    cls_fmData  fmData;

    void setFmName(int i);

private slots:
    void on_tbnRead_clicked();

    void on_tbnWrite_clicked();

    void on_tbnParaAdd_clicked();

private:
    Ui::Dialog_fmdata *ui;

    QLineEdit           *lineEdit_SET_KF1[XUNYIN_SET_KF_NUM];
    QLabel              *label_SET_KF1[XUNYIN_SET_KF_NUM];

    QDoubleValidator    *pDfValidator[XUNYIN_SET_KF_NUM];
};

#endif // DIALOG_FMDATA_H

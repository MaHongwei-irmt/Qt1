#ifndef FCADDRESS_H
#define FCADDRESS_H

#include <QDialog>
#include <QEventLoop>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class FCAddress;
}

class FCAddress : public QDialog
{
    Q_OBJECT

public:
    explicit FCAddress(QWidget *parent = 0);
    ~FCAddress();
    void InitUI();
    void InitConnect();
    int exec();
    void closeEvent(QCloseEvent *);
private slots:
    void OnOk();
    void OnCancel();
signals:
    void SendAddress(QVector<int>);
private:
    Ui::FCAddress *ui;
    QEventLoop *m_eventLoop;
    QVector<int> Address;
    QPixmap ImageFileOK=tr(":/Image/OK.png");
    QPixmap ImageFileCancel=tr(":/Image/Cancel.png");
};

#endif // FCADDRESS_H

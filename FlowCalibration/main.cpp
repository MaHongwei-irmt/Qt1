#include "FSC_MainWindow.h"
#include <QApplication>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(outputMessage);
    FSCLOG << "\r\nstart";

    FSC_MainWindow w;
    w.setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    w.show();

    return a.exec();
}

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    (void)type;

    QString context_info = QString("%1:%2:").arg(QString(context.file)).arg(context.line);
    QString message = QString("%1 %2").arg(context_info).arg(msg);
    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");

    QFile file(current_date + "-log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

QByteArray HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;

    HexString = HexString.trimmed();
    HexString = HexString.simplified();

    QStringList sl = HexString.split(" ");

    foreach (QString s, sl)
    {
        if(!s.isEmpty())
        {
            char c = static_cast<char> (s.toInt(&ok, 16) & 0xFF);

            if(ok)
            {
                ret.append(c);
            }
        }
    }
    return ret;
}

QString ByteArrayToHexString(QByteArray &ba)
{
    QDataStream out(&ba, QIODevice::ReadWrite);
    QString buf;

    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");
        buf += str;
    }
    return buf;
}

uint16_t Checksum_computeChecksum(const char *buffer, int length)
{
    uint16_t i;
    uint16_t crc = 0xFFFF;
    if (length == 0) length = 1;

    while (length--) {
       crc ^= *buffer;
       for (i = 0; i<8; i++)
       {
          if (crc & 1) {
            crc >>= 1;
            crc ^= 0xA001;
          } else {
            crc >>= 1;
          }
       }
       buffer++;
    }

    return crc;
}

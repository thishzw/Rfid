#ifndef RFIDREADERTHREAD_H
#define RFIDREADERTHREAD_H

#include <QObject>
#include<QThread>
#include<QtSerialPort/QSerialPort>
class RFIDReaderThread : public QThread
{
    Q_OBJECT
public:
    RFIDReaderThread(QObject* ptr,QSerialPort* in_com,qint64 sleepTime);
private:
    void run();
    qint64 time;
    QSerialPort *com;
};

#endif // RFIDREADERTHREAD_H

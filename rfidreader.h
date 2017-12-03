#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <QObject>
#include<QThread>
#include<QtSerialPort/QSerialPort>
#include"rfidreaderthread.h"
#include<QMessageBox>
#include<QVector>
class RFIDReader : public QObject
{
    Q_OBJECT
public:
    explicit RFIDReader(QObject *parent = 0);
    enum domain{Reserved=0,EPC=1,TID=2,User=3};
    void setCom(QString comName);
    bool run();
    void newtry();
    void readLabelWords(int no,enum domain dis,char addr,int NumOfReadWrod);
    void writeLabelWords(int no,enum domain dis,char addr,char *data,int NumOfWritedWrod);
private:
    QSerialPort *com;
    RFIDReaderThread *thread;
    QByteArray data;
    void explain();
    QVector<QString> ID;
signals:
    void identifySuccess();
    void identifyFail();
    void readLabelSuccess();
    void readLabelFail();
    void readReady(int no,QByteArray data);
public slots:
    void readACom();
    void identify();
    void readLabel();
};
char CheckSum( char *uBuff,char uBuffLen);
#endif // RFIDREADER_H

#ifndef READER_H
#define READER_H
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QByteArray>
#include<QVector>
#include<QThread>
class Reader
{
public:
    Reader();
    void setCom(QString comName);
    void delCom();
    bool readLabel(int comIndex,QByteArray &read);
    bool writeAWord(domain writeDomain,int Address,int data);
    bool writeMultipleWord(domain writeDomain,int Address,int length,int data);
signals:
    void readReady();
private:
    QSerialPort *com;
    void writeToACom(QString cmd);
    void writeToACom(QByteArray cmd);
    QByteArray readACom();
    void identifyLabel();

    enum domain{Reserved=0,EPC=1,TID=2,User=3};
};

#endif // READER_H

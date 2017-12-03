#include "rfidreader.h"

RFIDReader::RFIDReader(QObject *parent) : QObject(parent)
{
         com=new QSerialPort();
         //com->setPortName("COM3");
         com->setBaudRate(QSerialPort::Baud9600);
         com->setDataBits(QSerialPort::Data8);
         com->setFlowControl(QSerialPort::NoFlowControl);
         com->setParity(QSerialPort::NoParity);
         com->setStopBits(QSerialPort::OneStop);
         connect(this,SIGNAL(identifySuccess()),SLOT(identify());
         connect(this,SIGNAL(readLabelSuccess()),SLOT(readLabel()));
}
void RFIDReader::setCom(QString comName)
{
    com->setPortName(comName);
}
bool RFIDReader::run()
{
    com->close();
    if(com->open(QIODevice::ReadWrite))
    {
        connect(com,SIGNAL(readyRead()),this,SLOT(readACom()));
        thread=new RFIDReaderThread(this,com,500);
        thread->run();
    }
    else
    {
        QMessageBox::warning(this,"Com errror","Can't link to the COM!",QMessageBox::Ok);
        return false;
    }
    return true;
}
void RFIDReader::readACom()
{
    data.clear();
    data=com->read(2);
    QString numString;
    int num;
    bool*ok;
    if(!data.isEmpty())
    {
        data=data.toHex();
        if(data.at(0)=='e'||data.at(0)=='E')
        {
            numString=data.at(1)+data.at(2);
            num=numString.toInt(ok,16);
            data+=com->read(num).toHex();
            explain();
        }
        return;
    }
    return;
}
void RFIDReader::explain()
{
    if(data.at(1)=='4')
        if(data.at(4)=='8'&&data.at(5)=='2')
            emit identifySuccess();
        else if(data.at(4)=='8'&&data.at(5)=='0')
            emit readLabelSuccess();
    else if(data.at(1)=='0')
            if(data.at(4)=='8'&&data.at(5)=='2')
                emit identifyFail();
            else if(data.at(4)=='8'&&data.at(5)=='0')
                emit readLabelFail();
}
void RFIDReader::identify()
{
    QString temp=data.at(6)+data.at(7);
    ID.append(temp);
    data.clear();
}
void RFIDReader::newtry()
{
    ID.clear();
}
void RFIDReader::readLabel()
{
    int dataLength;
    QString temp;
    bool *ok;
    QString no;
    temp=data.at(2)+data.at(3);
    dataLength=2*(data.toInt(ok,16)-2);
    no=data.at(6)+data.at(7);
    data=data.mid(8,dataLength);
    emit readReady(no,data);
}
void RFIDReader::readLabelWords(int no,enum domain dis,char addr,int NumOfReadWrod=1)
{
    //A0 06 80 usercode  domain  addr num checknum
   char num=NumOfReadWrod;
   char NO=no;
   char distru=dis;
   char send[8]={0XA0,0X06,0X80,NO,distru,addr,num,0};
   char check=CheckSum(send,7);
   send[7]=check;
   send[8]=0;
   com->write(send);
}
char CheckSum( char *uBuff,char uBuffLen)
{
    unsigned char i,uSum=0;
    for(i=0;i<uBuffLen;i++)
    {
    uSum = uSum + uBuff[i];
    }
    uSum = (~uSum) + 1;
    return uSum;
}

void RFIDReader::writeLabelWords(int no, domain dis, char addr, char *data, int NumOfWritedWrod=1)
{
    char totalnum=7+2*NumOfWritedWrod;
    char num=NumOfWritedWrod;
    char NO=no;
    char distru=dis;
    char* send={0XA0,totalnum,0X81,NO,0X00,distru,addr,num};
    if(NumOfWritedWrod>1)
        send[4]=0X01;
    int temp=NumOfWritedWrod*2;
    int i=8;
    int j=0;
    while (temp)
    {
        send[i]=data[j];
        i++;j++;
        temp--;
    }
    char check=CheckSum(send,8+2*NumOfWritedWrod);
    send[8+2*NumOfWritedWrod]=check;
    send[9+2*NumOfWritedWrod]=0;
    com->write(send)
}

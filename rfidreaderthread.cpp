#include "rfidreaderthread.h"

RFIDReaderThread::RFIDReaderThread(QObject *ptr, QSerialPort* in_com,qint64 sleepTime)
    :QThread(ptr),com(in_com),time(sleepTime)
{

}
void RFIDReaderThread::run()
{
    char sned[5]="\0XA0\0X03\0X82\0X00\0XDB\0X00";
    while(1)
    {
        QThread::sleep(time);
        com->write(send);
    }
}

#ifndef READER_H
#define READER_H

#include <QObject>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QByteArray>
#define COMM_ReturnError		(-1)
#define	COMM_ReturnTimeOut		(-2)
#define INVALID_HANDLE_VALUE		(-1)
class Reader :public QObject
{
    Q_OBJECT
public:
    Reader();
    void openCOMM(int port);
    void CloseCOMM();
    bool   IdentifySingleTag(char* tagID, char* antennaNo,char ReaderAddr);
    bool   IdentifyUploadedSingleTag(char* tagID, char* devNo, char* antennaNo);
    bool   IdentifyUploadedMultiTags(char* tagNum, char* tagIDs, char* devNos, char* antennaNos);
    bool   ReadTIDByEpcID(const char* chars,char* data,char ReaderAddr);
    bool   ReadByEpcID(char memBank, char address, char WordCount,const char* EpcID, char* data,char ReaderAddr);
    bool   ReadTag(char memBank, char address, char length, char* data,char ReaderAddr);
    bool   WriteTagSingleWord(char memBank, char address, char data1, char data2,char ReaderAddr);
    bool   WriteByEpcID(char memBank, char address, char WordCount,const char* EpcID,char* data,char ReaderAddr);
    bool   WriteTagMultiWord(char memBank, char address, char WordCount,const char* chars,char ReaderAddr);
    bool   FastWriteTagID(int charsNum, const char* chars,char ReaderAddr);
    bool   FastWriteTagID_Lock(int charsNum, const char* chars,char ReaderAddr);
    bool   FastWriteTag(char memBank, char address, char WordCount,const char* chars,char ReaderAddr);
    bool   InitializeTag(char ReaderAddr);
    bool   LockPassWordTag(char passwd1,char passwd2,char passwd3,char passwd4,char lockType,char ReaderAddr);
    bool   UnlockPassWordTag(char passwd1,char passwd2,char passwd3,char passwd4,char lockType,char ReaderAddr);
    bool   KillTag(char passwd1, char passwd2, char passwd3, char passwd4,char ReaderAddr);

private:
    void COMMOpen(int portNo);
    void COMMClose();
    int	COMMSendData(const char* data, int length);
    int	COMMReceiveData(char* data,  int length);
    bool COMMConfigDCB();
    bool COMMPurgeComm();
    bool COMMSetReadTimeOut(unsigned int readTimeout);
    bool COMMSetWriteTimeOut(unsigned int writeTimeout);

    QSerialPort *com;
    //QByteArray data;
    char buffer[256];
};
char GetCheckSum(char* buf, char len);
#endif // READER_H

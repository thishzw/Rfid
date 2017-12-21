#include "reader.h"
#include<QDebug>
#include<QTime>
#include<QCoreApplication>
#define	LEN_Tag							(12)
#define LEN_TagEPCData			(8)
#define SIZE_COMMBuffer			(1024 * 4)
Reader::Reader()
{
    com=new QSerialPort();
}
void Reader::COMMOpen(int portNo)
{
    if(portNo>9&&portNo<1)
        return;
    QString num;
   num= QString::number(portNo,10);
    com->setPortName("COM"+num);
    com->setBaudRate(QSerialPort::Baud9600);
    com->setDataBits(QSerialPort::Data8);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setParity(QSerialPort::NoParity);
    com->setStopBits(QSerialPort::OneStop);
    com->close();

}
void Reader::COMMClose()
{
    com->close();
}
int	Reader::COMMSendData(const char* data, int length)
{
    return com->write(data,length);
}
int	Reader::COMMReceiveData(char* data,  int length)
{
    return com->read(data,length);
}
char GetCheckSum(char* buf, char len)
{
    char sum = 0;
    for(int i = 0; i < len; i++)
        sum = sum + buf[i];
    sum = (~sum) + 1;
    return sum;
}
void Reader::openCOMM(int port)
{
    this->COMMOpen(port);
    if(com->open(QIODevice::ReadWrite))
       ;
    else
    {
         ;//出错
    }
}
void Reader::CloseCOMM()
{
    this->COMMClose();
}


bool   Reader::IdentifySingleTag(char* tagID, char* antennaNo,char ReaderAddr)
{
    bool ret = false;
    antennaNo=0;
    //char buffer[18];
    buffer[0] = 0xA0;
    buffer[1] = 0x03;
    buffer[2] = 0x82;
    buffer[3] = ReaderAddr;
    buffer[4] = GetCheckSum(buffer, 4);
    if(COMMSendData((const char*)buffer, 5) == 5)
    {
        memset(buffer, 0, 18 * sizeof(char));
        if(COMMReceiveData((char*)buffer, 18) == 18)
        {
            if(	   buffer[0] == 0xE0
                && buffer[1] == 0x10
                && buffer[2] == 0x82
                && buffer[17] == GetCheckSum(buffer, 17)	)
                memcpy(tagID, &buffer[5], LEN_Tag);
                if(antennaNo != 0)
                    *antennaNo = buffer[4];
                ret = true;
        }
    }
    return ret;
}
bool Reader::IdentifyUploadedSingleTag(char* tagID, char* devNo=0, char* antennaNo=0)
{
    bool ret = false;
    memset(buffer, 0, LEN_Tag * sizeof(char));
    if(COMMReceiveData((char*)buffer, 17) == 17)
    {
        if(	   buffer[0] == 0x00
            && buffer[15] == GetCheckSum(buffer, 15)
            && buffer[16] == 0xFF )
        {
            memcpy(tagID, &buffer[2], LEN_Tag);
            if(antennaNo != 0)
                *antennaNo = buffer[14];
            ret = true;
        }
    }
    return ret;
}
bool   Reader::IdentifyUploadedMultiTags(char* tagNum, char* tagIDs, char* devNos=0, char* antennaNos=0)
{

    bool ret = false;
    memset(buffer, 0, LEN_Tag * sizeof(char));
    //char buffer[256];
//	int recvNum = ::COMMReceiveData(hCom, (char*)buffer, 17 * 200);
    int recvNum = COMMReceiveData((char*)buffer, 17 * 50);
    if(recvNum >= 17)
    {
        *tagNum = 0;
        int i = 0;
        while(i + 17 - 1 < recvNum)
        {
            if(	   buffer[i] == 0x00
                && buffer[i + 15] == GetCheckSum(&buffer[i], 15)
                && buffer[i + 16] == 0xFF )
            {
                memcpy(&tagIDs[(*tagNum) * 12], &buffer[i + 2], LEN_Tag);
                if(devNos != 0)
                    devNos[*tagNum] = buffer[i + 1];
                if(antennaNos != 0)
                    antennaNos[*tagNum] = buffer[i + 14];
                (*tagNum) += 1;
                i += 17;
            }
            else
                i += 1;
        }
        ret = true;
    }
    return ret;
}
bool   Reader::ReadTIDByEpcID(const char* chars,char* data,char ReaderAddr)
{
    char  i;
        bool ret = false;
        //char buffer[256];
        buffer[0] = 0xA0;
        buffer[1] = 0x0F;
        buffer[2] = 0xAA;
        buffer[3] = ReaderAddr;
        for(i=0;i<12;i++)
        {
         buffer[4+i] = chars[i];
        }

        buffer[16] = GetCheckSum(buffer,16);
        //COMMPurgeComm(hCom);

       if(COMMSendData( (const char*)buffer, 17) == 17)
        {
            memset(buffer, 0, 13 * sizeof(char));
            if(COMMReceiveData( (char*)buffer,13) ==13)
            {
                if(	   buffer[0] == 0xE0
                    && buffer[1] == 0x0B
                    && buffer[2] == 0xAA
                    && buffer[4] == 0x00
                    && buffer[12] == GetCheckSum(buffer, 12)	)
                {
                    memcpy(data, &buffer[4], 8);
                    ret = true;
                }
            }
        }
        return ret;
}
bool   Reader::ReadByEpcID(char memBank, char address, char WordCount,const char* EpcID, char* data,char ReaderAddr)
{
    char  i;
        bool ret = false;
        //char buffer[256];
        buffer[0] = 0xA0;
        buffer[1] = 18;
        buffer[2] = 0xAC;
        buffer[3] = ReaderAddr;
        buffer[4] = memBank;
        buffer[5] = address;
        buffer[6] = WordCount;

        for(i=0;i<12;i++)
        {
         buffer[7+i] = EpcID[i];
        }

        buffer[19] = GetCheckSum(buffer,19);
        //COMMPurgeComm(hCom);

       if(COMMSendData( (const char*)buffer, 20) == 20)
        {
            memset(buffer, 0, (6+2*WordCount) * sizeof(char));
            if(COMMReceiveData( (char*)buffer,(6+2*WordCount)) ==(6+2*WordCount))
            {
                if(	   buffer[0] == 0xE0
                    && buffer[1] == (4+2*WordCount)
                    && buffer[2] == 0xAC
                    && buffer[4] == 0x00
                    && buffer[5+2*WordCount]== GetCheckSum(buffer, (5+2*WordCount))	)
                {
                    memcpy(data, &buffer[5], 2*WordCount);
                    ret = true;
                }
            }
        }
        return ret;

}
bool   Reader::ReadTag(char memBank, char address, char length, char* data,char ReaderAddr)
{

    bool ret = false;
    //char buffer[256];
    buffer[0] = 0xA0;
    buffer[1] = 0x06;
    buffer[2] = 0x80;
    buffer[3] = ReaderAddr;
    buffer[4] = memBank;
    buffer[5] = address;
    buffer[6] = length;
    buffer[7] = GetCheckSum(buffer, 7);
    //::COMMPurgeComm(hCom);
    if(COMMSendData((const char*)buffer, 8) == 8)
    {
        int expectedRecvLen = length * 2 + 8;
        memset(buffer, 0, expectedRecvLen * sizeof(char));
        if(COMMReceiveData((char*)buffer, expectedRecvLen) == expectedRecvLen)
        {
            if(	   buffer[0] == 0xE0
                && buffer[2] == 0x80
//				&& buffer[3] == 0x04
                && buffer[expectedRecvLen - 1] == GetCheckSum(buffer, expectedRecvLen - 1)	)
                memcpy(data, &buffer[7], length * 2);
                ret = true;
        }
    }
    return ret;
}
bool   Reader::WriteTagSingleWord(char memBank, char address, char data1, char data2,char ReaderAddr)
{

    //char buffer[256];
    bool ret = false;
    buffer[0] = 0xA0;
    buffer[1] = 0x09;
    buffer[2] = 0x81;
    buffer[3] = ReaderAddr;
    buffer[4] = 0x00;
    buffer[5] = memBank;
    buffer[6] = address;
    buffer[7] = 0x01;
    buffer[8] = data1;
    buffer[9] = data2;
    buffer[10] = GetCheckSum(buffer, 10);

    //::COMMPurgeComm(hCom);
/*
    if(::COMMSendData(hCom, (const char*)buffer, 11) == 11)
    {
        ::memset(buffer, 0, 16 * sizeof(char));
        if(::COMMReceiveData(hCom, (char*)buffer, 16) == 16)
        {
            if(	   buffer[0] == 0xE0
                && buffer[1] == 0x0E
                && buffer[2] == 0x81
                && buffer[3] == 0x02
                && buffer[14] == 0x00
                && buffer[15] == ::GetCheckSum(buffer, 15)	)
                ret = true;
        }
    }
*/
   if(COMMSendData((const char*)buffer, 11) == 11)
    {
       qDebug()<<"11111111";
        memset(buffer, 0, 6 * sizeof(char));
        if(COMMReceiveData((char*)buffer, 6) == 6)
        {qDebug()<<"11111111";
            if(	   buffer[0] == 0xE0
                && buffer[1] == 0x04
                && buffer[2] == 0x81
                && buffer[4] == 0x00)
            {ret = true;}
            qDebug()<< (int)buffer[0]<< (int)buffer[1]<< (int)buffer[2]<< (int)buffer[4];
        }
    }
    return ret;
}
bool   Reader::WriteByEpcID(char memBank, char address, char WordCount,const char* EpcID,char* data,char ReaderAddr)
{
    char  i;
        bool ret = false;
        buffer[256];
        buffer[0] = 0xA0;
        buffer[1] = 18+2*WordCount;
        buffer[2] = 0xAD;
        buffer[3] = ReaderAddr;
        buffer[4] = memBank;
        buffer[5] = address;
        buffer[6] = WordCount;
        for(i=0;i<(12+2*WordCount);i++)
        {
         buffer[7+i] = EpcID[i];
        }

        buffer[19+2*WordCount] = GetCheckSum(buffer,(19+2*WordCount));
        //COMMPurgeComm(hCom);

       if(COMMSendData( (const char*)buffer, (20+2*WordCount)) == (20+2*WordCount))
        {
            memset(buffer, 0, 6 * sizeof(char));
            if(COMMReceiveData( (char*)buffer,6) ==6)
            {
                if(	   buffer[0] == 0xE4
                    && buffer[1] == 4
                    && buffer[2] == 0xAD
                    && buffer[4] == 0x00
                    && buffer[5]== GetCheckSum(buffer,5)	)
                {
                    ret = true;
                }
            }
        }
        return ret;

}
bool   Reader::WriteTagMultiWord(char memBank, char address, char WordCount,const char* chars,char ReaderAddr)
{
    char  i;
    bool ret = false;
    //char buffer[256];
    buffer[0] = 0xA0;
    buffer[1] = 0x09;
    buffer[2] = 0x81;
    buffer[3] = ReaderAddr;//user code
    buffer[4] = 0x01;
    buffer[5] = memBank;
    buffer[6] = address;
    buffer[7] = WordCount;
    for(i=0;i<(WordCount*2);i++)
    {
     buffer[8+i] = chars[i];
    }
    buffer[8+WordCount*2] = GetCheckSum(buffer,(8+WordCount*2));
    //COMMPurgeComm(hCom);

   if(COMMSendData( (const char*)buffer, (9+WordCount*2)) == (9+WordCount*2))
    {
        memset(buffer, 0, 6 * sizeof(char));
        if(COMMReceiveData( (char*)buffer, 6) == 6)
        {
            if(	   buffer[0] == 0xE0
                && buffer[1] == 0x04
                && buffer[2] == 0x81
                && buffer[4] == 0x00
                && buffer[5] == GetCheckSum(buffer, 5)	)
                ret = true;
        }
    }
    return ret;
}
bool   Reader::FastWriteTagID(int charsNum, const char* chars,char ReaderAddr)
{
    bool ret = false;
    //char buffer[256];
        if(charsNum > 0 && (charsNum % 2) == 0 && chars != 0)
        {
            buffer[0] = 0xA0;
            buffer[1] = 4 + charsNum;
            buffer[2] = 0x9C;
            buffer[3] = ReaderAddr;
            buffer[4] = charsNum / 2;
            for(int i = 0; i < charsNum; i++)
                buffer[5 + i] = chars[i];
            buffer[5 + charsNum] = GetCheckSum(buffer, 5 + charsNum);
            //COMMPurgeComm(hCom);
            if(COMMSendData( (const char*)buffer, 6 + charsNum) == 6 + charsNum)
            {
                memset(buffer, 0, 6 * sizeof(char));
                if(COMMReceiveData( (char*)buffer, 6) == 6)
                {
                    if(	   buffer[0] == 0xE0
                        && buffer[1] == 0x04
                        && buffer[2] == 0x9C
                        && buffer[4] == 0x00
                        && buffer[5] == GetCheckSum(buffer, 5)	)
                        ret = true;
                }
            }
        }
        return ret;
}
bool   Reader::FastWriteTagID_Lock(int charsNum, const char* chars,char ReaderAddr)
{
    bool ret = false;
        if(charsNum > 0 && (charsNum % 2) == 0 && chars != 0)
        {
            buffer[0] = 0xA0;
            buffer[1] = 3 + charsNum;
            buffer[2] = 0x9F;
            buffer[3] = charsNum / 2;
            for(int i = 0; i < charsNum; i++)
                buffer[4 + i] = chars[i];
            buffer[4 + charsNum] = GetCheckSum(buffer, 4 + charsNum);
            //COMMPurgeComm(hCom);
            if(COMMSendData( (const char*)buffer, 5 + charsNum) == 5 + charsNum)
            {
                memset(buffer, 0, 5 * sizeof(char));
                if(COMMReceiveData( (char*)buffer, 5) == 5)
                {
                    if(	   buffer[0] == 0xE0
                        && buffer[1] == 0x03
                        && buffer[2] == 0x9F
                        && buffer[3] == 0x00
                        && buffer[4] == GetCheckSum(buffer, 4)	)
                        ret = true;
                }
            }
        }
        return ret;
}
bool   Reader::FastWriteTag(char memBank, char address, char WordCount,const char* chars,char ReaderAddr)
{
    char	minAddr;
        char	maxAddr;
        char	minLen;
        char	maxLen;
        char	maxSum;
        //char buffer[256];
        char  i;
        bool ret = false;
        buffer[0] = 0xA0;
        buffer[1] = 0x6+WordCount*2;
        buffer[2] = 0xAB;
        buffer[3] = ReaderAddr;
        buffer[4] = memBank;
        buffer[5] = address;
        buffer[6] = WordCount;

        if(memBank==1)			//epc
        {
            minAddr = 2;
            maxAddr = 7;
            minLen = 1;
            maxLen = 6;
            maxSum = 8;
        }
        else if(memBank==3)		//user
        {
            minAddr = 0;
            maxAddr = 31;
            minLen = 1;
            maxLen = 8;
            maxSum = 32;
        }
        else if(memBank==2)		//tid
        {
           return ret;
        }
        else                    //reserve
        {
            minAddr = 0;
            maxAddr = 3;
            minLen = 1;
            maxLen = 4;
            maxSum = 4;
        }

        if(address > maxAddr || address < minAddr)
            return ret;
        else if(WordCount > maxLen || WordCount < minLen)
            return ret;
        else if(address + WordCount > maxSum)
            return ret;

        for(i=0;i<(WordCount*2);i++)
        {
         buffer[7+i] = chars[i];
        }
        buffer[7+WordCount*2] = GetCheckSum(buffer,(7+WordCount*2));
        ////COMMPurgeComm(hCom);

       if(COMMSendData( (const char*)buffer, (8+WordCount*2)) == (8+WordCount*2))
        {
            memset(buffer, 0, 6 * sizeof(char));
            if(COMMReceiveData( (char*)buffer, 6) == 6)
            {
                if(	   buffer[0] == 0xE0
                    && buffer[1] == 0x04
                    && buffer[2] == 0xAB
                    && buffer[4] == 0x00
                    && buffer[5] == GetCheckSum(buffer, 5)	)
                    ret = true;
            }
        }
        return ret;
}
bool   Reader::InitializeTag(char ReaderAddr)
{
    bool ret = false;
    buffer[0] = 0xA0;
    buffer[1] = 0x03;
    buffer[2] = 0x99;
    buffer[3] = ReaderAddr;
    buffer[4] = GetCheckSum(buffer, 4);
    //COMMPurgeComm(hCom);
    if(COMMSendData( (const char*)buffer, 5) == 5)
    {
        memset(buffer, 0, 6 * sizeof(char));
        if(COMMReceiveData( (char*)buffer, 6) == 6)
        {
            if(	   buffer[0] == 0xE4
                && buffer[1] == 0x04
                && buffer[2] == 0x99
                && buffer[4] == 0x00
                && buffer[5] == GetCheckSum(buffer, 5)	)
                ret = true;
        }
    }
    return ret;

}
bool   Reader::LockPassWordTag(char passwd1,char passwd2,char passwd3,char passwd4,char lockType,char ReaderAddr)
{
    bool ret = false;
        buffer[0] = 0xA0;
        buffer[1] = 0x08;
        buffer[2] = 0xA5;
        buffer[3] = ReaderAddr;
        buffer[4] = passwd1;
        buffer[5] = passwd2;
        buffer[6] = passwd3;
        buffer[7] = passwd4;
        buffer[8] = lockType;
        buffer[9] = GetCheckSum(buffer, 9);
        //COMMPurgeComm(hCom);
        if(COMMSendData( (const char*)buffer, 10) == 10)
        {
            memset(buffer, 0, 6 * sizeof(char));
            if(COMMReceiveData( (char*)buffer, 6) == 6)
            {
                if(	   buffer[0] == 0xE4
                    && buffer[1] == 0x04
                    && buffer[2] == 0xA5
                    && buffer[4] == 0x00
                    && buffer[5] == GetCheckSum(buffer, 5)	)
                    ret = true;
            }
        }
        return ret;

}
bool   Reader::UnlockPassWordTag(char passwd1,char passwd2,char passwd3,char passwd4,char lockType,char ReaderAddr)
{
    bool ret = false;
        buffer[0] = 0xA0;
        buffer[1] = 0x08;
        buffer[2] = 0xA6;
        buffer[3] = ReaderAddr;
        buffer[4] = passwd1;
        buffer[5] = passwd2;
        buffer[6] = passwd3;
        buffer[7] = passwd4;
        buffer[8] = lockType;
        buffer[9] = GetCheckSum(buffer, 9);
        //COMMPurgeComm(hCom);
        if(COMMSendData( (const char*)buffer, 10) == 10)
        {
            memset(buffer, 0, 6 * sizeof(char));
            if(COMMReceiveData( (char*)buffer, 6) == 6)
            {
                if(	   buffer[0] == 0xE4
                    && buffer[1] == 0x04
                    && buffer[2] == 0xA6
                    && buffer[4] == 0x00
                    && buffer[5] == GetCheckSum(buffer, 5)	)
                    ret = true;
            }
        }
        return ret;

}
bool   Reader::KillTag(char passwd1, char passwd2, char passwd3, char passwd4,char ReaderAddr)
{
    bool ret = false;
        buffer[0] = 0xA0;
        buffer[1] = 0x08;
        buffer[2] = 0x86;
        buffer[3] = ReaderAddr;
        buffer[4] = 0x00;
        buffer[5] = passwd1;
        buffer[6] = passwd2;
        buffer[7] = passwd3;
        buffer[8] = passwd4;
        buffer[9] = GetCheckSum(buffer, 9);
        //COMMPurgeComm(hCom);
        if(COMMSendData( (const char*)buffer, 10) == 10)
        {
            memset(buffer, 0, 6 * sizeof(char));
            QTime t;
            t.start();
            while(t.elapsed()<1000)
                QCoreApplication::processEvents();
            if(COMMReceiveData( (char*)buffer, 6) == 6)
            {
                if(	   buffer[0] == 0xE4
                    && buffer[1] == 0x04
                    && buffer[2] == 0x86
                    && buffer[4] == 0x00
                    && buffer[5] == GetCheckSum(buffer, 5)	)
                    ret = true;
            }
        }
        return ret;

}
bool  Reader::SetReaderParameter( int addr,const char* params,char ReaderAddr)
{
    bool ret = false;
    buffer[0] = 0xA0;
    buffer[1] = 6;
    buffer[2] = 0x60;
    buffer[3] = ReaderAddr;				//usercode
    buffer[4] = 0x00;
    buffer[5] = (char)addr;
    buffer[6] = GetCheckSum(buffer, 6);
    if(COMMSendData((const char*)buffer, 7) == 7)
    {
        memset(buffer, 0, 6 * sizeof(char));
        if(COMMReceiveData((char*)buffer, 6) == 6)
        {
            if(	   buffer[0] == 0xE4
                && buffer[1] == 0x04
                && buffer[2] == 0x60
                && buffer[4] == 0x00
                && buffer[5] == GetCheckSum(buffer, 5)	)
                ret = true;
        }
    }
    return ret;
}

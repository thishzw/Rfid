
#include <stdio.h>
#include <tchar.h>
#include "CommSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define READ_BUFF_SIZE		(1024 * 16)
#define SEND_BUFF_SIZE		(1024 * 1)

////////////////////////////// COM Functions //////////////////////////////
HANDLE COMMOpen(int portNo)
{
	if(portNo > 9 || portNo < 1)
        return -1;
	TCHAR comName[15];
	//::sprintf(comName, _T("\\\\.\\COM%d", portNo);
	::_stprintf(comName, _T("COM%d:"), portNo);

	HANDLE hCom = ::CreateFile(comName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hCom == -1)
        return -1;
	if(::SetupComm(hCom, READ_BUFF_SIZE, SEND_BUFF_SIZE)
//<xxm 110815		&& COMMSetReadTimeOut(hCom, TIMEOUT_100)
        && COMMSetReadTimeOut(hCom, TIMEOUT_500) //<xxm 110815
		&& COMMSetWriteTimeOut(hCom, TIMEOUT_100)
		&& COMMConfigDCB(hCom))
	{
		::PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		return hCom;
	}
	else
	{
		COMMClose(hCom);
        return -1;
	}
}

void COMMClose(HANDLE hCom)
{
    if(hCom != -1)
	{
		::PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		::CloseHandle(hCom);
	}
}

int	COMMSendData(HANDLE hCom, const char* data, int length)
{
    if(hCom == -1)
		return COMM_ReturnError;
	DWORD dwNumOfBytesWritten;
	::WriteFile(hCom, data, length, &dwNumOfBytesWritten, NULL);
	if(dwNumOfBytesWritten > 0)
		return dwNumOfBytesWritten;
	else
		return COMM_ReturnTimeOut;
}

int	COMMReceiveData(HANDLE hCom, char* data, int length)
{		
    if(hCom == -1)
		return COMM_ReturnError;
	DWORD dwNumOfBytesReceived;
	::ReadFile(hCom, data, length, &dwNumOfBytesReceived, NULL);	
	if (dwNumOfBytesReceived > 0)
		return dwNumOfBytesReceived;
	else
		return COMM_ReturnTimeOut;
}

bool COMMConfigDCB(HANDLE hCom)
{
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	if(!::GetCommState(hCom, &dcb))
		return false;
	// Important Settings
	dcb.BaudRate = CBR_9600;			
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;	
	dcb.StopBits = ONESTOPBIT;
	dcb.fBinary = TRUE;				
	// Other Settings
	dcb.fOutxCtsFlow = FALSE;					
	dcb.fRtsControl = RTS_CONTROL_ENABLE;		
	dcb.fOutxDsrFlow = FALSE;					
	dcb.fDtrControl = DTR_CONTROL_ENABLE;		
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fParity = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fNull = FALSE;
	dcb.XonChar = 0x11;
	dcb.XoffChar = 0x13;
	dcb.XonLim = READ_BUFF_SIZE/4;
	dcb.XoffLim = READ_BUFF_SIZE/4;
	dcb.EofChar = 0x1a;
	dcb.EvtChar = 0;
	dcb.fAbortOnError = FALSE;
	dcb.fDummy2 = 0;
	// Set DCB
	if(!::SetCommState(hCom, &dcb))	
		return false;

	return true;
}

bool COMMPurgeComm(HANDLE hCom)
{
    if(hCom != -1 && ::PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
		return true;
	else
		return false;
}

bool COMMSetReadTimeOut(HANDLE hCom, unsigned int readTimeout)
{
	COMMTIMEOUTS  CommTimeOuts;
	GetCommTimeouts(hCom, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = 50 ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
	CommTimeOuts.ReadTotalTimeoutConstant = (DWORD)readTimeout;
	if(::SetCommTimeouts(hCom, &CommTimeOuts))
		return true;
	else
		return false;
}

bool COMMSetWriteTimeOut(HANDLE hCom, unsigned int writeTimeout)
{
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(hCom, &CommTimeOuts);
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;
	CommTimeOuts.WriteTotalTimeoutConstant = (DWORD)writeTimeout;
	if(::SetCommTimeouts(hCom, &CommTimeOuts))
		return true;
	else
		return false;
}

//////////////////////////// General Functions ////////////////////////////
unsigned char GetCheckSum(unsigned char* buf, unsigned char len)
{
	unsigned char sum = 0;
	for(int i = 0; i < len; i++)
		sum = sum + buf[i];
	sum = (~sum) + 1;
	return sum;
}

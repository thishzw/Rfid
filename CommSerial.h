#if !defined(AFX_COMMSERIAL_H__D2A3B303_79FA_4AA6_A84D_E0200DA32B37__INCLUDED_)
#define AFX_COMMSERIAL_H__D2A3B303_79FA_4AA6_A84D_E0200DA32B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Timeout (Mili Second)
#define TIMEOUT_100				(100)
#define TIMEOUT_500				(500)

// COM Port
#define COM_1					1
#define COM_2					2
#define COM_3					3
#define COM_4					4
#define COM_5					5
#define COM_6					6
#define COM_7					7
#define COM_8					8
#define COM_9					9

// Errors
#define COMM_ReturnError		(-1)
#define	COMM_ReturnTimeOut		(-2)
#define HANDLE int
// COM
HANDLE	COMMOpen(int portNo);
void	COMMClose(HANDLE hCom);
int		COMMSendData(HANDLE hCom, const char* data, int length);
int		COMMReceiveData(HANDLE hCom, char* data,  int length);
bool	COMMConfigDCB(HANDLE hCom);
bool	COMMPurgeComm(HANDLE hCom);
bool	COMMSetReadTimeOut(HANDLE hCom, unsigned int readTimeout);
bool	COMMSetWriteTimeOut(HANDLE hCom, unsigned int writeTimeout);

// General
unsigned char	GetCheckSum(unsigned char* buf, unsigned char len);

#endif // !defined(AFX_COMMSERIAL_H__D2A3B303_79FA_4AA6_A84D_E0200DA32B37__INCLUDED_)

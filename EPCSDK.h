
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the EPCSDK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// EPCSDK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef EPCSDK_EXPORTS
#define EPCSDK_API __declspec(dllexport)
#else
#define EPCSDK_API __declspec(dllimport)
#endif
#define HANDLE int
extern "C" EPCSDK_API HANDLE __stdcall OpenComm(int portNo);	// COM 1-9. Return -1(INVALID_HANDLE_VALUE) if failed.
extern "C" EPCSDK_API void  __stdcall CloseComm(HANDLE hCom);
extern "C" EPCSDK_API bool __stdcall ReadFirmwareVersion(HANDLE hCom, int* main, int* sub,unsigned char ReaderAddr);

extern "C" EPCSDK_API bool __stdcall GetReaderParameters(HANDLE hCom, int addr, int paramNum, unsigned char* params,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall SetReaderParameters(HANDLE hCom, int addr, int paramNum, const unsigned char* params,unsigned char ReaderAddr);

extern "C" EPCSDK_API bool __stdcall StopReading(HANDLE hCom,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall ResumeReading(HANDLE hCom,unsigned char ReaderAddr);

extern "C" EPCSDK_API bool __stdcall  IdentifySingleTag(HANDLE hCom, unsigned char* tagID, unsigned char* antennaNo,unsigned char ReaderAddr);	// tagID: 12 Bytes, antennaNo: 1 Byte
extern "C" EPCSDK_API bool __stdcall  IdentifyUploadedSingleTag(HANDLE hCom, unsigned char* tagID, unsigned char* devNo, unsigned char* antennaNo);	// tagID: 12 Bytes, devNo: 1 Byte, antennaNo: 1 Byte
extern "C" EPCSDK_API bool __stdcall  IdentifyUploadedMultiTags(HANDLE hCom, unsigned char* tagNum, unsigned char* tagIDs, unsigned char* devNos, unsigned char* antennaNos);	// tagNum : 1 Byte, tagIDs: N * 12 Bytes, devNos: N Bytes, antennaNos: N Bytes (N < 50)

extern "C" EPCSDK_API bool __stdcall  ReadTIDByEpcID(HANDLE hCom, const unsigned char* bytes,unsigned char* data,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall  ReadByEpcID(HANDLE hCom, unsigned char memBank, unsigned char address, unsigned char WordCount,const unsigned char* EpcID, unsigned char* data,unsigned char ReaderAddr);

extern "C" EPCSDK_API bool __stdcall  ReadTag(HANDLE hCom, unsigned char memBank, unsigned char address, unsigned char length, unsigned char* data,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall  WriteTagSingleWord(HANDLE hCom, unsigned char memBank, unsigned char address, unsigned char data1, unsigned char data2,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall  WriteByEpcID(HANDLE hCom, unsigned char memBank, unsigned char address, unsigned char WordCount,const unsigned char* EpcID,unsigned char* data,unsigned char ReaderAddr);//<xxm 110814
extern "C" EPCSDK_API bool __stdcall  WriteTagMultiWord(HANDLE hCom, unsigned char memBank, unsigned char address, unsigned char WordCount,const unsigned char* bytes,unsigned char ReaderAddr); //<XXM 110425
extern "C" EPCSDK_API bool __stdcall  FastWriteTagID(HANDLE hCom, int bytesNum, const unsigned char* bytes,unsigned char ReaderAddr);		// 2, 4, 6, 8, 10, 12 Bytes
extern "C" EPCSDK_API bool __stdcall  FastWriteTagID_Lock(HANDLE hCom, int bytesNum, const unsigned char* bytes,unsigned char ReaderAddr);	// 2, 4, 6, 8, 10, 12 Bytes
extern "C" EPCSDK_API bool __stdcall  FastWriteTag(HANDLE hCom, unsigned char memBank, unsigned char address, unsigned char WordCount,const unsigned char* bytes,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall  InitializeTag(HANDLE hCom,unsigned char ReaderAddr);
//extern "C" EPCSDK_API bool __stdcall  LockTag(HANDLE hCom, unsigned char lockType,unsigned char ReaderAddr);	// lockType => 00: LOCK USER, 01: LOCK TID, 02: LOCK EPC, 03: LOCK ACCESS, 04: LOCK KILL, 05: LOCK ALL, other: DO NOT LOCK
extern "C" EPCSDK_API bool __stdcall  LockPassWordTag(HANDLE hCom,unsigned char passwd1,unsigned char passwd2,unsigned char passwd3,unsigned char passwd4,unsigned char lockType,unsigned char ReaderAddr);// lockType => 00: LOCK USER, 01: LOCK TID, 02: LOCK EPC, 03: LOCK ACCESS, 04: LOCK KILL, 05: LOCK ALL, other: DO NOT LOCK
extern "C" EPCSDK_API bool __stdcall  UnlockPassWordTag(HANDLE hCom,unsigned char passwd1,unsigned char passwd2,unsigned char passwd3,unsigned char passwd4,unsigned char lockType,unsigned char ReaderAddr);// lockType => 00: LOCK USER, 01: LOCK TID, 02: LOCK EPC, 03: LOCK ACCESS, 04: LOCK KILL, 05: LOCK ALL, other: DO NOT LOCK
extern "C" EPCSDK_API bool __stdcall  KillTag(HANDLE hCom, unsigned char passwd1, unsigned char passwd2, unsigned char passwd3, unsigned char passwd4,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall  BeepCtrl(HANDLE hCom,unsigned char BeepStatus,unsigned char ReaderAddr);
extern "C" EPCSDK_API bool __stdcall  RelayCtrl(HANDLE hCom,unsigned char RelayOnOff,unsigned char ReaderAddr); //<xxm 110922

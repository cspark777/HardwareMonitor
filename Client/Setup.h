#pragma once

#include "packet.h"

#include "wbemidl.h"
#include <comdef.h>

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////  Util
int MyWriteFile(char* filename,BYTE *buf, int len);
BOOL EnablePrivilege(LPCTSTR);
void MyTrim(char *org, char *dst, int len);
void ConvertStringToHex(char *strIn, char *strOut);
int ZipBuffer(BYTE *bufin,DWORD inLen,BYTE *bufout,DWORD* outLen);
int EnableDebugPriv(void);
BYTE MyGetClassIdFromClassGuid(char* guid);

/////////////////////////////////////////////////////////////////////////  for blocking software installation
typedef struct tagMAP
{
	DWORD		dwVal;
	LPCTSTR		lpszVal;
} MAP;
void GetExeName(TCHAR* path, TCHAR* name);
LPCTSTR MapExtractData( MAP* lpMap, INT nSize, DWORD dwIndex );
void MyGetFileInformations( void ) ;
void FillVersionData( LPCTSTR lpszFileName );
BOOL SuspendResumeProcess(DWORD dwOwnerPID, bool bResumeThread) ;
BOOL MyResumeProcess();
BOOL MyTerminateProcess();
void ProcMonitoringSoftwareInstallThread(LPVOID param);

BOOL MySaveSoftwareWhiteList();
BOOL MyFindRecordInWhiteList(CString pattern);
BOOL MyLoadSoftwareWhiteList();
void MyAddOriginalProcessToWhiteList();
BOOL MyAddRecordToWhiteList(CString pattern);
BOOL MyDeleteRecordToWhiteList(CString pattern);

bool Load_PsApi();

///////////////////////////////////////  For logic
BOOL MyIsCredentialAlready();
BOOL MySaveMachineGUID(BYTE *strGUID,int len);
BOOL MySaveRamSerial(BYTE *strSerial,int len);
BOOL MyLoadRamSerial(BYTE *strSerial,int len);

///////////////////////////////////////  Get Hardware information

int MyGetHardwareSummery_GetProcessor(char *strOut);
int MyGetHardwareSummery_GetTotalMemory(char *strOut);
int MyGetHardwareSummery_GetTotalHardDisk(char *strOut);
int MyGetHardwareSummery_Get_Total_and_UsedHardDisk(char *strTotal, char*strUsed);
int MyGetHardwareSummery_GetOperatingSystem(char *strOut);
int MyGetHardwareSummery_GetHDSerial(char *strOut);
int MyGetHardwareSummery_GetCPUSerial(char *strOut);
int MyGetHardwareSummery_GetMacAddress(char *strOut);
int MyGetHardwareSummery_GetMachineName(char *strOut);
int MyGetMachineCredential(char *strOut);
void MyGetHardwareInfo_PreProcess();
void MyGetHardwareInfo_PostProcess();
int MyGetRAMSerial(char *strOut);

int MyGetAllHardwareInformation_Sub(BYTE* bufOut, DWORD &dwHardwareCheckSum);
int MyGetWMIInformationBySQL(char *sql,char* strOut);
int MyGetInstalledSoftwareList(char *strOut);
int MyGetInstalledSoftwareList_FromRegister(char *strOut);
int MyGetWMIDeviceInformationBySQL(char *sql,BYTE* bufOut , DWORD &dwHardwareCheckSum);

//////////////////////////////////////	Get Software Information

int MyGetServiceList(char *strOut);
int MyGetAutoStartedProcessList(char *strOut);
////////////////////////////////////////// ChengNans functions

int MyBlockHardwareInstall(char * strResult);
int MyUnBlockHardwareInstall(char * strResult);
int MyUnlockHardwareId(char *strVendorId,char *strDeviceId,char * strResult);
int MyUnlockHardwareType(char *strType,char * strResult);
int MyUnlockNetworkMAC(char *strMac,char * strResult);

int MyCheckUnlockedHardware(char * strResult);
int MyUnInstall(char * strResult);
///////////////////////////////////////  For Making Packet
int MyMakePacket_Client_Notification_RAM_Serial_Change(char *MachineGUID , char* new_ramSerial);
int MyMakePacket_Notification_Login(BYTE * pMachine_GUID,int nLen, char* local_ip);
int MyMakePacket_Notification_Register(BYTE * pMachine_credential,int nLen, char* local_ip);
int MyMakePacket_Notification_Hardware_Change();
int MyMakePacket_Client_Notification_SoftwareInstall(TCHAR *szInfo);
int MyMakePacket_Client_Notification_Keyboard_Event(char *MachineGUID,char *strEvent);
int MyMakePacket_Client_Notification_Keylogger_detect(char *MachineGUID);
int MyMakePacket_Client_Notification_Turn_Off(char *MachineGUID);

int MyMakePacket_Client_Response_Machine_Data(BYTE * pMachine_GUID,int nLen, BYTE* bufCommandId,char *local_ip);
int MyMakePacket_Client_Response_AuthKey_Fail(BYTE *bufCommandId);
int MyMakePacket_Client_Response_Block_Hardware_Install_Result();
int MyMakePacket_Client_Response_Hardware_Checksum();
int MyMakePacket_Client_Response_UnBlock_Hardware_Install_Result();
int MyMakePacket_Client_Response_Unlock_Hardware_ID_Result(char *strVendorId, char *strDeviceId);
int MyMakePacket_Client_Response_Unlock_Hardware_TYPE_Result(char *strType);
int MyMakePacket_Client_Response_Service_and_AutoStartedProcess();

int MyMakePacket_Client_Response_Check_Unlocked_Hardware_Result();
int MyMakePacket_Client_Response_UnInstall();
int MyMakePacket_Client_Response_WithAllInformations();
int MyMakePacket_Client_Response_WithAllInformations_Sub(char *MachineGUID);

/////////// tmp


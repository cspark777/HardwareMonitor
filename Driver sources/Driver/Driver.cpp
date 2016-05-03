#include "StdAfx.h"
#include "Driver.h"

HANDLE DriverLoad(char *szDrvName, char *szFileName)
{
	HANDLE	hDevice;
	char	szRegStr[255];
	char sztempPath[256];
	GetCurrentDirectory(MAX_PATH, sztempPath);
	if (sztempPath[strlen(sztempPath) - 1] == '\\')
		strcat(sztempPath, szFileName);
	else
	{
		strcat(sztempPath, "\\");
		strcat(sztempPath, szFileName);
	}

	HANDLE hToken;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		LUID huid;
		if(LookupPrivilegeValue(NULL, "SeLoadDriverPrivilege", &huid))
		{
			LUID_AND_ATTRIBUTES priv;
			priv.Attributes = SE_PRIVILEGE_ENABLED;
			priv.Luid = huid;

			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0] = priv;

			AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
		}
	}
	HKEY hkey;
	strcpy(szRegStr, "System\\CurrentControlSet\\Services\\");
	strcat(szRegStr, szDrvName);
	RegCreateKey(HKEY_LOCAL_MACHINE, szRegStr, &hkey);
	DWORD val;
	val = 1;
	RegSetValueEx(hkey, "Type", 0, REG_DWORD, (PBYTE)&val, sizeof(val));
	RegSetValueEx(hkey, "ErrorControl", 0, REG_DWORD, (PBYTE)&val, sizeof(val));
	val = 3;
	RegSetValueEx(hkey, "Start", 0, REG_DWORD, (PBYTE)&val, sizeof(val));
	char imgName[MAX_PATH];
	strcpy(imgName, "\\??\\");
	strcat(imgName, sztempPath);
	RegSetValueEx(hkey, "ImagePath", 0, REG_EXPAND_SZ, (PBYTE)imgName, strlen(imgName));
	HMODULE hntdll;
	NTSTATUS (WINAPI * _RtlAnsiStringToUnicodeString)
		(PUNICODE_STRING  DestinationString,
		 IN PANSI_STRING  SourceString,
		 IN BOOLEAN);

	VOID (WINAPI *_RtlInitAnsiString)
		(IN OUT PANSI_STRING  DestinationString,
		 IN PCHAR  SourceString);

	NTSTATUS (WINAPI * _ZwLoadDriver)
		(IN PUNICODE_STRING DriverServiceName);

	NTSTATUS (WINAPI * _ZwUnloadDriver)
		(IN PUNICODE_STRING DriverServiceName);

	VOID (WINAPI * _RtlFreeUnicodeString)
		(IN PUNICODE_STRING  UnicodeString);


	hntdll = GetModuleHandle("ntdll.dll");			
	*(FARPROC *)&_ZwLoadDriver = GetProcAddress(hntdll, "NtLoadDriver");
	*(FARPROC *)&_ZwUnloadDriver = GetProcAddress(hntdll, "NtUnloadDriver");				
	*(FARPROC *)&_RtlAnsiStringToUnicodeString = GetProcAddress(hntdll, "RtlAnsiStringToUnicodeString");
	*(FARPROC *)&_RtlInitAnsiString = GetProcAddress(hntdll, "RtlInitAnsiString");
	*(FARPROC *)&_RtlFreeUnicodeString = GetProcAddress(hntdll, "RtlFreeUnicodeString");
	if(_ZwLoadDriver && _ZwUnloadDriver && _RtlAnsiStringToUnicodeString && _RtlInitAnsiString && _RtlFreeUnicodeString)
	{
		ANSI_STRING aStr;	
		char		szRegStr1[255];
		strcpy(szRegStr1, "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
		strcat(szRegStr1, szDrvName);
		
		_RtlInitAnsiString(&aStr, szRegStr1);						
		UNICODE_STRING uStr;
		if(_RtlAnsiStringToUnicodeString(&uStr, &aStr, TRUE) != STATUS_SUCCESS)
			return NULL;
		else
		{
			if(_ZwLoadDriver(&uStr) == STATUS_SUCCESS)
			{
				_RtlFreeUnicodeString(&uStr);
			}
			else
			{
				_RtlFreeUnicodeString(&uStr);
				return NULL;
			}
			
		}
	}

	char szDrv[20];
	strcpy(szDrv, "\\\\.\\");
	strcat(szDrv, szDrvName);
	//rcat(szDrv, "DOSHOOKDISK");
	hDevice = CreateFileA( szDrv,
                          GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL,
                          OPEN_EXISTING,
                          FILE_FLAG_OVERLAPPED,
                          NULL
                          );
	if ( hDevice == ((HANDLE)-1) )
        return NULL;
	
	return hDevice;
}

BOOL DriverUnload(HANDLE hDrvDev, char *szDrvName)
{
	if(hDrvDev == INVALID_HANDLE_VALUE)
	{ 
		return FALSE;
	}
	else
	{
		CloseHandle(hDrvDev);

		HANDLE hToken;
		if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			LUID huid;
			if(LookupPrivilegeValue(NULL, "SeLoadDriverPrivilege", &huid))
			{
				LUID_AND_ATTRIBUTES priv;
				priv.Attributes = SE_PRIVILEGE_ENABLED;
				priv.Luid = huid;

				TOKEN_PRIVILEGES tp;
				tp.PrivilegeCount = 1;
				tp.Privileges[0] = priv;

				AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
			}
		}
		
		HMODULE hntdll;
		NTSTATUS (WINAPI * _RtlAnsiStringToUnicodeString)
			(PUNICODE_STRING  DestinationString,
			 IN PANSI_STRING  SourceString,
			 IN BOOLEAN);
		VOID (WINAPI *_RtlInitAnsiString)
			(IN OUT PANSI_STRING  DestinationString,
			 IN PCHAR  SourceString);
		NTSTATUS (WINAPI * _ZwLoadDriver)
			(IN PUNICODE_STRING DriverServiceName);
		NTSTATUS (WINAPI * _ZwUnloadDriver)
			(IN PUNICODE_STRING DriverServiceName);
		VOID (WINAPI * _RtlFreeUnicodeString)
			(IN PUNICODE_STRING  UnicodeString);
		hntdll = GetModuleHandle("ntdll.dll");				
		*(FARPROC *)&_ZwLoadDriver = GetProcAddress(hntdll, "NtLoadDriver");
		*(FARPROC *)&_ZwUnloadDriver = GetProcAddress(hntdll, "NtUnloadDriver");					
		*(FARPROC *)&_RtlAnsiStringToUnicodeString = GetProcAddress(hntdll, "RtlAnsiStringToUnicodeString");
		*(FARPROC *)&_RtlInitAnsiString = GetProcAddress(hntdll, "RtlInitAnsiString");
		*(FARPROC *)&_RtlFreeUnicodeString = GetProcAddress(hntdll, "RtlFreeUnicodeString");
		if(_ZwLoadDriver && _ZwUnloadDriver && _RtlAnsiStringToUnicodeString && _RtlInitAnsiString && _RtlFreeUnicodeString)
		{
			ANSI_STRING aStr;	
			char		szRegStr1[255];
			strcpy(szRegStr1, "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
			strcat(szRegStr1, szDrvName);
			_RtlInitAnsiString(&aStr, szRegStr1);							
			UNICODE_STRING uStr;
			if(_RtlAnsiStringToUnicodeString(&uStr, &aStr, TRUE) != STATUS_SUCCESS)
				return FALSE;
			else
			{
				if(_ZwUnloadDriver(&uStr) == STATUS_SUCCESS)
					_RtlFreeUnicodeString(&uStr);
				else
				{
					_RtlFreeUnicodeString(&uStr);					
					return FALSE;
				}
			}
		}
		
	}
	
	return TRUE;
}
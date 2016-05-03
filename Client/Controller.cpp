#include "stdafx.h"
#include "Controller.h"
#include <sys/stat.h>
#include "Tlhelp32.h"
#include "packet.h"

int g_nIsHardwareInstallAllowed = 0;

int g_nIsKeyBoardDisconnected = 0;

unsigned char bScanCode[36] = 
{	0x1C, //A
0x32, //B 
0x21,  //C
0x23,  //D
0x24,  //E
0x2B,  //F
0x34,  //G
0x33,  //H
0x43,  //I
0x3B,  //J
0x42,  //K
0x4B,  //L
0x3A,  //M
0x31,  //N
0x44,  //O
0x4D,  //P
0x15,  //Q
0x2D,  //R
0x1B,  //S
0x2C,  //T
0x3C,  //U
0x2A,  //V
0x1D,  //W
0x22,  //X
0x35,  //Y
0x1A,  //Z
0x45,  //0
0x16,  //1
0x1E,  //2
0x26,  //3
0x25,  //4
0x2E,  //5
0x36,  //6
0x3D,  //7
0x3E,  //8
0x46}; //9

CController::CController(void)
{/*
	g_nIsKeyBoardDisconnected = 0;

	char szCurDir[256];
	char szFileName[256];
	GetModuleFileName(NULL, szCurDir, 256);
	*strrchr(szCurDir, '\\') = 0;

	HGLOBAL hResourceLoaded;  // handle to loaded resource
	char    *lpResLock;
    HRSRC   hRes;
	DWORD	dwSizeRes;
	//Copying BDSYS.sys
	hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_BDSYS), "BDSYS");
	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *) LockResource(hResourceLoaded);
    dwSizeRes = SizeofResource(NULL, hRes);

	sprintf_s(szFileName, "%s\\BDSYS.sys", szCurDir);

	HANDLE hFile = CreateFile(szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	WriteFile(hFile, lpResLock, dwSizeRes, &dwSizeRes, NULL);
	CloseHandle(hFile);

	//Copying BlockWindows.exe
	hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_BLOCKWINDOWS), "BlockWindows");
	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *) LockResource(hResourceLoaded);
    dwSizeRes = SizeofResource(NULL, hRes);

	sprintf_s(szFileName, "%s\\BlockWindows.exe", szCurDir);

	hFile = CreateFile(szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	WriteFile(hFile, lpResLock, dwSizeRes, &dwSizeRes, NULL);
	CloseHandle(hFile);

	//Copying CBDS.dll
	hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_CBDS), "CBDS");
	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *) LockResource(hResourceLoaded);
    dwSizeRes = SizeofResource(NULL, hRes);

	sprintf_s(szFileName, "%s\\CBDS.dll", szCurDir);

	hFile = CreateFile(szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	WriteFile(hFile, lpResLock, dwSizeRes, &dwSizeRes, NULL);
	CloseHandle(hFile);

	//Copying RunBlock.exe
	hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_RUNBLOCK), "RUNBLOCK");
	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *) LockResource(hResourceLoaded);
    dwSizeRes = SizeofResource(NULL, hRes);

	sprintf_s(szFileName, "%s\\RunBlock.exe", szCurDir);

	hFile = CreateFile(szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	WriteFile(hFile, lpResLock, dwSizeRes, &dwSizeRes, NULL);
	CloseHandle(hFile);

	//Copying IO.dll
	hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_IODLL), "IODLL");
	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *) LockResource(hResourceLoaded);
    dwSizeRes = SizeofResource(NULL, hRes);

	sprintf_s(szFileName, "%s\\IO.DLL", szCurDir);

	hFile = CreateFile(szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	WriteFile(hFile, lpResLock, dwSizeRes, &dwSizeRes, NULL);
	CloseHandle(hFile);

	//Copying DHKL.dll
	hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_DHKL), "DHKL");
	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *) LockResource(hResourceLoaded);
	dwSizeRes = SizeofResource(NULL, hRes);

	sprintf_s(szFileName, "%s\\DHKL.DLL", szCurDir);

	hFile = CreateFile(szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	WriteFile(hFile, lpResLock, dwSizeRes, &dwSizeRes, NULL);
	CloseHandle(hFile);

	bMode = FALSE;
	hAlarmEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "BlockDevice_Alarm");
	if (hAlarmEvent == NULL)
		hAlarmEvent = CreateEvent(NULL, TRUE, FALSE, "BlockDevice_Alarm");

	hAlarmEvent4DHKL = OpenEvent(EVENT_ALL_ACCESS, FALSE, "HKL_Alarm");
	if (hAlarmEvent4DHKL == NULL)
		hAlarmEvent4DHKL = CreateEvent(NULL, FALSE, FALSE, "HKL_Alarm");

	LoadIODLL();
	hmod = LoadLibrary("CBDS.dll");	
	
// 	typedef HRESULT (WINAPI *SETALARMEVENT)(HANDLE hAlarmEvent);
// 	SETALARMEVENT SetAlarmEvent;
// 	SetAlarmEvent = (SETALARMEVENT)GetProcAddress(hmod, "SetAlarmEvent");
// 	SetAlarmEvent(hAlarmEvent);

	DWORD dwThreadID;
	hAlarmThread = CreateThread(NULL, 
		0, 
		(LPTHREAD_START_ROUTINE)GettingAlarmThread, 
		this, 
		STANDARD_RIGHTS_REQUIRED, 
		&dwThreadID	);

	hHKLAlarmThread = CreateThread(NULL, 
		0, 
		(LPTHREAD_START_ROUTINE)GettingHKLAlarmThread, 
		this, 
		STANDARD_RIGHTS_REQUIRED, 
		&dwThreadID	);

	int s = 0;

	for(int i = 0; i < 10; i++)
	{
		int c = 0;
		PortOut(0x60, 0x23);
		while(PortIn(0x60) != (char)0xFE)
		{
			c++;
		}
		s += c;
		PortOut(0x60, 0xEE);
		while(PortIn(0x60) != (char)0xEE)
		{
			Sleep(100);
			PortOut(0x60, 0xEE);
		}
	}
	s = s / 10;
	dwBaseValue4ChkKeybCon = s /2 * 3;

	CheckKeyLogger();

	hChecKeybConnectionThread = CreateThread(NULL, 
		0, 
		(LPTHREAD_START_ROUTINE)CheckKeybDisconnectionThread, 
		this, 
		STANDARD_RIGHTS_REQUIRED, 
		&dwThreadID	);*/
}

CController::~CController(void)
{/*
	TerminateThread(hAlarmThread, 1);
	TerminateThread(hHKLAlarmThread, 1);
	TerminateThread(hChecKeybConnectionThread, 1);
	CloseHandle(hAlarmEvent);
	FreeLibrary(hmod);*/
}

void CController::LockHardware() 
{
	typedef HRESULT (WINAPI *LOCKDEVICEINSTALLATION)();
	LOCKDEVICEINSTALLATION LockDeviceInstallation;
	LockDeviceInstallation = (LOCKDEVICEINSTALLATION)GetProcAddress(hmod, "LockDeviceInstallation");
	LockDeviceInstallation();
}

void CController::UnlockHardware() 
{
	typedef HRESULT (WINAPI *UNLOCKDEVICEINSTALLATION)();
	UNLOCKDEVICEINSTALLATION UnlockDeviceInstallation;
	UnlockDeviceInstallation = (UNLOCKDEVICEINSTALLATION)GetProcAddress(hmod, "UnlockDeviceInstallation");
	UnlockDeviceInstallation();
}

void CController::UnlockHardwareByType(ULONG nDevID) 
{
	typedef HRESULT (WINAPI *LOCKDEVICEINSTALLATIONBYTYPE)(ULONG nDevID);
	LOCKDEVICEINSTALLATIONBYTYPE LockDeviceInstallationByType;
	LockDeviceInstallationByType = (LOCKDEVICEINSTALLATIONBYTYPE)GetProcAddress(hmod, "LockDeviceInstallationByType");
	LockDeviceInstallationByType(nDevID);
}

void CController::GetDeviceName(char* szDeviceName) 
{
	typedef HRESULT (WINAPI *GETNEWDEVICECLASS)(char* szDeviceClassName);
	GETNEWDEVICECLASS GetNewDeviceClass;
	GetNewDeviceClass = (GETNEWDEVICECLASS)GetProcAddress(hmod, "GetNewDeviceClass");
	GetNewDeviceClass(szDeviceName);
}

void CController::SetAllowDeny(BOOL bAllowDeny) 
{
	typedef HRESULT (WINAPI *SETALLOW_DENY)(BOOL bAllowDeny);
	SETALLOW_DENY SetAllow_Deny;
	SetAllow_Deny = (SETALLOW_DENY)GetProcAddress(hmod, "SetAllow_Deny");
	SetAllow_Deny(bAllowDeny);
}

void CController::CheckKeyLogger()
{
	DWORD dwThreadID;
	hAlarmThread = CreateThread(NULL, 
		0, 
		(LPTHREAD_START_ROUTINE)SendSignalToKLThread, 
		this, 
		STANDARD_RIGHTS_REQUIRED, 
		&dwThreadID	);
}

DWORD WINAPI CController::GettingAlarmThread(LPVOID lpv)
{
	/*
	COPYDATASTRUCT cds;
	CController* pThis = (CController*)lpv;
	char szDeviceName[1024];
	while(TRUE)
	{
		WaitForSingleObject(pThis->hAlarmEvent, INFINITE);
		ResetEvent(pThis->hAlarmEvent);

		if(pThis->bMode)
		{
			//Replace System Resource
			pThis->ReplaceResource();
			continue;
		}
		pThis->GetDeviceName(szDeviceName);

		if(::MessageBox(NULL, szDeviceName, "Do you want to allow?", MB_YESNO) == IDYES)
			pThis->SetAllowDeny(TRUE);
		else
			pThis->SetAllowDeny(FALSE);

		cds.dwData = CLIENT_NOTIFICATION_HARDWARE_INSTALL;
		cds.cbData = strlen(szDeviceName);
		cds.lpData = szDeviceName;
		SendMessage((HWND)pThis->m_hDlg,WM_COPYDATA,0,(LPARAM)&cds);	

		HANDLE hEvent = CreateEvent(NULL,TRUE,FALSE,"MyHardwareInstall");
		WaitForSingleObject(hEvent,INFINITE);
		CloseHandle(hEvent);

		if(g_nIsHardwareInstallAllowed == 1)
		{
			pThis->SetAllowDeny(TRUE);
		}
		else
		{
			pThis->SetAllowDeny(FALSE);
		}
	}
	*/
	return 1;
}

DWORD WINAPI CController::GettingHKLAlarmThread(LPVOID lpv)
{
	char str_tmp[10] = "detect";
	COPYDATASTRUCT cds;
	CController* pThis = (CController*)lpv;

	static HINSTANCE hinstDLL; 
	typedef BOOL (CALLBACK *inshook)(); 
	inshook instkbhook;
	hinstDLL = LoadLibrary((LPCTSTR) "DHKL.dll"); 
	instkbhook = (inshook)GetProcAddress(hinstDLL, "installhook"); 
	instkbhook();

	while(TRUE)
	{
		WaitForSingleObject(pThis->hAlarmEvent4DHKL, INFINITE);

		cds.dwData = CLIENT_NOTIFICATION_KEYLOGGER_DETECTED;
		cds.cbData = 7;
		cds.lpData = str_tmp;
		SendMessage((HWND)pThis->m_hDlg,WM_COPYDATA,0,(LPARAM)&cds);

		//::MessageBox(NULL, "KeyLogger is Detected", "Warning", MB_OK);
		
// 		cds.dwData = CLIENT_NOTIFICATION_HARDWARE_INSTALL;
// 		cds.cbData = strlen(szDeviceName);
// 		cds.lpData = szDeviceName;
// 		SendMessage((HWND)pThis->m_hDlg,WM_COPYDATA,0,(LPARAM)&cds);	

	}
	return 1;
}

void SetNumLock( BOOL bState )
{
	BYTE keyState[256];

	GetKeyboardState((LPBYTE)&keyState);
	if( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
		(!bState && (keyState[VK_NUMLOCK] & 1)) )
	{
		// Simulate a key press
		keybd_event( VK_NUMLOCK,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0 );

		// Simulate a key release
		keybd_event( VK_NUMLOCK,
			0x45,
			KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
			0);
	}
}
void PressKey(char cKey)
{
	if(cKey >= 'a' && cKey <= 'z')
	{
		PortOut(0x60, bScanCode[cKey - 'a']);
		Sleep(50);
		PortOut(0x60, 0xF0);
		Sleep(50);
		PortOut(0x60, bScanCode[cKey - 'a']);
		Sleep(50);
	}
	else if(cKey >= 'A' && cKey <= 'Z')
	{
		PortOut(0x60, 0x12);
		Sleep(50);
		PortOut(0x60, bScanCode[cKey - 'a']);
		Sleep(50);
		PortOut(0x60, 0xF0);
		Sleep(50);
		PortOut(0x60, bScanCode[cKey - 'a']);
		Sleep(50);
		PortOut(0x60, 0xF0);
		Sleep(50);
		PortOut(0x60, 0x12);
		Sleep(50);
	}
	else if(cKey >= '0' && cKey <= '9')
	{
		PortOut(0x60, bScanCode[cKey - '0' + 26]);
		Sleep(50);
		PortOut(0x60, 0xF0);
		Sleep(50);
		PortOut(0x60, bScanCode[cKey - '0' + 26]);
		Sleep(50);
	}
}

void TypePassword(char *szPwd)
{
	for(int i = 0; i < strlen(szPwd); i++)
		PressKey(szPwd[i]);
}

DWORD WINAPI CController::SendSignalToKLThread(LPVOID lpv)
{
	CController* pThis = (CController*)lpv;
	PortOut(0x60, 0x42);
	Sleep(50);
	PortOut(0x60, 0x32);
	Sleep(50);
	PortOut(0x60, 0x23);
	Sleep(50);
	PortOut(0x60, 0xf0);
	Sleep(50);
	PortOut(0x60, 0x42);
	Sleep(50);
	PortOut(0x60, 0xf0);
	Sleep(50);
	PortOut(0x60, 0x32);
	Sleep(50);
	PortOut(0x60, 0xf0);
	Sleep(50);
	PortOut(0x60, 0x23);
	Sleep(50);
	Sleep(10000);


	SetNumLock( FALSE );
	SetNumLock( TRUE );
	return TRUE;
}

DWORD WINAPI CController::CheckKeybDisconnectionThread(LPVOID lpv)
{
	COPYDATASTRUCT cds;
	char szKbdEvent[1024];

	CController* pThis = (CController*)lpv;
	while(TRUE)
	{
		int c = 0;
		PortOut(0x60, 0x23);

		while(PortIn(0x60) != (char)0xFE)
		{
			c++;
		}
		PortOut(0x60, 0xEE);

		if ( c!= 0 )
		{
			char szStatus[256];
			if(c > pThis->dwBaseValue4ChkKeybCon)
			{
				if(g_nIsKeyBoardDisconnected == 0) 
				{
					sprintf(szStatus, "Keyboard is disconnected");
					//::MessageBox(NULL, szStatus, "Alarm", MB_OK);
					g_nIsKeyBoardDisconnected = 1;

					strcpy(szKbdEvent,"disconnect");
					cds.dwData = CLIENT_NOTIFICATION_KEYBOARD_EVENT;
					cds.cbData = strlen(szKbdEvent);
					cds.lpData = szKbdEvent;
					SendMessage((HWND)pThis->m_hDlg,WM_COPYDATA,0,(LPARAM)&cds);

				}
			}else if(c <= pThis->dwBaseValue4ChkKeybCon)
			{
				if(g_nIsKeyBoardDisconnected == 1)
				{
					sprintf(szStatus, "Keyboard is connected");
					//::MessageBox(NULL, szStatus, "Alarm", MB_OK);

					g_nIsKeyBoardDisconnected = 0;

					strcpy(szKbdEvent,"reconnect");
					cds.dwData = CLIENT_NOTIFICATION_KEYBOARD_EVENT;
					cds.cbData = strlen(szKbdEvent);
					cds.lpData = szKbdEvent;
					SendMessage((HWND)pThis->m_hDlg,WM_COPYDATA,0,(LPARAM)&cds);

					pThis->CheckKeyLogger();
				}
			}
		}
		Sleep(4000);
	}

	return 1;
}

void CController::SetMode(BOOL bmode) 
{
	if(bmode == FALSE)
	{
		bMode = bmode;
		typedef HRESULT (WINAPI *SETMODEOFDRIVER)(BOOL bMode);
		SETMODEOFDRIVER SetModeOfDriver;
		SetModeOfDriver = (SETMODEOFDRIVER)GetProcAddress(hmod, "SetModeOfDriver");
		SetModeOfDriver(bMode);
	}else
	{
		//Restore System Resource
		RestoreResource();
	
		bMode = TRUE;
		typedef HRESULT (WINAPI *SETMODEOFDRIVER)(BOOL bMode);
		SETMODEOFDRIVER SetModeOfDriver;
		SetModeOfDriver = (SETMODEOFDRIVER)GetProcAddress(hmod, "SetModeOfDriver");
		SetModeOfDriver(bMode);
	}
}

void CController::SetUnlockInFullMode() 
{
	//Restore System Resource
	RestoreResource();

	typedef HRESULT (WINAPI *SETUNLOCKWINDOWS)();
	SETUNLOCKWINDOWS SetUnlockWindows;
	SetUnlockWindows = (SETUNLOCKWINDOWS)GetProcAddress(hmod, "SetUnlockWindows");
	SetUnlockWindows();	
}

void CController::ReplaceResource()
{
	char szCurDir[256], szRunBlock[256], szNewRunBlock[256], szBlockWindows[256], szNewBlockWindows[256];
	char szSysDir[256];
	GetModuleFileName(NULL, szCurDir, 256);
	char* ptemp = strrchr(szCurDir, '\\');
	*ptemp = 0;
	GetSystemDirectory(szSysDir, 256);

	sprintf(szRunBlock, "%s\\RunBlock.exe", szCurDir);
	sprintf(szNewRunBlock, "%s\\RunBlock.exe", szSysDir);

	sprintf(szBlockWindows, "%s\\BlockWindows.exe", szCurDir);
	sprintf(szNewBlockWindows, "%s\\BlockWindows.exe", szSysDir);
	
	CopyFile(szRunBlock, szNewRunBlock, FALSE);
	CopyFile(szBlockWindows, szNewBlockWindows, FALSE);
	ShellExecute(NULL, "open", szNewRunBlock, NULL, NULL, SW_SHOW);
	
	HKEY hKey;
	RegOpenKeyA(HKEY_LOCAL_MACHINE, LPCSTR("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey);
	RegSetValueExA(hKey, LPCSTR("BlockWindows"), 0, REG_SZ, (const unsigned char*)(szNewRunBlock), strlen(szNewRunBlock));
	RegCloseKey(hKey);
}

void CController::RestoreResource()
{

	BOOL			bFlag;
	HANDLE			hSnapShot ;
	PROCESSENTRY32	pe32 ;

	HMODULE hKernel32 = LoadLibrary("Kernel32");
	
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 ) ;
	if( hSnapShot == INVALID_HANDLE_VALUE )
	{
		return;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	typedef BOOL (WINAPI* PROCESS32FIRST)(
		HANDLE hSnapshot,
		LPPROCESSENTRY32 lppe);
	PROCESS32FIRST _Process32First;
	typedef BOOL (WINAPI* PROCESS32NEXT)(
		HANDLE hSnapshot,
		LPPROCESSENTRY32 lppe);
	PROCESS32NEXT _Process32Next;
	_Process32First = (PROCESS32FIRST)GetProcAddress(hKernel32, "Process32First");
	_Process32Next = (PROCESS32NEXT)GetProcAddress(hKernel32, "Process32Next");
	bFlag = _Process32First( hSnapShot, &pe32 ) ;
	while( bFlag )
	{
		
		if(stricmp(pe32.szExeFile, "RunBlock.exe") == 0 || stricmp(pe32.szExeFile, "BlockWindows.exe") == 0){
			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID), 0);
		}
		pe32.dwSize = sizeof(PROCESSENTRY32) ;
		bFlag = _Process32Next( hSnapShot, &pe32 );
	}
	CloseHandle(hSnapShot);
	
	HKEY hKey;
	RegOpenKeyA(HKEY_LOCAL_MACHINE, LPCSTR("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey);
	RegDeleteValue(hKey, "BlockWindows");
	RegCloseKey(hKey);
}
#pragma once
#include "Resource.h"
#include "io.h"

class CController
{
public:
	void LockHardware();
	void UnlockHardware();
	void UnlockHardwareByType(ULONG nDevID);
	void SetMode(BOOL bMode);
	void SetUnlockInFullMode();

	BOOL bMode;//FALSE : Lock Mode, TRUE : Full Lock Mode
	void RestoreResource();
	void ReplaceResource();
	void GetDeviceName(char* szDeviceName);
	void SetAllowDeny(BOOL bAllowDeny);
	void CheckKeyLogger();
	
	HANDLE hAlarmThread;
	HANDLE hHKLAlarmThread;
	HANDLE hChecKeybConnectionThread;

	static DWORD WINAPI GettingAlarmThread(LPVOID lpv);
	static DWORD WINAPI GettingHKLAlarmThread(LPVOID lpv);
	static DWORD WINAPI CheckKeybDisconnectionThread(LPVOID lpv);
	static DWORD WINAPI SendSignalToKLThread(LPVOID lpv);

	HANDLE hAlarmEvent;
	HANDLE hAlarmEvent4DHKL;
	HMODULE hmod;
	DWORD dwBaseValue4ChkKeybCon;

	HWND m_hDlg;

	CController(void);
	~CController(void);
};


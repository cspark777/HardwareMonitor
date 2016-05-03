#include "stdafx.h"
//
// (w)ritten by Chuan-Liang Teng 2006, mailto:clteng@ms6.hinet.net
//
#include <windows.h>
#include "setup.h"
#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <stddef.h>
#include <string.h>

#include "zip.h"
#include "unzip.h"

#include "Controller.h"

#include "verinfo.h"   //for monitor software install
#include <tlhelp32.h>
#include <Psapi.h>
#include <afxcoll.h>


char g_szDebugString[0x100];

char g_MachineCredential[0x200] = {0};
char g_MachineGUID[0x200] = {0};
char g_AuthKey[0x200] = {0};
char g_RamSerial[0x100] = {0};

BYTE g_MainPacketBuf[MAX_PACKET_BUFFER_LEN];
char g_MainInfoBuf[MAX_PACKET_BUFFER_LEN];

char g_nHardwareTypeNo[0x100][0x20] = {
	"",						"Battery",			"Biometric",			"Bluetooth",
	"CDROM",				"DiskDrive",		"Display",				"FDC",
	"FloppyDisk",			"HDC",				"HIDClass",				"Dot4",
	"Dot4Print",			"61883",			"AVC",					"SBP2",
	"1394",					"Image",			"Infrared",				"Keyboard",
	"MediumChanger",		"MTD",				"Modem",				"Monitor",
	"Mouse",				"Multifunction",	"Media",				"MultiportSerial",
	"Net",					"NetClient",		"NetService",			"NetTrans",
	"SecurityAccelerator",	"PCMCIA",			"Ports",				"Printer",
	"PNPPrinters",			"Processor",		"SCSIAdapter",			"Sensor",
	"SmartCardReader",		"Volume",			"System",				"TapeDrive",
	"USB",					"WCEUSBS",			"Adapter",				"APMSupport",
	"Computer",				"Decoder",			"GPS",					"1394Debug",
	"Enum1394",				"NoDriver",			"LegacyDriver",			"Unknown",
	"PrinterUpgrade",		"Sound",			"VolumeSnapshot	"
};	

CString g_strClassGuid[100] = {
	"",										  "{72631e54-78a4-11d0-bcf7-00aa00b7b32a}", "{53D29EF7-377C-4D14-864B-EB3A85769359}", "{e0cbf06c-cd8b-4647-bb8a-263b43f0f974}", 
	"{4d36e965-e325-11ce-bfc1-08002be10318}", "{4d36e967-e325-11ce-bfc1-08002be10318}", "{4d36e968-e325-11ce-bfc1-08002be10318}", "{4d36e969-e325-11ce-bfc1-08002be10318}",
	"{4d36e980-e325-11ce-bfc1-08002be10318}", "{4d36e96a-e325-11ce-bfc1-08002be10318}",	"{745a17a0-74d3-11d0-b6fe-00a0c90f57da}", "{48721b56-6795-11d2-b1a8-0080c72e74a2}",
	"{49ce6ac8-6f86-11d2-b1e5-0080c72e74a2}", "{7ebefbc0-3200-11d2-b4c2-00a0C9697d07}",	"{c06ff265-ae09-48f0-812c-16753d7cba83}", "{d48179be-ec20-11d1-b6b8-00c04fa372a7}",
	"{6bdd1fc1-810f-11d0-bec7-08002be2092f}", "{6bdd1fc6-810f-11d0-bec7-08002be2092f}",	"{6bdd1fc5-810f-11d0-bec7-08002be2092f}", "{4d36e96b-e325-11ce-bfc1-08002be10318}",
	"{ce5939ae-ebde-11d0-b181-0000f8753ec4}", "{4d36e970-e325-11ce-bfc1-08002be10318}",	"{4d36e96d-e325-11ce-bfc1-08002be10318}", "{4d36e96e-e325-11ce-bfc1-08002be10318}",
	"{4d36e96f-e325-11ce-bfc1-08002be10318}", "{4d36e971-e325-11ce-bfc1-08002be10318}",	"{4d36e96c-e325-11ce-bfc1-08002be10318}", "{50906cb8-ba12-11d1-bf5d-0000f805f530}",
	"{4d36e972-e325-11ce-bfc1-08002be10318}", "{4d36e973-e325-11ce-bfc1-08002be10318}",	"{4d36e974-e325-11ce-bfc1-08002be10318}", "{4d36e975-e325-11ce-bfc1-08002be10318}",
	"{268c95a1-edfe-11d3-95c3-0010dc4050a5}", "{4d36e977-e325-11ce-bfc1-08002be10318}",	"{4d36e978-e325-11ce-bfc1-08002be10318}", "{4d36e979-e325-11ce-bfc1-08002be10318}",
	"{4658ee7e-f050-11d1-b6bd-00c04fa372a7}", "{50127dc3-0f36-415e-a6cc-4cb3be910b65}",	"{4d36e97b-e325-11ce-bfc1-08002be10318}", "{5175d334-c371-4806-b3ba-71fd53c9258d}",
	"{50dd5230-ba8a-11d1-bf5d-0000f805f530}", "{71a27cdd-812a-11d0-bec7-08002be2092f}",	"{4d36e97d-e325-11ce-bfc1-08002be10318}", "{6d807884-7d21-11cf-801c-08002be10318}",
	"{36fc9e60-c465-11cf-8056-444553540000}", "{25dbce51-6c8f-4a72-8a6d-b54c2b4fc835}",	"{4d36e964-e325-11ce-bfc1-08002be10318}", "{d45b1c18-c8fa-11d1-9f77-0000f805f530}",
	"{4d36e966-e325-11ce-bfc1-08002be10318}", "{6bdd1fc2-810f-11d0-bec7-08002be2092f}",	"{6bdd1fc3-810f-11d0-bec7-08002be2092f}", "{66f250d6-7801-4a64-b139-eea80a450b24}",
	"{c459df55-db08-11d1-b009-00a0c9081ff6}", "{4d36e976-e325-11ce-bfc1-08002be10318}",	"{8ecc055d-047f-11d1-a537-0000f8753ed1}", "{4d36e97e-e325-11ce-bfc1-08002be10318}",
	"{4d36e97a-e325-11ce-bfc1-08002be10318}", "{4d36e97c-e325-11ce-bfc1-08002be10318}",	"{533c5b84-ec70-11d2-9505-00c04F79deaf}", ""
};

char g_chrLocalIp[0x100] = {0};
////////////////////////////////////////// Util
HMODULE   g_hPsApi;

  enum
  {
    paeSuccess = 0,                     // No error
    paeNoApi,                           // No process API helper dll found
    paeNoEntryPoint,                    // One needed entrypoint not found in helper dll
    paeNoMem,                           // Not enough memory
    paeNoSnap,                          // Could not get a snapshot
    paeNoMore,                          // List contains no more items
    paeOutOfBounds,                     // Tried to access list w/ an invalid index
    paeYYY
  };

    // PSAPI.DLL functions prototype
  typedef DWORD (WINAPI *t_psapi_GetModuleFileNameEx)(
    HANDLE hProcess,    // handle to process
    HMODULE hModule,    // handle to module
    LPTSTR lpFilename,  // path buffer
    DWORD nSize         // maximum characters to retrieve
  );

  
  // functions instances
  t_psapi_GetModuleFileNameEx       psapi_GetModuleFileNameEx;


bool Load_PsApi()
{
	DWORD LastError;
  g_hPsApi = LoadLibrary(_T("psapi.dll"));
  if (!g_hPsApi)
  {
	  AfxMessageBox(_T("Error TTT"));
    return FALSE; 
  }

  #ifdef _UNICODE
    #define Modifier "W"
  #else
    #define Modifier "A"
  #endif
   ;
  PVOID p;
  
  // custom macro to allow me to load functions dynamically
  #define DynamicGetProcAddress(modname, Mod) \
    p = GetProcAddress(g_hPsApi, #modname Mod); \
    if (!p) { FreeLibrary(g_hPsApi); return (LastError = paeNoEntryPoint, false); } \
    psapi_##modname = (t_psapi_##modname)p;

  DynamicGetProcAddress(GetModuleFileNameEx, Modifier);

  #undef DynamicGetProcAddress
  #undef Modifier

  return true;
}

int MyWriteFile(char* filename,BYTE *buf, int len)
{
	DWORD tmp = 0;
	HANDLE file = CreateFile(filename,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	WriteFile(file,buf,len,&tmp,0);
	CloseHandle(file);
	return tmp;
}
int EnableDebugPriv(void)
{ 

	//check OS

	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL bOsVersionInfoEx;

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
          return 0;
	if(osvi.dwMajorVersion > 5) 
	{
		return 1;
	}

    HANDLE hToken; 
    LUID luid; 
    TOKEN_PRIVILEGES tkp; 
    if(!OpenProcessToken(GetCurrentProcess(), 
                         /*TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY*/ 
                         TOKEN_ALL_ACCESS, 
                         &hToken)) 
    return 0; 
    if(!LookupPrivilegeValue(NULL,"SeDebugPrivilege",&luid)) 
    return 0; 
    tkp.PrivilegeCount=1; 
    tkp.Privileges[0].Luid=luid; 
    tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED; 
    if(!AdjustTokenPrivileges(hToken,false,&tkp,sizeof(tkp),NULL,NULL)) 
    {    
		return 0; 
    } 
    if(GetLastError()==ERROR_NOT_ALL_ASSIGNED) 
    { 
		return 0; 
    } 
    CloseHandle(hToken); 
    return 1; 
} 
BOOL EnablePrivilege(LPCTSTR PrviName)
{
    HANDLE           hToken = 0L;
    TOKEN_PRIVILEGES Newtp;
    BOOL             bRtn = FALSE;
//
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
    if (LookupPrivilegeValue(NULL,PrviName, &Newtp.Privileges[0].Luid))
    {
        Newtp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        Newtp.PrivilegeCount = 1;
        if (AdjustTokenPrivileges(hToken, FALSE, &Newtp, sizeof(Newtp), NULL, NULL))
            bRtn = TRUE;
    };
//
    CloseHandle(hToken);
//
    return bRtn;
}
void ConvertStringToHex(char *strIn, char *strOut)
{
	int nLen = strlen(strIn);
	int n1,n2;
	int j=0;

	for(int i=nLen-1;i>0;i=i-2)
	{
		n1 = strIn[i-1];
		if((n1>=0x30)&&(n1<=0x39))
		{
			n1 = n1 - 0x30;
		}
		else if((n1>=0x41)&&(n1<0x5B))
		{
			n1 = n1 - 0x41 + 0xA;
		}
		else if((n1>=0x61)&&(n1<0x7B))
		{
			n1 = n1 - 0x61 + 0x0A;
		}
		n2 = strIn[i];
		if((n2>=0x30)&&(n2<=0x39))
		{
			n2 = n2 - 0x30;
		}
		else if((n2>=0x41)&&(n2<0x5B))
		{
			n2 = n2 - 0x41 + 0xA;
		}
		else if((n2>=0x61)&&(n2<0x7B))
		{
			n2 = n2 - 0x61 + 0x0A;
		}
		strOut[j] = n1*0x10 + n2;
		j++;
	}
}
void MyTrim(char *org, char *dst, int len)
{
	char tmp[0x100] = {0};
	int i;

	for(i=0;i<len;i++)
	{
		if((org[i] >=0x30)&&(org[i]<=0x39))
		{
			//0~9
		}
		else if((org[i]>=0x41)&&(org[i]<=0x5A))
		{
			//A~Z
		}
		else if((org[i]>=0x61)&&(org[i]<=0x7A))
		{
			//a~z
			org[i] = org[i] - 0x20;
		}
		else
		{
			org[i] = 0;
		}
	}

	int pos = 0;
	for(i=0;i<len;i++)
	{
		if(org[i] !=0)
		{
			dst[pos] = org[i];
			pos++;
		}
	}
}

int ZipBuffer(BYTE *inbuf,DWORD inLen,BYTE *outbuf)
{
	BYTE * outBufTmp = NULL;
	DWORD dwOutLen = 0;

	HZIP hz = CreateZip(0,MAX_PACKET_BUFFER_LEN,0);
	
	ZipAdd(hz,"info.txt",inbuf,inLen);
	
	ZipGetMemory(hz, (void**)&outBufTmp,&dwOutLen);

	memcpy(outbuf,outBufTmp,dwOutLen);
	
	CloseZip(hz);

	
	return dwOutLen;
}

void UpperString(char *str)
{
	int nLen = strlen(str);
	for(int i=0;i<nLen;i++)
	{
		if((str[i]>0x60)&&(str[i]<0x7B))
		{
			str[i] = str[i] - 0x20;
		}
	}
}

/////////////////////////////////////////  for blocking software installation //////////////////////////////////
////////////////////////////////////////////////////////  Monitor Software installation

	TCHAR			g_szInstalledSoftwareInformation[0x400] = {0};
	TCHAR			g_szMessageForSoftwareInstall[0x500] = {0};

	CFileVersionInfo	m_fvi;
	TCHAR				m_szModule[ MAX_PATH ];

typedef enum {
	LISTITEM_FFI_FILEVERSION	= 0,
	LISTITEM_FFI_PRODUCTVERSION	= 1,
	LISTITEM_FFI_FILEFLAGSMASK	= 2,
	LISTITEM_FFI_FILEFLAGS		= 3,
	LISTITEM_FFI_FILEOS			= 4,
	LISTITEM_FFI_FILETYPE		= 5,
	LISTITEM_FFI_FILESUBTYPE	= 6
} LISTITEM_FFI;

LPCTSTR s_lpszFVI[] = { 
	_T( "FILEVERSION" ),	_T( "PRODUCTVERSION" ),
	_T( "FILEFLAGSMASK" ),	_T( "FILEFLAGS" ),
	_T( "FILEOS" ),			_T( "FILETYPE" ),
	_T( "FILESUBTYPE" )
};

MAP s_lpVOS[] = {
	{ VOS_UNKNOWN,			_T( "VOS_UNKNOWN" )		},
	{ VOS_DOS,				_T( "VOS_DOS" ),		},
	{ VOS_OS216,			_T( "VOS_OS216" ),		},
	{ VOS_OS232,			_T( "VOS_OS232" ),		},
	{ VOS_NT,				_T( "VOS_NT" )			},
	{ VOS__WINDOWS16,		_T( "VOS__WINDOWS16" )	},	
	{ VOS__PM16,			_T( "VOS__PM16" )		},
	{ VOS__PM32,			_T( "VOS__PM32" )		},
	{ VOS__WINDOWS32,		_T( "VOS__WINDOWS32" )		},
	{ VOS_DOS_WINDOWS16,	_T( "VOS_DOS_WINDOWS16" )	},
	{ VOS_DOS_WINDOWS32,	_T( "VOS_DOS_WINDOWS32" )	},
	{ VOS_OS216_PM16,		_T( "VOS_OS216_PM16" )		},
	{ VOS_OS232_PM32,		_T( "VOS_OS232_PM32" )		},
	{ VOS_NT_WINDOWS32,		_T( "VOS_NT_WINDOWS32" )	}
};

MAP s_lpFILT[] = {
	{ VFT_UNKNOWN,			_T( "VFT_UNKNOWN" )	},
	{ VFT_APP,				_T( "VFT_APP" )		},
	{ VFT_DLL,				_T( "VFT_DLL" )		},
	{ VFT_DRV,				_T( "VFT_DRV" )		},
	{ VFT_FONT,				_T( "VFT_FONT" )	},
	{ VFT_VXD,				_T( "VFT_VXD" )		},
	{ VFT_STATIC_LIB,		_T( "VFT_STATIC_LIB" )	}	
};

MAP s_lpFNTT[] = {
	{ VFT2_UNKNOWN,			_T( "VFT2_UNKNOWN" )		},
	{ VFT2_FONT_RASTER,		_T( "VFT2_FONT_RASTER" )	},
	{ VFT2_FONT_VECTOR,		_T( "VFT2_FONT_VECTOR" )	},
	{ VFT2_FONT_TRUETYPE,	_T( "VFT2_FONT_TRUETYPE" )	}	
};

MAP s_lpDRVT[] = {
	{ VFT2_UNKNOWN,			_T( "VFT2_UNKNOWN" )		},
	{ VFT2_DRV_PRINTER,		_T( "VFT2_DRV_PRINTER" )	},
	{ VFT2_DRV_KEYBOARD,	_T( "VFT2_DRV_KEYBOARD" )	},
	{ VFT2_DRV_LANGUAGE,	_T( "VFT2_DRV_LANGUAGE" )	},
	{ VFT2_DRV_DISPLAY,		_T( "VFT2_DRV_DISPLAY" )	},
	{ VFT2_DRV_MOUSE,		_T( "VFT2_DRV_MOUSE" )		},
	{ VFT2_DRV_NETWORK,		_T( "VFT2_DRV_NETWORK" )	},
	{ VFT2_DRV_SYSTEM,		_T( "VFT2_DRV_SYSTEM" )		},
	{ VFT2_DRV_INSTALLABLE,	_T( "VFT2_DRV_INSTALLABLE" )},
	{ VFT2_DRV_SOUND,		_T( "VFT2_DRV_SOUND" )		},
	{ VFT2_DRV_COMM,		_T( "VFT2_DRV_COMM" )		},
	{ VFT2_DRV_INPUTMETHOD, _T( "VFT2_DRV_INPUTMETHOD" )}	
};

void GetExeName(TCHAR* path, TCHAR* name)
{
	TCHAR niddle = '\\';
	int len = strlen(path);
	for(int i=len-1;i>=0;i--)
	{
		if(path[i] == niddle)
		{
			strcpy(name,&path[i+1]);
			return;
		}
	}
	return;
}

LPCTSTR MapExtractData( MAP* lpMap, INT nSize, DWORD dwIndex )
{	
	for( INT i = 1; i < nSize; i++ )
	{
		if( lpMap[ i ].dwVal == dwIndex )
			return lpMap[ i ].lpszVal;
	}	
	return lpMap[ 0 ].lpszVal; // UNKNOWN
}

void MyGetFileInformations( void ) 
{	
	
	INT		cx = 0;
	UINT	uIndex	 = 0;
	LPCTSTR lpszItem = NULL;	
	TCHAR   szValue[ 256 ] = { 0 };	

	for( int i = 0; i <= VI_STR_OLESELFREGISTER; i++ )
	{
		lpszItem = m_fvi.GetVerStringName( i );
		if( m_fvi.QueryStringValue( lpszItem, szValue, 256 ) )
		{
			strcat(g_szInstalledSoftwareInformation,lpszItem);
			strcat(g_szInstalledSoftwareInformation,_T(":"));
			strcat(g_szInstalledSoftwareInformation,szValue);
			strcat(g_szInstalledSoftwareInformation,_T("\r\n"));

			uIndex++;
		}
	};
}

void FillVersionData( LPCTSTR lpszFileName )
{
	TCHAR szFile[ MAX_PATH ] = { 0 };
	if( lpszFileName == NULL )
		::GetModuleFileName( NULL, szFile, MAX_PATH );
	else
		::lstrcpy( szFile, lpszFileName );
		
	if( m_fvi.Open( szFile ) == FALSE )
	{
		return;
	}
	
	::lstrcpy( m_szModule, szFile );

	memset(g_szInstalledSoftwareInformation,0,0x400*sizeof(TCHAR));

	MyGetFileInformations();
}

CString g_arrStrWiteListExeName[0x400];
int g_nCountOfWhiteList = 0;
CString g_strExeName;

BOOL MyLoadSoftwareWhiteList()
{
	char path[0x100] = {0};
	//GetSystemDirectory(path,0x100);
	//strcat(path,"\\SoftwareWhiteList");

	strcat(path,"SoftwareWhiteList");

	DWORD dwTmp = 0;

	HANDLE file = CreateFile(path,GENERIC_READ,0,0,OPEN_ALWAYS,0,0);
	if(file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	int nsize = ::GetFileSize(file,&dwTmp);

	BYTE *pData = new BYTE[nsize];

	ReadFile(file,pData,nsize,&dwTmp,0);
	CloseHandle(file);

	char szTmp[0x100] = {0};
	int nStartPos = 0;
	int nEndPos = 0;
	g_nCountOfWhiteList = 0;

	while(nEndPos<nsize)
	{
		if((pData[nEndPos] == '\r')&&(pData[nEndPos+1] == '\n'))
		{
			memset(szTmp,0,0x100);
			memcpy(szTmp,&pData[nStartPos],nEndPos - nStartPos);
			UpperString(szTmp);
			g_arrStrWiteListExeName[g_nCountOfWhiteList] = szTmp;
			g_nCountOfWhiteList++;
			nStartPos = nEndPos + 2;
			nEndPos = nStartPos + 1;
		}
		else
		{
			nEndPos++;
		}
	}

	delete pData;

	MyAddOriginalProcessToWhiteList();

	return true;
}

BOOL MyDeleteRecordToWhiteList(CString pattern)
{
	for(int i=0;i<g_nCountOfWhiteList;i++)
	{
		if(g_arrStrWiteListExeName[i] == pattern)
		{
			g_arrStrWiteListExeName[i] = "";
		}
	}
	return true;
}

BOOL MyAddRecordToWhiteList(CString pattern)
{
	UpperString(g_strExeName.GetBuffer());
	g_arrStrWiteListExeName[g_nCountOfWhiteList] = pattern;
	g_nCountOfWhiteList++;

	return true;
}

BOOL MyFindRecordInWhiteList(CString pattern)
{
	for(int i=0;i<g_nCountOfWhiteList;i++)
	{
		if(g_arrStrWiteListExeName[i] == pattern)
		{
			return true;
		}
	}
	return false;
}

void MyAddOriginalProcessToWhiteList()
{
	HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return;  

	pe32.dwSize = sizeof(PROCESSENTRY32); 
 
    if (Process32First(hProcessSnap, &pe32)) 
    { 
        do 
        { 
			UpperString(pe32.szExeFile);
			bRet = MyFindRecordInWhiteList(pe32.szExeFile);
			if(!bRet)
			{
				MyAddRecordToWhiteList(pe32.szExeFile);
			}
        } 
        while (Process32Next(hProcessSnap, &pe32));  
    } 
    
    CloseHandle (hProcessSnap); 
	return ;
}
BOOL MySaveSoftwareWhiteList()
{
	char path[0x100] = {0};
	//GetSystemDirectory(path,0x100);
	//strcat(path,"\\SoftwareWhiteList");

	strcat(path,"SoftwareWhiteList");

	char szSplit[3] = "\r\n";

	DWORD dwTmp = 0;

	HANDLE file = CreateFile(path,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);

	for(int i=0;i<g_nCountOfWhiteList;i++)
	{
		if(g_arrStrWiteListExeName[i] != "")
		{
			WriteFile(file,g_arrStrWiteListExeName[i].GetBuffer(),g_arrStrWiteListExeName[i].GetLength(),&dwTmp,0);
			WriteFile(file,szSplit,2,&dwTmp,0);
		}
	}
	CloseHandle(file);

	return true;
}

int g_nSoftwareInstall_BlockingStatus = 0; // 0:ready , 1: waiting server response
DWORD g_dwSuspendedProcessId = 0;

BOOL SuspendResumeProcess(DWORD dwOwnerPID, bool bResumeThread) 
{ 
    HANDLE        hThreadSnap = NULL; 
    BOOL          bRet        = FALSE; 
    THREADENTRY32 te32        = {0}; 
 
    // Take a snapshot of all threads currently in the system. 

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    // Fill in the size of the structure before using it. 

    te32.dwSize = sizeof(THREADENTRY32); 
 
    // Walk the thread snapshot to find all threads of the process. 
    // If the thread belongs to the process, add its information 
    // to the display list.
 
    if (Thread32First(hThreadSnap, &te32)) 
    { 
        do 
        { 
            if (te32.th32OwnerProcessID == dwOwnerPID) 
            {
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
				if (bResumeThread)
				{					
					ResumeThread(hThread);
					
				}
				else
				{					
					SuspendThread(hThread);
					
				}
				CloseHandle(hThread);
            } 
        }
        while (Thread32Next(hThreadSnap, &te32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;          // could not walk the list of threads 
 
    // Do not forget to clean up the snapshot object. 
    CloseHandle (hThreadSnap); 
 
    return (bRet); 
} 
BOOL MyResumeProcess()
{
	BOOL res = SuspendResumeProcess(g_dwSuspendedProcessId,true);
	g_dwSuspendedProcessId = 0;
	g_nSoftwareInstall_BlockingStatus = 0;

	MyAddRecordToWhiteList(g_strExeName);
	
	MySaveSoftwareWhiteList();

	g_strExeName = "";
	return res;
}
BOOL MyTerminateProcess()
{
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS ,0,g_dwSuspendedProcessId);
	if(hProc == 0)
	{
		return false;
	}
	TerminateProcess(hProc,1);
	CloseHandle(hProc);
	g_dwSuspendedProcessId = 0;
	g_nSoftwareInstall_BlockingStatus = 0;
}

BOOL CheckSoftwareInstall(TCHAR *szFileName , TCHAR * szTitle , TCHAR *szInfo)
{
	/*
	CString strPattern = _T("Setup");
	CString strTitle = szTitle;

	int pos = strTitle.Find(strPattern);
	if( pos > -1)
		return true;
	else
		return false;
		*/

	UpperString(szFileName);

	CString szTmp = szFileName;
	if(szTmp == "")
		return true;

	for(int i=0;i<g_nCountOfWhiteList;i++)
	{
		
		if(g_arrStrWiteListExeName[i] == szTmp)
		{
			return true;
		}
	}

	return false;

}

void ProcMonitoringSoftwareInstallThread(LPVOID param)
{
	/*
	HWND hDlg = (HWND)param;

	TCHAR strTitle[0x100];
	TCHAR strProcessPath[0x100];
	TCHAR strExeName[0x100] = {0};

	DWORD dwProcessId;
	DWORD dwError = 0;

	COPYDATASTRUCT cds;
	
	DWORD dwCurrentId = 0;

	EnableDebugPriv();

	OutputDebugString("Start Monitoring");

	BOOL res = MyLoadSoftwareWhiteList();
	if(res)
	{
	}
	else
	{

	}

	MySaveSoftwareWhiteList();

	while (true)
	{
		Sleep(1000);

		
		HWND hwnd = ::GetForegroundWindow();

		::GetWindowText(hwnd,strTitle,100);
		::GetWindowThreadProcessId(hwnd,&dwProcessId);

		dwCurrentId = GetCurrentProcessId();

		if(dwCurrentId == dwProcessId)
			continue;
		if(g_dwSuspendedProcessId == dwProcessId)
			continue;
		

		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS ,0,dwProcessId);
		if(hProc == 0)
		{
			dwError = GetLastError();

			sprintf(g_szDebugString," error %d : %d ",dwProcessId,dwError);
			OutputDebugString(g_szDebugString);
			continue;
		}

		//GetModuleFileNameEx(hProc,0,strProcessPath,0x100);
		psapi_GetModuleFileNameEx(hProc,0,strProcessPath,0x100);
		
		sprintf(g_szDebugString," path : %s\r\n",strProcessPath);
		OutputDebugString(g_szDebugString);
		

		GetExeName(strProcessPath,strExeName);

		BOOL res = CheckSoftwareInstall(strExeName,strTitle,g_szInstalledSoftwareInformation);
		
		if(!res)
		{
			FillVersionData(strProcessPath);

			sprintf(g_szMessageForSoftwareInstall,_T(" Application name : %s \r\n WindowTitle : %s \r\n information : %s"),strExeName,strTitle,g_szInstalledSoftwareInformation);

			if( g_nSoftwareInstall_BlockingStatus == 1) 
			{
				//current waiting for server response, so process should be shutdowned.
				TerminateProcess(hProc,1);
				CloseHandle(hProc);
			}
			else
			{
				sprintf(g_szDebugString," path : %s\r\n",strProcessPath);
				OutputDebugString(g_szDebugString);

				CloseHandle(hProc);

				g_dwSuspendedProcessId = dwProcessId;
				g_strExeName = strExeName;
				SuspendResumeProcess(dwProcessId,false);// suspend
				g_nSoftwareInstall_BlockingStatus = 1;
				
				cds.dwData = CLIENT_NOTIFICATION_INSTALL_SOFTWARE_ATTEMPT;
				cds.cbData = strlen(g_szMessageForSoftwareInstall);
				cds.lpData = g_szMessageForSoftwareInstall;
				SendMessage((HWND)hDlg,WM_COPYDATA,0,(LPARAM)&cds);		
				
				//HANDLE hEvent = CreateEvent(NULL,TRUE,FALSE,"MySoftewareInstall");
				//WaitForSingleObject(hEvent,INFINITE);
				//CloseHandle(hEvent);
				
			}
			
		}
	}
	*/
}

/////////////////////////////////////////  for logic

BOOL MyIsCredentialAlready()
{
	char path[0x100] = {0};
	GetSystemDirectory(path,0x100);
	strcat(path,"\\MachineGUID");

	HANDLE file = CreateFile(path,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
	if(file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DWORD nLen = 0;
	ReadFile(file,g_MachineGUID,0x200,&nLen,0);
	CloseHandle(file);

	return true;
}
BOOL MySaveMachineGUID(BYTE *strGUID,int len)
{
	char path[0x100] = {0};
	GetSystemDirectory(path,0x100);
	strcat(path,"\\MachineGUID");

	HANDLE file = CreateFile(path,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	if(file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DWORD nLen = 0;
	WriteFile(file,strGUID,len,&nLen,0);
	CloseHandle(file);

	return true;
}

BOOL MySaveRamSerial(BYTE *strSerial,int len)
{
	char path[0x100] = {0};
	GetSystemDirectory(path,0x100);
	strcat(path,"\\RAMSerial");

	HANDLE file = CreateFile(path,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	if(file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DWORD nLen = 0;
	WriteFile(file,strSerial,len,&nLen,0);
	CloseHandle(file);

	return true;
}

int MyLoadRamSerial(BYTE *strSerial,int len)
{
	char path[0x100] = {0};
	GetSystemDirectory(path,0x100);
	strcat(path,"\\RAMSerial");

	HANDLE file = CreateFile(path,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
	if(file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD nLen = 0;
	ReadFile(file,strSerial,len,&nLen,0);
	CloseHandle(file);

	return nLen;
}

////////////////////////////////////////   Get Hardware information

int MyGetHardwareSummery_GetProcessor(char *strOut)
{
	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	ULONG uCount = 1, uReturned;

	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	///////////////////////////////////////////////////////get processor
	
	strTmp.Format("Select * from Win32_Processor");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		OutputDebugString("MyGetHardwareSummery_GetProcessor:Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		OutputDebugString("MyGetHardwareSummery_GetProcessor:Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"Name");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		_bstr_t bstrPath = &vtProp;
		char* strPath=(char*)bstrPath;
		VariantClear( &vtProp );
		strcat(strOut,strPath);		
	}

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();
	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return strlen(strOut);
}

int MyGetHardwareSummery_GetTotalMemory(char *strOut)
{
	char strTmp[0x10] = {0};

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	ULONG MemorySize = (ULONG)((statex.ullTotalPhys)/(1024*1024));

	ltoa(MemorySize,strTmp,10);
	strcat(strOut,strTmp);
	strcat(strOut,"MByte");

	return strlen(strOut);
}

int MyGetHardwareSummery_Get_Total_and_UsedHardDisk(char *strTotal, char*strUsed)
{
	//return len of strTotal, so I should get len of serUsed after exit here.

	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	char strTmp64[64];

	ULONG uCount = 1, uReturned;


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	//////////////////////////////////////////   Get TotalHardDisk

	__int64 lTotalSize = 0;
	__int64 lFreeSize = 0;
	
	strTmp.Format("Select * from Win32_LogicalDisk");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"Size");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		_bstr_t bstrPath = &vtProp;
		char* strPath=(char*)bstrPath;
		
		lTotalSize = lTotalSize + _atoi64(strPath);
		VariantClear( &vtProp );
		VariantInit( &vtProp ); 
			
		strClassProp = SysAllocString(L"FreeSpace");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		bstrPath = &vtProp;
		strPath=(char*)bstrPath;

		lFreeSize = lFreeSize + _atoi64(strPath);
		VariantClear( &vtProp );
		
	}

	lTotalSize = lTotalSize / 1024 / 1024;

	ltoa(lTotalSize,strTmp64,10);	
	strcat(strTotal,strTmp64);
	strcat(strTotal,"MByte");

	lFreeSize = lFreeSize / 1024 / 1024;
	lFreeSize = lTotalSize - lFreeSize;
	ltoa(lFreeSize,strTmp64,10);	
	strcat(strUsed,strTmp64);
	strcat(strUsed,"MByte");
	

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return strlen(strTotal);

}

int MyGetHardwareSummery_GetTotalHardDisk(char *strOut)
{
	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	char strTmp64[64];

	ULONG uCount = 1, uReturned;


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	//////////////////////////////////////////   Get TotalHardDisk

	__int64 lTotalSize = 0;
	__int64 lFreeSize = 0;
	
	strTmp.Format("Select * from Win32_LogicalDisk");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"Size");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		_bstr_t bstrPath = &vtProp;
		char* strPath=(char*)bstrPath;
		
		lTotalSize = lTotalSize + _atoi64(strPath);
		VariantClear( &vtProp );
		VariantInit( &vtProp ); 
			
		strClassProp = SysAllocString(L"FreeSpace");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		bstrPath = &vtProp;
		strPath=(char*)bstrPath;

		lFreeSize = lFreeSize + _atoi64(strPath);
		VariantClear( &vtProp );
		
	}

	lTotalSize = lTotalSize / 1024 / 1024;

	ltoa(lTotalSize,strTmp64,10);
	strcat(strOut,"TotalHardDisk,");
	strcat(strOut,strTmp64);
	strcat(strOut,"MByte\r\n");

	lFreeSize = lFreeSize / 1024 / 1024;
	lFreeSize = lTotalSize - lFreeSize;
	ltoa(lFreeSize,strTmp64,10);
	strcat(strOut,"UsedHardDisk,");
	strcat(strOut,strTmp64);
	strcat(strOut,"MByte\r\n");
	

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return strlen(strOut);

}

int MyGetHardwareSummery_GetOperatingSystem(char *strOut)
{
	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	ULONG uCount = 1, uReturned;


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	//////////////////////////////  GetOperatingSystem
	
	strTmp.Format("Select * from Win32_OperatingSystem");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"Caption");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		_bstr_t bstrPath = &vtProp;
		char* strPath=(char*)bstrPath;
		
		strcat(strOut,strPath);
	
		VariantClear( &vtProp );
		VariantInit( &vtProp ); 
					
	}

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return strlen(strOut);

}

int MyGetHardwareSummery_GetHDSerial(char *strOut)
{

	CString strVlaueOfArray;
	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	


	HRESULT hRes;
	BSTR strQuery = (L"Select * from Win32_PhysicalMedia");
	BSTR bstrQL = (L"WQL");
	hRes = pWbemServices->ExecQuery(bstrQL, strQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	ULONG uCount = 1, uReturned;
	IWbemClassObject * pClassObject = NULL;


	//hRes = pEnumObject->Reset();

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		OutputDebugString("-------------------------------------------------------------------ok1");

		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			OutputDebugString("-------------------------------------------------------------------ok2");
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
		pClassObject->BeginEnumeration( WBEM_FLAG_NONSYSTEM_ONLY ); 
		{ 
			CIMTYPE pvtType; 
			VARIANT     pVal; 
			BSTR        pstrName;  
			
 
			//get name
			VARIANT v;
			
			BSTR strClassProp = SysAllocString(L"SerialNumber");
			hRes = pClassObject->Get(strClassProp, 0, &v, 0, 0);
			if(hRes != S_OK)
			{
				//AfxMessageBox("Could not Get Value");
				OutputDebugString("-------------------------------------------------------------------ok3");
				return 0;
			}
			SysFreeString(strClassProp);

			if((v.vt == VT_NULL)||(v.vt == VT_EMPTY))
			{
				//continue;
			}
			else
			{
				_bstr_t bstrPath = &v;
				char* strPath=(char*)bstrPath;
				VariantClear( &v );
			
				strcat(strOut,strPath);
				strcat(strOut,"   ");
			
				OutputDebugString(strPath);
			}

		} 
		
	}

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();
	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	return strlen(strOut);

}

int MyGetHardwareSummery_GetCPUSerial(char *strOut)
{
	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	ULONG uCount = 1, uReturned;


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	///////////////////////////////////////// get CPUSerial

	strTmp.Format("Select * from Win32_Processor");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"ProcessorId");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
		{
			strcat(strOut,"NULL");
		}
		else
		{
			_bstr_t bstrPath = &vtProp;
			char* strPath=(char*)bstrPath;		
			strcat(strOut,strPath);
		}

		VariantClear( &vtProp );
		VariantInit( &vtProp ); 
					
	}

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return strlen(strOut);
}

int MyGetHardwareSummery_GetMacAddress(char *strOut)
{
	//return len of out string

	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	ULONG uCount = 1, uReturned;


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	////////////////////////////////// Get MacAddress  and IP Address
	strTmp.Format("Select * from Win32_NetworkAdapterConfiguration");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"MACAddress");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		_bstr_t bstrPath = &vtProp;
		char* strPath=(char*)bstrPath;
		strcat(strOut,strPath);
		VariantClear( &vtProp );
		break;			
	}

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return strlen(strOut);
}

int MyGetHardwareSummery_GetMachineName(char *strOut)
{
	int len = 0;
	CString strTmp;
	CString strVlaueOfArray;
	HRESULT hRes;
	BSTR bstrQuery;
	BSTR bstrQL = (L"WQL");

	ULONG uCount = 1, uReturned;


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	IWbemClassObject * pClassObject = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	
	//////////////////////////////  GetOperatingSystem
	
	strTmp.Format("Select * from Win32_ComputerSystem");
	bstrQuery = strTmp.AllocSysString();
	
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
			
		BSTR strClassProp = SysAllocString(L"Name");
		hRes = pClassObject->Get(strClassProp, 0, &vtProp, 0, 0);
		if(hRes != S_OK)
		{
			//AfxMessageBox("Could not Get Value");
			return 0;
		}
		SysFreeString(strClassProp);

		if((vtProp.vt == VT_NULL)||(vtProp.vt == VT_EMPTY))
			continue;

		_bstr_t bstrPath = &vtProp;
		char* strPath=(char*)bstrPath;
		
		strcat(strOut,strPath);
		len = strlen(strOut);
	
		VariantClear( &vtProp );
		VariantInit( &vtProp ); 
					
	}

	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();

	SysFreeString(bstrQuery);

	return len;
}

///////////////////////////////////////////////////////////////	Get Software Information

int MyGetServiceList(char *strOut)
{
	int res = 0;

	MyGetHardwareInfo_PreProcess();
	MyGetWMIInformationBySQL("Select Caption,Name,PathName,ServiceType,StartMode,StartName,State,Status from Win32_Service  ",strOut);
	MyGetHardwareInfo_PostProcess();

	return res;
}
int MyGetAutoStartedProcessList(char *strOut)
{
	int res = 0;

	MyGetHardwareInfo_PreProcess();
	MyGetWMIInformationBySQL("Select Caption,Command,Location,User from Win32_StartupCommand  ",strOut);
	MyGetHardwareInfo_PostProcess();

	return res;
}
int MyGetInstalledSoftwareList(char *strOut)
{
	int res = 0;

	MyGetHardwareInfo_PreProcess();
	MyGetWMIInformationBySQL("SELECT Caption, Description, InstallDate, packageName,Vendor,Version FROM Win32_Product",strOut);
	MyGetHardwareInfo_PostProcess();

	return res;
}

#define	IS_KEY		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define	IS_KEY_LEN	256
#define IS_DISPLAY				_T("DisplayName")
#define IS_INSTALL_DATE			_T("InstallDate")
#define IS_INSTALL_LOCATION		_T("InstallLocation")
#define IS_DISPLAY_VERSION		_T("DisplayVersion")
#define IS_PUBLISHER			_T("Publisher")

int MyGetInstalledSoftwareList_FromRegister(char *strOut)
{
	// Build a list of installed applications by enumerating
	//	HKLM\Software\Microsoft\Windows\CurrentVersion\Uninstall
	//	and fetching "DisplayName" entry

	HKEY hKey;
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, IS_KEY, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return 0;

	DWORD dwIndex = 0;
	LONG lRet;
	DWORD cbName = IS_KEY_LEN;
	TCHAR szSubKeyName[IS_KEY_LEN];
		
	while ((lRet = ::RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL,
		NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// Do we have a key to open?
		if (lRet == ERROR_SUCCESS)
		{
			// Open the key and get the value
			HKEY hItem;
			if (::RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ, &hItem) != ERROR_SUCCESS)
				continue;
			// Opened - look for "DisplayName"
			TCHAR szDisplayName[IS_KEY_LEN];
			DWORD dwSize = sizeof(szDisplayName);
			DWORD dwSize1 = dwSize;
			DWORD dwType;

			dwSize1 = dwSize;
			if (::RegQueryValueEx(hItem, IS_DISPLAY, NULL, &dwType,
				(LPBYTE)&szDisplayName, &dwSize1) == ERROR_SUCCESS)
			{
				strcat(strOut,"******");
				strcat(strOut,szDisplayName);
				strcat(strOut,"\r\n");		
			}
			else
			{
				DWORD dwError = GetLastError();
				continue;
			}

			dwSize1 = dwSize;
			if (::RegQueryValueEx(hItem, IS_INSTALL_DATE, NULL, &dwType,
				(LPBYTE)&szDisplayName, &dwSize1) == ERROR_SUCCESS)
			{
				strcat(strOut,"InstallDate,");
				strcat(strOut,szDisplayName);
				strcat(strOut,"\r\n");		
			}
			/*
			if (::RegQueryValueEx(hItem, IS_INSTALL_LOCATION, NULL, &dwType,
				(LPBYTE)&szDisplayName, &dwSize1) == ERROR_SUCCESS)
			{
				strcat(strOut,"InstallLocation,");
				strcat(strOut,szDisplayName);
				strcat(strOut,"\r\n");		
			}
			*/
			dwSize1 = dwSize;
			if (::RegQueryValueEx(hItem, IS_DISPLAY_VERSION, NULL, &dwType,
				(LPBYTE)&szDisplayName, &dwSize1) == ERROR_SUCCESS)
			{
				strcat(strOut,"DisplayVersion,");
				strcat(strOut,szDisplayName);
				strcat(strOut,"\r\n");		
			}

			dwSize1 = dwSize;
			if (::RegQueryValueEx(hItem, IS_PUBLISHER, NULL, &dwType,
				(LPBYTE)&szDisplayName, &dwSize1) == ERROR_SUCCESS)
			{
				strcat(strOut,"Publisher,");
				strcat(strOut,szDisplayName);
				strcat(strOut,"\r\n");		
			}

			::RegCloseKey(hItem);
		}
		dwIndex++;
		cbName = IS_KEY_LEN;
	}
	::RegCloseKey(hKey);
	return 1;
}

int MyGetMachineCredential(char *strOut)
{
	char strTmp[0x200] = {0};
	char strCredential[0x200] = {0};
	MyGetHardwareSummery_GetProcessor(strTmp);
	MyGetHardwareSummery_GetHDSerial(strTmp);
	MyGetHardwareSummery_GetMacAddress(strTmp);
	MyGetHardwareSummery_GetTotalMemory(strTmp);
	MyTrim(strTmp,strCredential,strlen(strTmp));
	strcat(strOut,strCredential);

	return 1;
}
int MyGetWMIInformationBySQL(char *sql,char* strOut)
{
	//CString strTmp = "Select Caption, ClassGuid, CompatibleID, Description, DeviceID, PNPDeviceID,  Manufacturer, Name, HardwareID from Win32_PnPEntity Where NOT DeviceID LIKE '\%ROOT\%' AND NOT DeviceID LIKE '\%STORAGE\%'";
	CString strTmp = sql;
	CString strVlaueOfArray;


	BSTR bstrQuery = strTmp.AllocSysString();


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	


	HRESULT hRes;
	//BSTR strQuery = (L"Select * from Win32_DiskPartition");
	BSTR bstrQL = (L"WQL");
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	ULONG uCount = 1, uReturned;
	IWbemClassObject * pClassObject = NULL;


	//hRes = pEnumObject->Reset();

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		OutputDebugString("-------------------------------------------------------------------ok1");

		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			OutputDebugString("-------------------------------------------------------------------ok2");
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
		pClassObject->BeginEnumeration( WBEM_FLAG_NONSYSTEM_ONLY ); 
		{ 
			CIMTYPE pvtType; 
			VARIANT     pVal; 
			BSTR        pstrName;  
			
 
			//get name
			VARIANT v;
		
			BSTR strClassProp = SysAllocString(L"Caption");
			hRes = pClassObject->Get(strClassProp, 0, &v, 0, 0);
			if(hRes != S_OK)
			{
				//AfxMessageBox("Could not Get Value");
				OutputDebugString("-------------------------------------------------------------------ok3");
				continue;
			}
			SysFreeString(strClassProp);

			if((v.vt == VT_NULL)||(v.vt == VT_EMPTY))
			{
				//continue;
			}
			else
			{

				_bstr_t bstrPath = &v;
				char* strPath=(char*)bstrPath;
				VariantClear( &v );
				strcat(strOut,"******");
				strcat(strOut,strPath);
				strcat(strOut,"\r\n");
			
				OutputDebugString(strPath);
			}

			//get properties
			while( pClassObject->Next( 0, &pstrName, &pVal, &pvtType, NULL ) == WBEM_S_NO_ERROR )  
			{ 
				if((pVal.vt == VT_NULL)||(pVal.vt == VT_EMPTY))
					continue;

				//for property name
				char *p= _com_util::ConvertBSTRToString(pstrName);  

				if(strcmp(p,"Caption") == 0)
				{
					continue;
				}
				strcat(strOut,p);
				strcat(strOut,",");
				delete[] p;

				//for property value
				if(pVal.vt & VT_ARRAY)
				{
					DWORD dwBytes = 0;
					SAFEARRAY *pArrayVal = NULL;
					pArrayVal = pVal.parray;
					dwBytes = pArrayVal->rgsabound[0].cElements;
					int * pArr = (int*)(pArrayVal->pvData);
					for(int i=0;i<dwBytes;i++)
					{
						strVlaueOfArray.Format(_T("%d"),pArr[i]);
						strcat(strOut,strVlaueOfArray.GetBuffer());
						strcat(strOut," ");
					}
					strcat(strOut,"\r\n");
					continue;	
				}
				
				_bstr_t bstrVal = &pVal;
				char* strVal=(char*)bstrVal;
				strcat(strOut,strVal);
				strcat(strOut,"\r\n");

				if(strlen(strOut)>0xFFF00)
				{
					OutputDebugString("-------------------------------------------------------------------ok5");
					return 1;
				}

				SysFreeString( pstrName ); 
				VariantClear( &pVal ); 
			} 
		} 
		
	}

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();
	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();


	SysFreeString(bstrQuery);

	return 1;
}


BYTE MyGetClassIdFromClassGuid(char* guid)
{
	CString str_guid = CString(guid);
	for (BYTE i=0;i<100;i++)
	{
		if(str_guid.Compare(g_strClassGuid[i]) == 0)
			return i;
	}
	return 0;
}


int MyGetWMIDeviceInformationBySQL(char *sql,BYTE* bufOut , DWORD &dwHardwareCheckSum)
{
	//sql : INPUT , bufOut : OUTPUT, dwHardwareCheckSum : OUTPUT

	//CString strTmp = "Select Caption, ClassGuid, CompatibleID, Description, DeviceID, PNPDeviceID,  Manufacturer, Name, HardwareID from Win32_PnPEntity Where NOT DeviceID LIKE '\%ROOT\%' AND NOT DeviceID LIKE '\%STORAGE\%'";
	CString strTmp = sql;
	CString strVlaueOfArray;

	char strOut[0x100] = "";
	int pos = 0;
	int count = 0;
	dwHardwareCheckSum = 0;

	BSTR bstrQuery = strTmp.AllocSysString();


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	


	HRESULT hRes;
	//BSTR strQuery = (L"Select * from Win32_DiskPartition");
	BSTR bstrQL = (L"WQL");
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	ULONG uCount = 1, uReturned;
	IWbemClassObject * pClassObject = NULL;


	//hRes = pEnumObject->Reset();

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 
		OutputDebugString("-------------------------------------------------------------------ok1");

		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			OutputDebugString("-------------------------------------------------------------------ok2");
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
		pClassObject->BeginEnumeration( WBEM_FLAG_NONSYSTEM_ONLY ); 
		{ 
			CIMTYPE pvtType; 
			VARIANT     pVal; 
			BSTR        pstrName;  

			//get properties
			while( pClassObject->Next( 0, &pstrName, &pVal, &pvtType, NULL ) == WBEM_S_NO_ERROR )  
			{ 
				if((pVal.vt == VT_NULL)||(pVal.vt == VT_EMPTY))
					continue;

				//for property name
				char *p= _com_util::ConvertBSTRToString(pstrName);  
				/*
				//for property value
				if(pVal.vt & VT_ARRAY)
				{
					DWORD dwBytes = 0;
					SAFEARRAY *pArrayVal = NULL;
					pArrayVal = pVal.parray;
					dwBytes = pArrayVal->rgsabound[0].cElements;
					int * pArr = (int*)(pArrayVal->pvData);
					for(int i=0;i<dwBytes;i++)
					{
						strVlaueOfArray.Format(_T("%d"),pArr[i]);
						strcat(strOut,strVlaueOfArray.GetBuffer());
						strcat(strOut," ");
					}
					strcat(strOut,"\r\n");
					continue;	
				}
				*/
				_bstr_t bstrVal = &pVal;
				char* strVal=(char*)bstrVal;

				if(strcmp(p,"ClassGuid") == 0)
				{
					BYTE cid = MyGetClassIdFromClassGuid(strVal);
					memcpy(&bufOut[pos],&cid,1);
					pos++;
				}
				else //process device id
				{
					
					DWORD dwVen = 0;
					DWORD dwDev = 0;

					CString strRes = strVal;
					int nVenPos = strRes.Find("VEN_");
					int nDevPos = strRes.Find("DEV_");
					if(nVenPos != -1)
					{
						CString ven = strRes.Mid(nVenPos+4,4);
						ConvertStringToHex(ven.GetBuffer(),(char*)&dwVen);
					}
					
					if(nDevPos != -1)
					{
						CString dev = strRes.Mid(nDevPos+4,4);					
						ConvertStringToHex(dev.GetBuffer(),(char*)&dwDev);
					}					
					
					memcpy(&bufOut[pos],&dwVen,4);
					pos = pos + 4;
					memcpy(&bufOut[pos],&dwDev,4);
					pos = pos + 4;
					count++;
					dwHardwareCheckSum = dwHardwareCheckSum + (dwVen<<16) + dwDev;
				}

				delete[] p;

				SysFreeString( pstrName ); 
				VariantClear( &pVal ); 
			} 
		} 
		
	}

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();
	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();


	SysFreeString(bstrQuery);

	return count;
}

int MyGetRAMSerial(char *strOut)
{
	MyGetHardwareInfo_PreProcess();

	
	CString sql = "Select SerialNumber from Win32_PhysicalMemory";
	CString strTmp = sql;
	CString strVlaueOfArray;


	BSTR bstrQuery = strTmp.AllocSysString();


	IWbemLocator * pIWbemLocator = NULL;
	IWbemServices * pWbemServices = NULL;
	IEnumWbemClassObject * pEnumObject  = NULL;

	BSTR bstrNamespace = (L"root\\cimv2");


	if(CoCreateInstance (
            CLSID_WbemAdministrativeLocator,
            NULL ,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER , 
            IID_IUnknown ,
            ( void ** ) & pIWbemLocator
            ) != S_OK)
				return 0;

	if(pIWbemLocator->ConnectServer(
                bstrNamespace,  // Namespace
                NULL,          // Userid
                NULL,           // PW
                NULL,           // Locale
                0,              // flags
                NULL,           // Authority
                NULL,           // Context
                &pWbemServices
                ) != S_OK)
				return 0;
	


	HRESULT hRes;
	BSTR bstrQL = (L"WQL");
	hRes = pWbemServices->ExecQuery(bstrQL, bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumObject);
	
	if(hRes != S_OK)
	{
		AfxMessageBox("Could not execute Query");
		return 0;
	}

	ULONG uCount = 1, uReturned;
	IWbemClassObject * pClassObject = NULL;

	if(hRes != S_OK)
	{
		AfxMessageBox("Could not Enumerate");
		return 0;
	}

	while( pEnumObject )  
	{ 

		HRESULT hr = pEnumObject->Next( WBEM_INFINITE, 1, &pClassObject, &uReturned ); 
		if( !uReturned )  
		{ 
			break; 
		} 
 
		VARIANT vtProp; 
		VariantInit( &vtProp ); 
		pClassObject->BeginEnumeration( WBEM_FLAG_NONSYSTEM_ONLY ); 
		{ 
			CIMTYPE pvtType; 
			VARIANT     pVal; 
			BSTR        pstrName;  
			
			//get properties
			while( pClassObject->Next( 0, &pstrName, &pVal, &pvtType, NULL ) == WBEM_S_NO_ERROR )  
			{ 
				if((pVal.vt == VT_NULL)||(pVal.vt == VT_EMPTY))
					continue;
			
				_bstr_t bstrVal = &pVal;
				char* strVal=(char*)bstrVal;
				strcat(strOut,strVal);
				strcat(strOut,",");

				if(strlen(strOut)>0xFFF00)
				{					
					return 1;
				}

				SysFreeString( pstrName ); 
				VariantClear( &pVal ); 
			} 
		} 
		
	}

	if(pIWbemLocator != NULL)
		pIWbemLocator->Release();
	if(pWbemServices != NULL)
		pWbemServices->Release();
	if(pEnumObject != NULL)
		pEnumObject->Release();
	if(pClassObject!=NULL)
		pClassObject->Release();


	SysFreeString(bstrQuery);

	MyGetHardwareInfo_PostProcess();

	return 1;
}

int MyGetAllHardwareInformation_Sub(BYTE* bufOut, DWORD &dwHardwareCheckSum)
{
	int nCount = 0;
	MyGetHardwareInfo_PreProcess();	
	nCount = MyGetWMIDeviceInformationBySQL("Select ClassGuid,DeviceID from Win32_PnPEntity Where NOT DeviceID LIKE '\%ROOT\%' AND NOT DeviceID LIKE '\%STORAGE\%' AND DeviceID LIKE '\%VEN\%'",bufOut,dwHardwareCheckSum);
	MyGetHardwareInfo_PostProcess();
	return nCount;
}

void MyGetHardwareInfo_PreProcess()
{
	CoInitialize(NULL);


	//Security needs to be initialized in XP first and this was the major problem 
	//why it was not working in XP.

	if(CoInitializeSecurity( NULL,
	-1,
	NULL,
	NULL,
	RPC_C_AUTHN_LEVEL_PKT,
	RPC_C_IMP_LEVEL_IMPERSONATE,
	NULL,
	EOAC_NONE,
	0
	) != S_OK)
		return ;

}
void MyGetHardwareInfo_PostProcess()
{
	CoUninitialize();

}

////////////////////////////////////////   For making Packet

int MyMakePacket_Notification_Register(BYTE * pMachine_credential,int nLen, char* local_ip)
{
	int nCommand = CLIENT_NOTIFICATION_REGISTER;
	int nCount = 2;
	int len = 0;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);
	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);
	memcpy(&g_MainPacketBuf[0x08],&nLen,4);
	memcpy(&g_MainPacketBuf[0x0C],pMachine_credential,nLen);

	len = nLen + 0x0C;

	int nIpLen = strlen(local_ip);
	memcpy(&g_MainPacketBuf[len],&nIpLen,4);

	len = len + 4;

	memcpy(&g_MainPacketBuf[len],local_ip,nIpLen);

	return len + nIpLen;
}

int MyMakePacket_Notification_Login(BYTE * pMachine_GUID,int nLen, char* local_ip)
{
	int nCommand = CLIENT_NOTIFICATION_LOGIN;
	int nCount = 2;
	int len = 0;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);
	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);
	memcpy(&g_MainPacketBuf[0x08],&nLen,4);
	memcpy(&g_MainPacketBuf[0x0C],pMachine_GUID,nLen);

	len = nLen + 0x0C;

	int nIpLen = strlen(local_ip);
	memcpy(&g_MainPacketBuf[len],&nIpLen,4);

	len = len + 4;

	memcpy(&g_MainPacketBuf[len],local_ip,nIpLen);

	return len + nIpLen;
}

int MyMakePacket_Client_Response_Machine_Data(BYTE * pMachine_GUID,int nGuidLen , BYTE* bufCommandId , char *local_ip)
{

	int nCommand = CLIENT_RESPONSE_MACHINE_DATA;
	int nCount = 2;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);
	memset(g_MainInfoBuf,0,MAX_PACKET_BUFFER_LEN);

	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);
	memcpy(&g_MainPacketBuf[0x08],bufCommandId,8);	

	char strMachineInfo_tmp[0x100] = "";

	int nTmpLen = nGuidLen;	
	int pos = 0x10;	
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);  //guid
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],pMachine_GUID,nTmpLen);
	pos = pos + nTmpLen;

	MyGetHardwareInfo_PreProcess();

	nTmpLen = MyGetHardwareSummery_GetMachineName(strMachineInfo_tmp); //machine name
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	nTmpLen = strlen(local_ip); //machine ip
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)local_ip,nTmpLen);
	pos = pos + nTmpLen;

	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_GetProcessor(strMachineInfo_tmp); //processor
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_GetTotalMemory(strMachineInfo_tmp); //Total memory
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	char strUsedHardDisk[0x100] = "";
	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_Get_Total_and_UsedHardDisk(strMachineInfo_tmp,strUsedHardDisk); //Total harddisk and used harddisk
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;
	nTmpLen = strlen(strUsedHardDisk);		
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strUsedHardDisk,nTmpLen);
	pos = pos + nTmpLen;

	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_GetOperatingSystem(strMachineInfo_tmp); //Operating system
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_GetHDSerial(strMachineInfo_tmp); //Hard serial
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_GetCPUSerial(strMachineInfo_tmp); //CPU serial
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	memset(strMachineInfo_tmp,0,0x100);
	nTmpLen = MyGetHardwareSummery_GetMacAddress(strMachineInfo_tmp); //MAC address
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4);
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)strMachineInfo_tmp,nTmpLen);
	pos = pos + nTmpLen;

	MyGetHardwareInfo_PostProcess();

	DWORD dwHardWareCheckSum = 0;
	int nDeviceCount = MyGetAllHardwareInformation_Sub((BYTE*)g_MainInfoBuf,dwHardWareCheckSum);		
	

	nTmpLen = 4;
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4); //hardware checksum
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],&dwHardWareCheckSum,nTmpLen);
	pos = pos + nTmpLen;

	nTmpLen = strlen(g_MachineCredential);
	memcpy(&g_MainPacketBuf[pos],&nTmpLen,4); //machine credential
	pos = pos + 4;
	memcpy(&g_MainPacketBuf[pos],g_MachineCredential,nTmpLen);
	pos = pos + nTmpLen;

	memcpy(&g_MainPacketBuf[pos],&nDeviceCount,4);  //device data list
	pos = pos + 4;
	int nLen = nDeviceCount * 0x09;
	memcpy(&g_MainPacketBuf[pos],(BYTE*)g_MainInfoBuf,nLen);
	pos = pos + nLen;

/*	
	DWORD dwLen1 = 0;
	HANDLE file = CreateFile("info.zip",GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	WriteFile(file,g_MainPacketBuf,dwLen+0x0C,&dwLen1,0);
	CloseHandle(file);
*/
	return pos;
}

int MyMakePacket_Client_Response_AuthKey_Fail(BYTE *bufCommandId)
{
	int nCommand = CLIENT_RESPONSE_AUTH_KEY_FAIL;
	int nCount = 0;
	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);
	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);
	memcpy(&g_MainPacketBuf[0x08],bufCommandId,8);
	return 0x10;
}

int MyMakePacket_Client_Notification_Software_Run(char *MachineGUID,char *softwareName , char *exeFile, char *fileVersion , char*publisher,char* sha1)
{
	int nCommand = CLIENT_NOTIFICATION_SOFTWARE_RUNNING;
	int nCount = 6;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);

	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);

	int nPos = 0x08;

	int nGUIDLen = strlen(MachineGUID);
	memcpy(&g_MainPacketBuf[nPos],&nGUIDLen,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],MachineGUID,nGUIDLen);
	nPos = nPos + nGUIDLen;

	int nLen2 = strlen(softwareName);
	memcpy(&g_MainPacketBuf[nPos],&nLen2,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],softwareName,nLen2);
	nPos = nPos + nLen2;

	nLen2 = strlen(exeFile);
	memcpy(&g_MainPacketBuf[nPos],&nLen2,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],exeFile,nLen2);
	nPos = nPos + nLen2;

	nLen2 = strlen(fileVersion);
	memcpy(&g_MainPacketBuf[nPos],&nLen2,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],fileVersion,nLen2);
	nPos = nPos + nLen2;

	nLen2 = strlen(publisher);
	memcpy(&g_MainPacketBuf[nPos],&nLen2,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],publisher,nLen2);
	nPos = nPos + nLen2;

	nLen2 = strlen(sha1);
	memcpy(&g_MainPacketBuf[nPos],&nLen2,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],sha1,nLen2);
	nPos = nPos + nLen2;

	return nPos;
}

int MyMakePacket_Client_Notification_Keyboard_Event(char *MachineGUID,char *strEvent)
{
	int nCommand = CLIENT_NOTIFICATION_KEYBOARD_EVENT;
	int nCount = 2;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);

	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);

	int nPos = 0x08;

	int nGUIDLen = strlen(MachineGUID);
	memcpy(&g_MainPacketBuf[nPos],&nGUIDLen,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],MachineGUID,nGUIDLen);
	nPos = nPos + nGUIDLen;

	int nLen2 = strlen(strEvent);
	memcpy(&g_MainPacketBuf[nPos],&nLen2,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],strEvent,nLen2);
	nPos = nPos + nLen2;

	return nPos;
}

int MyMakePacket_Client_Notification_Keylogger_detect(char *MachineGUID)
{
	int nCommand = CLIENT_NOTIFICATION_KEYLOGGER_DETECTED;
	int nCount = 1;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);

	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);

	int nPos = 0x08;

	int nGUIDLen = strlen(MachineGUID);
	memcpy(&g_MainPacketBuf[nPos],&nGUIDLen,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],MachineGUID,nGUIDLen);
	nPos = nPos + nGUIDLen;

	return nPos;
}

int MyMakePacket_Client_Notification_RAM_Serial_Change(char *MachineGUID , char* new_ramSerial)
{
	int nCommand = CLIENT_NOTIFICATION_MEMORY_SERIAL_CHANGED;
	int nCount = 2;

	memset(g_MainPacketBuf,0,MAX_PACKET_BUFFER_LEN);

	memcpy(&g_MainPacketBuf[0x00],&nCommand,4);
	memcpy(&g_MainPacketBuf[0x04],&nCount,4);

	int nPos = 0x08;

	int nGUIDLen = strlen(MachineGUID);
	memcpy(&g_MainPacketBuf[nPos],&nGUIDLen,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],MachineGUID,nGUIDLen);
	nPos = nPos + nGUIDLen;

	int new_ramSerial_len = strlen(new_ramSerial);
	memcpy(&g_MainPacketBuf[nPos],&new_ramSerial_len,4);
	nPos = nPos + 4;
	memcpy(&g_MainPacketBuf[nPos],new_ramSerial,new_ramSerial_len);
	nPos = nPos + new_ramSerial_len;

	return nPos;
}

/////////////////////////////////////////////////////////  ChengNans Function
CController controller;
int MyBlockHardwareInstall(char * strResult)
{
	// call ChengNans function
	//controller.LockHardware();

	strcpy(strResult,"SUCCESS; Machine hardware locked");
	return strlen(strResult);
}
int MyUnBlockHardwareInstall(char * strResult)
{
	// call ChengNans function
	//controller.UnlockHardware();
	strcpy(strResult,"SUCCESS; Machine hardware unlocked");
	return strlen(strResult);
}
int MyUnlockHardwareId(char *strVendorId,char *strDeviceId,char * strResult)
{
	// call ChengNans function

	strcpy(strResult,"is not implemented , SUCCESS; Hardware  VendorID, DeviceID unlocked.");
	return strlen(strResult);
}
int MyUnlockHardwareType(char *strType,char * strResult)
{
	// call ChengNans function
	for(int i=0;i<0x100;i++)
	{
		if(strcmp(strType,g_nHardwareTypeNo[i]) == 0)
		{
			//controller.UnlockHardwareByType(i);
			strcpy(strResult,"SUCCESS; Hardware  Type unlocked.");
			return strlen(strResult);
		}
	}

	strcpy(strResult,"Fails; Unknown device type.");
	return strlen(strResult);
	
}
int MyUnlockNetworkMAC(char *strMac,char * strResult)
{
	// call ChengNans function

	strcpy(strResult,"is not implemented , SUCCESS; Network Mac unlocked.");
	return strlen(strResult);
}
int MyCheckUnlockedHardware(char * strResult)
{
	// call ChengNans function

	strcpy(strResult,"No exceptions found.");
	return strlen(strResult);
}
int MyUnInstall(char * strResult)
{
	// call ChengNans function

	strcpy(strResult,"SUCCESS; Uninstall in progress.");
	return strlen(strResult);
}



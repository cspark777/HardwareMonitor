
#include "ioctl.h" 
#include <stdio.h>

#define STATUS_SUCCESS	((NTSTATUS)0x00000000L)

typedef LONG	NTSTATUS;
typedef struct _STRING {
	USHORT  Length;
	USHORT  MaximumLength;
	PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;

typedef struct _UNICODE_STRING {
	USHORT  Length;
	USHORT  MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

HANDLE DriverLoad(char *szDrvName, char *szFileName);
BOOL DriverUnload(HANDLE hDrvDev, char *szDrvName);

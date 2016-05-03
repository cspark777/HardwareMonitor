 /***************************************************************************** 
 * 
 *    Copyright (c) 2002 DEVGURU. 
 * 
 *  All rights reserved. DEVGURU source code is an unpublished 
 *  work and the use of a copyright notice does not imply otherwise. 
 *  This source code contains confidential, trade secret material of 
 *  DEVGURU. Any attempt or participation in deciphering, 
 *  decoding, reverse engineering or in any way altering the source code is 
 *  strictly prohibited, unless the prior written consent of 
 *  DEVGURU is obtained. 
 * 
 * 
 * File Name :  testApp.CPP 
 * 
 * Written by:   DEVGURU  
 * Date:     7/19/2002 
 * 
 * Description : Driver �� ReadFile(), WriteFile() API�� �̿��� ����ϴ� ����  
 *               
 ***************************************************************************** 
 * Revision History 
 * 
 * Rev.  Date        Who        Description 
 * 0.01  7/19/2002      DEVGURU      ó������ �ۼ��� 
 *                                   Driver �� ���  
 *****************************************************************************/ 
 
#include "stdafx.h" 
#include "windows.h" 
#include "stdio.h" 
#include "winioctl.h" 
#include "conio.h" 
#include "Tlhelp32.h"
#include "Driver.h"


int main(int argc, char* argv[]) 
{ 
	char a = '\0';
	HANDLE  handle; 
	DWORD dwBytes;
	ULONG expid[2] = {0};

	// Driver�� Open �Ѵ�. 
	
	handle = CreateFileA( "\\\\.\\BDSYS",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL
        );
	if(handle == INVALID_HANDLE_VALUE)
		handle = DriverLoad("BDSYS", "BDSYS.SYS");
	if ( INVALID_HANDLE_VALUE != handle ) 
	{ 
		DeviceIoControl(handle, 
			IOCTL_BLOCK_ALL_DEVICE_INST, 
			NULL, 
			0, 
			NULL, 
			0, 
			&dwBytes, 
			NULL);
		printf("Blocked!\r\n");
		getchar();
		getchar();
		DeviceIoControl(handle, 
			IOCTL_UNBLOCK_ALL_DEVICE_INST, 
			NULL, 
			0, 
			NULL, 
			0, 
			&dwBytes, 
			NULL);
		printf("Unblocked!\r\n");
		getchar();
		getchar();
		// Driver Open �� ������ ��� 
		DriverUnload(handle, "BDSYS");
		return 0; 
	} 
	
	
	return 0; 
} 

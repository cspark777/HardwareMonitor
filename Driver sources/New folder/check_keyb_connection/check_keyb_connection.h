
// check_keyb_connection.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ccheck_keyb_connectionApp:
// See check_keyb_connection.cpp for the implementation of this class
//

class Ccheck_keyb_connectionApp : public CWinApp
{
public:
	Ccheck_keyb_connectionApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ccheck_keyb_connectionApp theApp;
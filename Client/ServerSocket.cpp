// ServerSocket.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerSocket.h"

#include "ClientSocketDlg.h"
#include "SocketManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WSA_VERSION  MAKEWORD(2,0)
#define MAX_HOSTNAME 256
#define MAX_HOSTADDR 40


/////////////////////////////////////////////////////////////////////////////
const int SOCK_TCP1	= 0;
const int SOCK_UDP  = 1;

int g_nIsConnect = 0;


/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp

BEGIN_MESSAGE_MAP(CServerSocketApp, CWinApp)
	//{{AFX_MSG_MAP(CServerSocketApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp construction

CServerSocketApp::CServerSocketApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nLinkMode = 0; // server
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerSocketApp object

CServerSocketApp theApp;

DWORD ThreadMainProc (LPVOID lpdwThreadParam ) 
{
	CClientSocketDlg dlg2;
	CClientSocketDlg* pMainWnd = &dlg2;
	//int nResponse = ((CDialog*)pMainWnd)->Create(IDD_CLIENTSOCKET_DIALOG);
	//pMainWnd->ShowWindow(0);

	int nResponse = ((CDialog*)pMainWnd)->DoModal();

	HANDLE hEvent = CreateEvent(NULL,TRUE,FALSE,"myeve1");
	//WaitForSingleObject(hEvent,INFINITE);
	SetEvent(hEvent);

	return 1;
}
/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp initialization

BOOL CServerSocketApp::InitInstance()
{
	WSADATA		WSAData = { 0 };
	if ( 0 != WSAStartup( WSA_VERSION, &WSAData ) )
	{
		// Tell the user that we could not find a usable
		// WinSock DLL.
		if ( LOBYTE( WSAData.wVersion ) != LOBYTE(WSA_VERSION) ||
			 HIBYTE( WSAData.wVersion ) != HIBYTE(WSA_VERSION) )
			 ::MessageBox(NULL, _T("Incorrect version of WS2_32.dll found"), _T("Error"), MB_OK);

		WSACleanup( );
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1200
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Load Icons for Tray 
	m_pIconList[0] = LoadIcon (MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIconList[1] = LoadIcon (MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIconList[2] = LoadIcon (MAKEINTRESOURCE(IDR_MAINFRAME));

	ParseCommandLineArgs();

	DWORD dwThreadId;
	if (::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&ThreadMainProc,(LPVOID)this,0,&dwThreadId ) == NULL)
	{
		
	}

	HANDLE hEvent = CreateEvent(NULL,TRUE,FALSE,"myeve1");
	WaitForSingleObject(hEvent,INFINITE);
/*
	CClientSocketDlg dlg2;

	m_pMainWnd = &dlg2; // Client


	int nResponse = ((CDialog*)m_pMainWnd)->Create(IDD_CLIENTSOCKET_DIALOG);
	m_pMainWnd->ShowWindow(0);
	
//	int nResponse = ((CDialog*)m_pMainWnd)->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

//	HANDLE hEvent = CreateEvent(NULL,TRUE,FALSE,"myeve1");
//	WaitForSingleObject(hEvent,INFINITE);
*/
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	
	return FALSE;
}

int CServerSocketApp::ExitInstance() 
{
	// Terminate use of the WS2_32.DLL
	WSACleanup();
	
	return CWinApp::ExitInstance();
}

void CServerSocketApp::ParseCommandLineArgs()
{
	CString	strCmdLine = m_lpCmdLine;
	if (!strCmdLine.IsEmpty())
	{
		strCmdLine.MakeUpper();
		while( !strCmdLine.IsEmpty() )
		{
			CString	strCurrent = strCmdLine;
			int	nNextPos = strCmdLine.Find(TCHAR(' '));
			if (nNextPos > 0)
			{
				strCurrent = strCmdLine.Left( nNextPos );
				strCmdLine.Delete(0, nNextPos+1);
			}
			else
			{
				strCurrent = strCmdLine;
				strCmdLine = _T("");
			}

			if (strCurrent == _T("/SERVER")	|| strCurrent == _T("/S") )
				m_nLinkMode	= 0;
			else if	(strCurrent	== _T("/CLIENT") ||	strCurrent == _T("/C") )
				m_nLinkMode	= 1;

		}

	}
}



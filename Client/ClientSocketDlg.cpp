// ClientSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>
#include "ServerSocket.h"
#include "ClientSocketDlg.h"
#include "Setup.h"
#include "Controller.h"
#include "MyPacketList.h"
#include "SocketManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern char g_MachineCredential[0x200];
extern char g_MachineGUID[0x200];
extern char g_AuthKey[0x200];
extern char g_RamSerial[0x100];


extern BYTE g_MainPacketBuf[MAX_PACKET_BUFFER_LEN];
extern char g_MainInfoBuf[MAX_PACKET_BUFFER_LEN];
extern CController controller;

extern char g_chrLocalIp[0x100];

const int SOCK_TCP	= 0;
const int SOCK_UDP  = 1;

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg dialog

#define				COMMAND_GET_HARDINFOS	"GET_HARDINFO\r\n"	

extern int g_nIsConnect;
extern CString g_arrStrWiteListExeName[0x400];
extern int g_nCountOfWhiteList;

UINT  ClientThread(LPVOID pParam);
UINT  ListenFromServerThread(LPVOID pParam);
UINT  SendingThread(LPVOID pParam);

int g_ListenThreadIsRunned = 0;
CString g_strServerIp;
CString g_strPort;
int g_nSoftwareSniffStatue = 1; // 0: stop state , 1: start state

CMyPacketList g_PacketList;   // packet queue
CSocketManager g_SocketManager; //socket manager

CClientSocketDlg::CClientSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientSocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientSocketDlg)
	m_nSockType = SOCK_TCP;	// default TCP
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CClientSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientSocketDlg)
	DDX_Control(pDX, IDC_TXT_MESSAGE, m_ctlMessage);
	DDX_Control(pDX, IDC_SVR_PORTINC, m_ctlPortInc);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_ctlMsgList);
	DDX_Control(pDX, IDC_IPADDRESS, m_ctlIPAddress);
	DDX_Radio(pDX, IDC_TCP, m_nSockType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SVR_PORT, m_ctlPort);
}


BEGIN_MESSAGE_MAP(CClientSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CClientSocketDlg)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDOK, &CClientSocketDlg::OnBnClickedOk)

	//for tray
	ON_MESSAGE(WM_TRAY_NOTIFY, OnTrayNotify)
	ON_COMMAND(ID_DETECT_SHOW, OnTrayRestore)
	

	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CClientSocketDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg message handlers

BOOL CClientSocketDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_ESCAPE)
			return TRUE;

		if (nVirtKey == VK_RETURN && (GetFocus()->m_hWnd  == m_ctlMessage.m_hWnd))
		{
			if (g_SocketManager.IsOpen())
				OnBtnSend();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

int CClientSocketDlg::MySendPacket_Wrapper(int message,BYTE *buf,int nPacketLen)
{
	DWORD nLen = 0;
	char strName[10] = {0};
	sprintf(strName,"%d",message);
	HANDLE file = CreateFile(strName,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	WriteFile(file,buf,nPacketLen,&nLen,0);
	CloseHandle(file);

	return SendMyPacket(buf,nPacketLen);
}
int CClientSocketDlg::SendMyPacket(BYTE *buf,int nPacketLen)
{
	/*
	
	int nRet = -1;
	bool bSuccess = g_SocketManager.ConnectTo( g_strServerIp, g_strPort, AF_INET, SOCK_STREAM); // TCP
	if(bSuccess)
	{
		nRet = g_SocketManager.WriteComm( buf, nPacketLen, INFINITE);
		g_SocketManager.StopComm();
	}
	return nRet;
	*/

	g_PacketList.AddPacket(buf,nPacketLen);

	return nPacketLen;
}

BOOL CClientSocketDlg::OnInitDialog() 
{
	ASSERT( GetDlgItem(IDC_BTN_SEND) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_START) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_STOP) != NULL );
	ASSERT( GetDlgItem(IDC_TCP) != NULL );
	ASSERT( GetDlgItem(IDC_UDP) != NULL );

	CDialog::OnInitDialog();

	char szStr[256];
	MyBlockHardwareInstall(szStr);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


	g_SocketManager.SetDlgPointer(this);

	m_ctlPortInc.SetRange32( 2000, 4500);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow( FALSE );
	GetDlgItem(IDC_BTN_STOP)->EnableWindow( FALSE );

	memset(g_chrLocalIp,0,0x100);
	g_SocketManager.GetLocalAddress( g_chrLocalIp, 0x100);
	
	//m_ctlIPAddress.SetWindowText( strLocal );
	m_ctlIPAddress.SetWindowText( "192.168.90.191" );
	m_ctlPort.SetWindowText("45000");

	// Initialize socket manager
	g_SocketManager.SetMessageWindow( &m_ctlMsgList );
	g_SocketManager.SetServerState( false );	// run as client
	g_SocketManager.SetSmartAddressing( false );	// always send to server
	 
	m_bIsVisible = true;

	MyInitProcess();

	AfxBeginThread(ListenFromServerThread,(LPVOID)this);	

	AfxBeginThread(SendingThread,(LPVOID)this);

	//MyInitProcess();

	Load_PsApi();

	OnHide();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClientSocketDlg::SendPacket(LPBYTE pData, DWORD dwCount)
{
	g_SocketManager.WriteComm( pData, dwCount, INFINITE);
}

void CClientSocketDlg::DisplayMessage(char *message)
{
	
	HWND hWnd = m_ctlMsgList.GetSafeHwnd();
	DWORD dwResult = 0;
	if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
	{
		int nLen = (int) dwResult;
		if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
		{
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)message, SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}

	}
}

void CClientSocketDlg::OnBtnConnect() 
{
	m_ctlIPAddress.GetWindowText(g_strServerIp);
	m_ctlPort.GetWindowText(g_strPort);

	MyLoadRamSerial((BYTE*)g_RamSerial,0x100);

	bool bRes = MyIsCredentialAlready();
	if(!bRes)
		MySendRegisterPacket(g_MachineCredential,g_chrLocalIp);			
	else
		MySendLoginPacket(g_MachineGUID,g_chrLocalIp);

/*
	BYTE ss[0x40] = {
		0x01,0x04,0x00,0x00,0x02,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x41,0x41,0x41,0x41,
		0x2D,0x42,0x42,0x42,0x42,0x2D,0x43,0x43,0x43,0x43,0x2D,0x44,0x44,0x44,0x44,0x2D,
		0x45,0x45,0x45,0x45,0x18,0x00,0x00,0x00,0x31,0x31,0x31,0x31,0x2D,0x32,0x32,0x32,
		0x32,0x2D,0x33,0x33,0x33,0x33,0x2D,0x34,0x34,0x34,0x34,0x2D,0x35,0x35,0x35,0x35
	};
	ProcessPacket(ss,0x40);*/
}

void CClientSocketDlg::OnBtnDisconnect() 
{
	// Disconnect socket
	g_SocketManager.StopComm();
	if (!g_SocketManager.IsOpen())
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow( TRUE );
		PrevDlgCtrl();
		GetDlgItem(IDC_BTN_SEND)->EnableWindow( FALSE );
		GetDlgItem(IDC_BTN_STOP)->EnableWindow( FALSE );
		GetDlgItem(IDC_TCP)->EnableWindow( TRUE );
		GetDlgItem(IDC_UDP)->EnableWindow( TRUE );
	}
	g_nIsConnect = 0;
}

void CClientSocketDlg::OnBtnSend() 
{
	if(g_nIsConnect != 1) 
		return;

	BYTE byBuffer[256] = { 0 };
	CString strText;

	// Update so that we can retrieve new port number also... :-)
	UpdateData();
	m_ctlMessage.GetWindowText( strText );
	int nLen = strText.GetLength();

	if (nLen > 0)
	{
		strText += _T("\r\n");
		nLen = strText.GetLength() + 1;
		USES_CONVERSION;
		strcpy_s(reinterpret_cast<LPSTR>(byBuffer), 256, T2CA(strText));
		g_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
	}
}


void CClientSocketDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pTray)
	{
		delete m_pTray ;
		m_pTray = NULL ;
	} 

}

void CClientSocketDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if(!m_bIsVisible)
        lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialog::OnWindowPosChanging(lpwndpos);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CClientSocketDlg::MyInitProcess()
{
	MyGetHardwareInfo_PreProcess();
	MyGetMachineCredential(g_MachineCredential);
	MyGetHardwareInfo_PostProcess();

/*
	BOOL bRes;
	int nPacketLen = 0;

	if(g_nIsConnect)
	{
		bRes = MyIsCredentialAlready();
		if(bRes)
		{
			nPacketLen = strlen(g_MachineGUID);
			nPacketLen = MyMakePacket_Notification_Login((BYTE*)g_MachineGUID,nPacketLen);
			MySendPacket_Wrapper(27,g_MainPacketBuf,nPacketLen);
		}
		else
		{
			
			nPacketLen = strlen(g_MachineCredential);
			nPacketLen = MyMakePacket_Notification_Register((BYTE*)g_MachineCredential,nPacketLen);
			MySendPacket_Wrapper(25,g_MainPacketBuf,nPacketLen);
			
			//MySendRegisterPacket();
		}
	}
*/
	HWND hwnd = this->GetSafeHwnd();

	controller.m_hDlg = hwnd;

	DWORD dwThreadId;
	
	if (CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&ProcMonitoringSoftwareInstallThread,(LPVOID)hwnd,0,&dwThreadId ) == NULL)
	{
		AfxMessageBox("monitoring error");
		ExitProcess(1);
	}

}
void CClientSocketDlg::MyFinalProcess() 
{
}

void CClientSocketDlg::ProcessPacket(LPBYTE pData, DWORD dwCount)
{
	int nMessage;
	int nLen;
	CString strResult,strTmp;
	char szDebugString[0x100]={0};

	memcpy(&nMessage,pData,4);

	switch(nMessage)
	{
	case SERVER_RESPONSE_REGISTERED:
		{
			int nGuidLen = 0;
			int nKeyLen = 0;

			memset(g_MachineGUID,0,0x200);
			memset(g_AuthKey,0,0x200);
			memcpy(&nGuidLen,&pData[0x08],4);
			memcpy(g_MachineGUID,&pData[0x0C],nGuidLen);
			memcpy(&nKeyLen,&pData[0x0C+nGuidLen],4);
			memcpy(g_AuthKey,&pData[0x0C + nGuidLen + 0x04],nKeyLen);

			MySaveMachineGUID((BYTE*)g_MachineGUID,nGuidLen);

			strResult = "GUID : ";
			strResult = strResult + g_MachineGUID;
			strResult = strResult + "\r\nAuthKey : ";
			strResult = strResult + g_AuthKey;
			strResult = strResult + "\r\n";

			DisplayMessage(strResult.GetBuffer());

			//check ram serial change
			char str_ramSerial[0x100] = {0};
			MyGetRAMSerial(str_ramSerial);
			if( (strcmp(g_RamSerial,"")!=0) && (strcmp(str_ramSerial,g_RamSerial) != 0 ))
			{
				MySaveRamSerial((BYTE*)str_ramSerial,strlen(str_ramSerial));
				nLen = MyMakePacket_Client_Notification_RAM_Serial_Change(g_MachineGUID,str_ramSerial);

				MySendPacket_Wrapper(16,g_MainPacketBuf,nLen);

				DisplayMessage("send notification Ram serial change\r\n");
			}

			break;
		}
	case SERVER_RESPONSE_WELCOME:
		{
			int nKeyLen = 0;
			 
			memset(g_AuthKey,0,0x200);
			memcpy(&nKeyLen,&pData[0x08],4);
			memcpy(g_AuthKey,&pData[0x0C],nKeyLen);

			strResult = "AuthKey : ";   
			strResult = strResult + g_AuthKey;
			strResult = strResult + "\r\n";

			DisplayMessage(strResult.GetBuffer());
			
			//check ram serial change
			char str_ramSerial[0x100] = {0};
			MyGetRAMSerial(str_ramSerial);
			if( (strcmp(g_RamSerial,"")!=0) && (strcmp(str_ramSerial,g_RamSerial) != 0 ))
			{
				MySaveRamSerial((BYTE*)str_ramSerial,strlen(str_ramSerial));
				nLen = MyMakePacket_Client_Notification_RAM_Serial_Change(g_MachineGUID,str_ramSerial);

				MySendPacket_Wrapper(16,g_MainPacketBuf,nLen);

				DisplayMessage("send notification Ram serial change\r\n");
			}
			
			break;
		}
	case SERVER_REQUIRE_GET_HARDWARE_INFO:
		{
			BYTE bufCommandId[8];
			memset(bufCommandId,0,8);

			int nKeyLen = 0;
			char strAuthKey[0x200] = {0};
			
			memcpy(bufCommandId,&pData[0x8],8);
			memcpy(&nKeyLen,&pData[0x10],4);
			memcpy(strAuthKey,&pData[0x14],nKeyLen);

			if(strcmp(strAuthKey,g_AuthKey) != 0)
			{
				//process Fail
				nLen = MyMakePacket_Client_Response_AuthKey_Fail(bufCommandId);
				MySendPacket_Wrapper(11,g_MainPacketBuf,nLen);
				break;
			}

			//process send allhardware information
			nLen = MyMakePacket_Client_Response_Machine_Data((BYTE*)g_MachineGUID,strlen(g_MachineGUID),bufCommandId,g_chrLocalIp);

			MySendPacket_Wrapper(10,g_MainPacketBuf,nLen);

			DisplayMessage("send response_machine_data packet\r\n");

			break;
		}
	case SERVER_REQUIRE_CLIENT_START_SOFTWARE_SNIFF:
		{
			g_nSoftwareSniffStatue = 1;
			break;
		}
	case SERVER_REQUIRE_CLIENT_STOP_SOFTWARE_SNIFF:
		{
			g_nSoftwareSniffStatue = 0;
			break;
		}	
	}
	
}

void CClientSocketDlg::OnBnClickedOk()
{
	OnHideapp();
}

//for tray

void CClientSocketDlg::OnHide()
{
	m_pTray = new CTrayNot (this,WM_TRAY_NOTIFY,NULL,theApp.m_pIconList);
	m_pTray->SetState(IDR_MAINFRAME);
	m_bHidden = TRUE;
}

void CClientSocketDlg::OnUnHide()
{
  ShowWindow (SW_RESTORE) ;
  m_bHidden = FALSE ;
  /////////////////////////////////////
  // Remove icon from taskbar tray
  if (m_pTray)
  {
     delete m_pTray ;
     m_pTray = NULL ;
  }
}

LONG CClientSocketDlg::OnTrayNotify ( WPARAM wParam, LPARAM lParam )
{
  switch (lParam)
  {
    case WM_RBUTTONDOWN:
    {
       CMenu menu ;
       // Load and Verify Menu
       VERIFY(menu.LoadMenu(IDR_MENU_TRAY));
       CMenu* pPopup = menu.GetSubMenu (0) ;
       ASSERT(pPopup != NULL);

       // Get the cursor position
       POINT pt ;
       GetCursorPos (&pt) ;

       // Fix Microsofts' BUG!!!!
       SetForegroundWindow();

       ///////////////////////////////////
       // Display The Menu
       pPopup->TrackPopupMenu(TPM_LEFTALIGN |
       TPM_RIGHTBUTTON,pt.x, pt.y, AfxGetApp()->GetMainWnd() );
       break ;
    }
    case WM_LBUTTONDBLCLK:
       //////////////////////////////////
       // Unhide our Window
       if (m_bHidden)
          ShowWindow (SW_RESTORE);
       //OnUnHide() ;
       break ;
  }
  return (0) ;
}

void CClientSocketDlg::OnTrayRestore() 
{
  // UnHide Application
  if (m_bHidden)
    ShowWindow (SW_RESTORE) ;
  m_bHidden = FALSE ;
}

void CClientSocketDlg::OnHideapp() 
{
  //This will be the onclick for the hide button
  //in order to call that the app is minimised.
  m_bHidden = true;
  theApp.HideApplication();
}

TCHAR g_szMyDebugString1[0x100];

BOOL CClientSocketDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	TCHAR strTmp[0x100] = {0};

	DWORD dwMessage = pCopyDataStruct->dwData;
	DWORD dwLen = pCopyDataStruct->cbData;
	TCHAR *pData = (TCHAR*)pCopyDataStruct->lpData;
	int nPacketLen = 0;

	switch (dwMessage)
	{
	
	case CLIENT_NOTIFICATION_KEYBOARD_EVENT:
		{
			//AfxMessageBox(pData);

			if(g_MachineGUID[0] == 0) // machine guid is empty
				return true;

			memcpy(strTmp,pData,dwLen);

			OutputDebugString(strTmp);

			nPacketLen = MyMakePacket_Client_Notification_Keyboard_Event(g_MachineGUID,strTmp);
			MySendPacket_Wrapper(5,g_MainPacketBuf,nPacketLen); ///

			break;
		} 
	case CLIENT_NOTIFICATION_KEYLOGGER_DETECTED:
		{
			if(g_MachineGUID[0] == 0) // machine guid is empty
				return true;

			nPacketLen = MyMakePacket_Client_Notification_Keylogger_detect(g_MachineGUID);
			MySendPacket_Wrapper(6,g_MainPacketBuf,nPacketLen);

			break;
		}
	case CLIENT_NOTIFICATION_SOFTWARE_RUNNING:
		{
			if(g_MachineGUID[0] == 0) // machine guid is empty
				return true;

			nPacketLen = MyMakePacket_Client_Notification_Software_Run(g_MachineGUID);
			MySendPacket_Wrapper(17,g_MainPacketBuf,nPacketLen);

			break;
		}

	}
	return true;
}
//====================================================== Packet send methods ====================
void CClientSocketDlg::AddMessageToList(char * strMsg)
{
	DWORD dwResult = 0;
	HWND hWnd = m_ctlMsgList.GetSafeHwnd();
	if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
	{
		int nLen = (int) dwResult;
		if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
		{
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)strMsg, SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}

	}
}
void CClientSocketDlg::MySendRegisterPacket(char *machine_credential, char *local_ip)
{
	int nPacketLen = 0;
	int nRet = 0;
	
	char strMsg[0x40] = "";	
	
	nPacketLen = strlen(machine_credential);
	nPacketLen = MyMakePacket_Notification_Register((BYTE*)machine_credential,nPacketLen,local_ip);
	nRet = MySendPacket_Wrapper(1,g_MainPacketBuf,nPacketLen);	///
	if(nRet != -1)
	{
		sprintf(strMsg,"Register packet is successfully sended.\r\n");
	}
	else
	{
		sprintf(strMsg,"Sending login packet is failed.\r\n");
	}

	AddMessageToList(strMsg);

	
}
void CClientSocketDlg::MySendLoginPacket(char*machine_guid, char*ip)
{
	int nPacketLen = 0;
	int nRet = 0;
	
	char strMsg[0x40] = "";	

	nPacketLen = strlen(g_MachineGUID);
	nPacketLen = MyMakePacket_Notification_Login((BYTE*)g_MachineGUID,nPacketLen,ip);
	nRet = MySendPacket_Wrapper(3,g_MainPacketBuf,nPacketLen);	///
	if(nRet != -1)
	{
		sprintf(strMsg,"Login packet is successfully sended.\r\n");
	}
	else
	{
		sprintf(strMsg,"Sending login packet is failed.\r\n");
	}

	AddMessageToList(strMsg);	
}

void CClientSocketDlg::OnBnClickedBtnExit()
{
	CDialog::OnOK();
}
//====================================================== Listen thread ==========================

SOCKET server;

struct CLIENT_THREAD_PARAM
{
	SOCKET clientSocket;
	CClientSocketDlg *pDlg;
};

UINT  ListenFromServerThread(LPVOID pParam)
{		
	CClientSocketDlg *pDlg = (CClientSocketDlg*) pParam;

	sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short)8888);
	server=socket(AF_INET,SOCK_STREAM,0);
		
	if(server==INVALID_SOCKET)
	{
		g_ListenThreadIsRunned = 0;
		return 0;
	}
	if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
	{
		g_ListenThreadIsRunned = 0;
		return 0;
	}
	if(listen(server,10)!=0)
	{
		g_ListenThreadIsRunned = 0;
		return 0;
	}

	sockaddr_in from;
	int fromlen=sizeof(from);

	g_ListenThreadIsRunned = 1;

	while(true)
	{
		CLIENT_THREAD_PARAM param;
		param.clientSocket = accept(server,(struct sockaddr*)&from,&fromlen);
		param.pDlg = pDlg;
		AfxBeginThread(ClientThread,(LPVOID)&param);	
	}	

	g_ListenThreadIsRunned = 0;
	return 1;
}

UINT  ClientThread(LPVOID pParam)
{
	CLIENT_THREAD_PARAM *p = (CLIENT_THREAD_PARAM*) pParam;

	char buff[512];
	CString cmd;
	CString params;
	int n;

	BOOL auth=false;
	SOCKET client=p->clientSocket;

	//send(client,buff,strlen(buff),0);
	//while(true)
	{
		n=recv(client,buff,512,0);
		if(n==SOCKET_ERROR )			
			return 0;
		//buff[n]=0;		
		p->pDlg->ProcessPacket((BYTE*)buff,n);
	}		

	closesocket(client);
	return 0;
}

UINT  SendingThread(LPVOID pParam)
{
	CClientSocketDlg *p = (CClientSocketDlg*) pParam;

	while(true)
	{
		PACKET_NODE * node = NULL;
		node = g_PacketList.GetPacket();
		if(node != NULL)
		{
			int nRet = -1;
			bool bSuccess = g_SocketManager.ConnectTo( g_strServerIp, g_strPort, AF_INET, SOCK_STREAM); // TCP

			if(bSuccess)
			{
				nRet = g_SocketManager.WriteComm( node->buf, node->nLen, INFINITE);
				g_SocketManager.StopComm();
			}
			if(nRet > 0)
			{
				// success send,
				g_PacketList.PopPacket();
			}
			else
			{
				Sleep(2000); //if fail sending, wait 1min.
			}
		}		

		Sleep(200);
	}
	return 0;
}


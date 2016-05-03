
// check_keyb_connectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "check_keyb_connection.h"
#include "check_keyb_connectionDlg.h"
#include "afxdialogex.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccheck_keyb_connectionDlg dialog




Ccheck_keyb_connectionDlg::Ccheck_keyb_connectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ccheck_keyb_connectionDlg::IDD, pParent)
	, m_basevalue(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ccheck_keyb_connectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ccheck_keyb_connectionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Ccheck_keyb_connectionDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Ccheck_keyb_connectionDlg message handlers

BOOL Ccheck_keyb_connectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	LoadIODLL();
	bConnected = TRUE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ccheck_keyb_connectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ccheck_keyb_connectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ccheck_keyb_connectionDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_STATUS)->SetWindowText("Scanning now...");
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
	m_basevalue = s /2 * 3;

	SetTimer(1, 3000, NULL);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
}


void Ccheck_keyb_connectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	int c = 0;
	PortOut(0x60, 0x23);

	while(PortIn(0x60) != (char)0xFE)
	{
		c++;
	}
	//PortOut(0x60, 0xFF);
	PortOut(0x60, 0xEE);
	if ( c!=0 )
	{
		char szStatus[256];
		if(c > m_basevalue)
		{
			sprintf(szStatus, "Keyboard is disconnected (%d)", c);
		}else if(c <= m_basevalue)
		{
			sprintf(szStatus, "Keyboard is connected (%d)", c);
		}
		GetDlgItem(IDC_STATUS)->SetWindowTextA(szStatus);
	}
	CDialogEx::OnTimer(nIDEvent);
}

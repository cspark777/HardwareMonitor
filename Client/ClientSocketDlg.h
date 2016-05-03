#if !defined(AFX_CLIENTSOCKETDLG_H__014D90D9_161C_4561_A8DC_982D723B8F65__INCLUDED_)
#define AFX_CLIENTSOCKETDLG_H__014D90D9_161C_4561_A8DC_982D723B8F65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocketDlg.h : header file
//

#include "ProcessInterface.h"

#include "TrayNot.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg dialog

class CClientSocketDlg : public CDialog, ProcessInterface
{
// Construction
public:
	CClientSocketDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClientSocketDlg)
	enum { IDD = IDD_CLIENTSOCKET_DIALOG };
	CEdit	m_ctlMessage;
	CSpinButtonCtrl	m_ctlPortInc;
	CEdit	m_ctlMsgList;
	CIPAddressCtrl	m_ctlIPAddress;
	
	int		m_nSockType;

public:
	BOOL	m_bIsVisible;
	void SendPacket(LPBYTE pData, DWORD dwCount);
	virtual void ProcessPacket(LPBYTE pData,DWORD dwCount);

//for tray icon

public:
	CTrayNot* m_pTray;
	BOOL m_bHidden;

public:
	int SendMyPacket(BYTE *buf,int nPacketLen);
	void DisplayMessage(char *message);
	void MyInitProcess();
	void MyFinalProcess() ;

	int MySendPacket_Wrapper(int message,BYTE *buf,int nPacketLen);

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocketDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;	
	// Generated message map functions
	//{{AFX_MSG(CClientSocketDlg)
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnDisconnect();
	afx_msg void OnBtnSend();
	virtual BOOL OnInitDialog();
	

	//for tray
	afx_msg LONG OnTrayNotify ( WPARAM wParam, LPARAM lParam ) ;
	afx_msg void OnTrayRestore();
	afx_msg void OnHideapp();
	afx_msg void OnHide();
	afx_msg void OnUnHide();

	afx_msg void OnDestroy();
	

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);


	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

public:
	void MySendRegisterPacket(char *machine_credential, char *local_ip);
	void MySendLoginPacket(char*machine_guid, char*ip);
	void AddMessageToList(char * strMsg);
	afx_msg void OnBnClickedBtnExit();
	CEdit m_ctlPort;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKETDLG_H__014D90D9_161C_4561_A8DC_982D723B8F65__INCLUDED_)

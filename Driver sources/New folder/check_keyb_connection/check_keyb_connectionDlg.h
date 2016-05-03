
// check_keyb_connectionDlg.h : header file
//

#pragma once


// Ccheck_keyb_connectionDlg dialog
class Ccheck_keyb_connectionDlg : public CDialogEx
{
// Construction
public:
	Ccheck_keyb_connectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHECK_KEYB_CONNECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_basevalue;
	BOOL bConnected;
};

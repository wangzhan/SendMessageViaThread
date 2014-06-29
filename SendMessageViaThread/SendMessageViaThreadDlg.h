
// SendMessageViaThreadDlg.h : header file
//

#pragma once

enum MsgID
{
    SEND_MESSAGE1 = WM_USER + 2000,
};


// CSendMessageViaThreadDlg dialog
class CSendMessageViaThreadDlg : public CDialog
{
// Construction
public:
	CSendMessageViaThreadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SENDMESSAGEVIATHREAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButton1();
    LRESULT OnSend(WPARAM wParam, LPARAM lParam);

    static UINT __cdecl ThreadFunc(LPVOID pParam);

    afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	int InvokeSend();

	int SendMessageHandler(int i);
	void PostMessageHandler(int i);
};

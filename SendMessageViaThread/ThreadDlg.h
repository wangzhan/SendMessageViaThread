#pragma once


// CThreadDlg dialog

enum
{
    ACCEPT_MESSAGE1 = WM_USER + 1000,
};

class CThreadDlg : public CDialog
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThreadDlg();

    virtual BOOL OnInitDialog();
    void OnDestroy();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

    LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
    static HWND g_hwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

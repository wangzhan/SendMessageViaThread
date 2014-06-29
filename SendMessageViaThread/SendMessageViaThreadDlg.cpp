
// SendMessageViaThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SendMessageViaThread.h"
#include "SendMessageViaThreadDlg.h"
#include "ThreadDlg.h"
#include "TaskClosure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSendMessageViaThreadDlg dialog




CSendMessageViaThreadDlg::CSendMessageViaThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMessageViaThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendMessageViaThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSendMessageViaThreadDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDCANCEL, &CSendMessageViaThreadDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, &CSendMessageViaThreadDlg::OnBnClickedButton1)
    ON_MESSAGE(SEND_MESSAGE1, OnSend)
    ON_BN_CLICKED(IDC_BUTTON2, &CSendMessageViaThreadDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSendMessageViaThreadDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSendMessageViaThreadDlg message handlers

BOOL CSendMessageViaThreadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSendMessageViaThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSendMessageViaThreadDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSendMessageViaThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSendMessageViaThreadDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CSendMessageViaThreadDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    AfxBeginThread(ThreadFunc, this);
}

UINT CSendMessageViaThreadDlg::ThreadFunc(LPVOID pParam)
{
    //CThreadDlg *pDlg = new CThreadDlg();
    //pDlg->Create(IDD_DIALOG1);
    //pDlg->ShowWindow(SW_SHOW);

    CThreadDlg dlg(CWnd::GetDesktopWindow());
    dlg.DoModal();

    return 0;
}

LRESULT CSendMessageViaThreadDlg::OnSend(WPARAM wParam, LPARAM lParam)
{
    MessageBox(_T("Main thread accetp msg;"));
    return 0;
}

void CSendMessageViaThreadDlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    if (CThreadDlg::g_hwnd == NULL)
    {
        MessageBox(_T(" Please begin thread firstly;"));
        return;
    }

    //::SendMessage(CThreadDlg::g_hwnd, ACCEPT_MESSAGE1, 0, LPARAM(GetSafeHwnd()));
	//::SendMessageTimeout(CThreadDlg::g_hwnd, ACCEPT_MESSAGE1, 0, LPARAM(GetSafeHwnd()),
	//	SMTO_BLOCK | SMTO_NOTIMEOUTIFNOTHUNG, 0, 0);
	//::SendMessageTimeout(CThreadDlg::g_hwnd, ACCEPT_MESSAGE1, 0, LPARAM(GetSafeHwnd()),
	//	SMTO_NOTIMEOUTIFNOTHUNG, 0, 0);
	::SendNotifyMessage(CThreadDlg::g_hwnd, ACCEPT_MESSAGE1, 0, LPARAM(GetSafeHwnd()));
}


void CSendMessageViaThreadDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	int j = InvokeSend();
	PostTask(CThreadDlg::g_hwnd, &CSendMessageViaThreadDlg::PostMessageHandler, j);
}

int CSendMessageViaThreadDlg::InvokeSend()
{
	int i = 1;
	SendTaskAndReturn4(CThreadDlg::g_hwnd, &CSendMessageViaThreadDlg::SendMessageHandler, i);
	return 0;
}

int CSendMessageViaThreadDlg::SendMessageHandler(int i)
{
	//MessageBox(L"SendMessageHandler");
	return i;
}

void CSendMessageViaThreadDlg::PostMessageHandler(int i)
{
	//MessageBox(L"PostMessageHandler");
}

// ThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SendMessageViaThread.h"
#include "ThreadDlg.h"
#include "SendMessageViaThreadDlg.h"
#include "TaskClosure.h"


// CThreadDlg dialog

HWND CThreadDlg::g_hwnd = NULL;

IMPLEMENT_DYNAMIC(CThreadDlg, CDialog)

CThreadDlg::CThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreadDlg::IDD, pParent)
{

}

CThreadDlg::~CThreadDlg()
{
    g_hwnd = NULL;
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialog)
    ON_MESSAGE(ACCEPT_MESSAGE1, OnAccept)
	ON_MESSAGE(WM_POST_TASK_CLOSURE, OnPost)
	ON_MESSAGE(WM_SEND_TASK_CLOSURE, OnSend)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CThreadDlg message handlers
LRESULT CThreadDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
    MessageBox(_T("Work thread accept msg;"));
    HWND h = (HWND)lParam;
    ::SendMessage(h, SEND_MESSAGE1, 0, 0);
    //::PostMessage(h, SEND_MESSAGE1, 0, 0);
    //Sleep(1000 * 5);
    return 0;
}

BOOL CThreadDlg::OnInitDialog()
{
    g_hwnd = GetSafeHwnd();
    return __super::OnInitDialog();
}

void CThreadDlg::OnDestroy()
{
    //delete this;
    __super::OnDestroy();
}

LRESULT CThreadDlg::OnPost(WPARAM wParam, LPARAM lParam)
{
	PostedTask *pTask = (PostedTask*)lParam;
	(*pTask)();
	delete pTask;
	return 0;
}

LRESULT CThreadDlg::OnSend(WPARAM wParam, LPARAM lParam)
{
	SentTask *pTask = (SentTask*)lParam;
	(*pTask)();
	return 0;
}

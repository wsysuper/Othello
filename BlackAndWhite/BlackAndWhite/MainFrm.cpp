// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "BlackAndWhite.h"

#include "MainFrm.h"

#include "afxdlgs.h"
#include "PlaySound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame
int CMainFrame::chess = 64;
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
    //ON_MESSAGE(WM_RECV, &OnRecv)
//    ON_WM_NCPAINT()
ON_COMMAND(ID_SET, &CMainFrame::OnSet)
ON_COMMAND(ID_Create, &CMainFrame::OnCreate)
ON_COMMAND(ID_Join, &CMainFrame::OnJoin)
ON_COMMAND(ID_MUS, &CMainFrame::OnMus)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}
    HICON m_hIcon;
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(m_hIcon, TRUE); // Set big icon
    //SetIcon(m_hIcon, FALSE); // Set small icon
	/*if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
    m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(20);
    */
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
    cs.style&=~WS_MAXIMIZEBOX;
    cs.style&=~WS_SIZEBOX;
    cs.cx = 3*::GetSystemMetrics(SM_CXSIZEFRAME)+3+799;
    cs.cy = 3*::GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYMENU)+610;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//void CMainFrame::OnNcPaint()
//{
//    //const   MSG*   pMsg   =   GetCurrentMessage();   
//    //DefWindowProc(WM_NCPAINT,   pMsg->wParam,   pMsg->lParam); 
//    CWindowDC dc(this);
//    dc.LineTo (200,200);
//    CRect rwnd,rtit;
//    GetWindowRect(&rwnd);
//    rtit.left = 0;
//    rtit.top = 0;
//    rtit.right = rwnd.right;
//    rtit.bottom =  rtit.top   +   GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYFRAME); 
//    CBrush cbr;
//    cbr.CreateStockObject(GRAY_BRUSH);
//    dc.FillRect(&rtit,&cbr);
//    // TODO: 在此处添加消息处理程序代码
//    // 不为绘图消息调用 CFrameWnd::OnNcPaint()
//}

void CMainFrame::OnSet()
{
    // TODO: 在此添加命令处理程序代码
    CDialog setDlg(IDD_SET);
    setDlg.DoModal();
    
}

void CMainFrame::OnCreate()
{
    // TODO: 在此添加命令处理程序代码
    CDialog setDlg(IDD_Create);
    setDlg.DoModal();
}

void CMainFrame::OnJoin()
{
    // TODO: 在此添加命令处理程序代码
    CDialog setDlg(IDD_JOIN);
    setDlg.DoModal();
}

void CMainFrame::OnMus()
{
	// TODO: Add your command handler code here
	CFileDialog   filedialog(TRUE,NULL, _T(" "),OFN_HIDEREADONLY, _T("Wav Files Mp3 Files (*.wav;*.mp3)|*.wav;*.mp3|All Files (*.*)|*.*||"),NULL); 
	int flag = filedialog.DoModal(); 
	CString cstrFilePath = filedialog.GetPathName(); 
	CString str1 = _T("wav");
	CString str2 = _T("mp3");
	CString str3 = _T("WAV");
	CString str4 = _T("MP3"); 
	if(flag == IDOK) {
		if((cstrFilePath.Right(3) != str1)&&(cstrFilePath.Right(3) != str2)&&(cstrFilePath.Right(3) != str3)&&(cstrFilePath.Right(3) != str4)) 
		{ 
			CDialog setDlg(IDD_ERROR);
			setDlg.DoModal();
			return;
		}
	}
	else {
		return;
	}
	play_mp3(cstrFilePath);
}

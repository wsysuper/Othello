// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
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
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
    m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(20);
    */
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
    cs.style&=~WS_MAXIMIZEBOX;
    cs.style&=~WS_SIZEBOX;
    cs.cx = 3*::GetSystemMetrics(SM_CXSIZEFRAME)+3+799;
    cs.cy = 3*::GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYMENU)+610;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
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
//    // TODO: �ڴ˴������Ϣ����������
//    // ��Ϊ��ͼ��Ϣ���� CFrameWnd::OnNcPaint()
//}

void CMainFrame::OnSet()
{
    // TODO: �ڴ���������������
    CDialog setDlg(IDD_SET);
    setDlg.DoModal();
    
}

void CMainFrame::OnCreate()
{
    // TODO: �ڴ���������������
    CDialog setDlg(IDD_Create);
    setDlg.DoModal();
}

void CMainFrame::OnJoin()
{
    // TODO: �ڴ���������������
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

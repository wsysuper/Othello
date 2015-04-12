// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "Game.h"
#include "BlackAndWhite.h"
#include "ChildView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
HWND CChildView::hwnd = 0;
int CChildView::dif = 0;
CChildView::CChildView()
: m_hdc(NULL)
, chess(CMainFrame::chess)
{
    pGC = new GameControl();
    pGC->chessboard->pwnd = this;
    wchess.LoadBitmap(IDB_WCHESS);
    bchess.LoadBitmap(IDB_BCHESS);
    tip.LoadBitmap(IDB_TIP);
    boardbmp.LoadBitmap(IDB_CHESSBOARD);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
//    ON_WM_NCPAINT()
ON_MESSAGE(WM_RECV, &CChildView::OnRecv)
ON_WM_CREATE()
ON_BN_CLICKED(1,&CChildView::OnReg)
ON_WM_LBUTTONUP()
ON_COMMAND(ID_DoubleGame, &CChildView::OnDoublegame)
ON_COMMAND(ID_NewGame, &CChildView::OnNewgame)
ON_COMMAND(ID_REG, &CChildView::OnReg)
ON_COMMAND(ID_Save, &CChildView::OnSave)
ON_COMMAND(ID_Load, &CChildView::OnLoad)
ON_COMMAND(ID_32802, &CChildView::On32802)
ON_COMMAND(ID_32803, &CChildView::On32803)
ON_COMMAND(ID_32804, &CChildView::On32804)
ON_COMMAND(ID_32806, &CChildView::On32806)
ON_COMMAND(ID_32807, &CChildView::On32807)
ON_UPDATE_COMMAND_UI(ID_32807, &CChildView::OnUpdate32807)
ON_UPDATE_COMMAND_UI(ID_32806, &CChildView::OnUpdate32806)
ON_COMMAND(ID_32808, &CChildView::On32808)
ON_UPDATE_COMMAND_UI(ID_32808, &CChildView::OnUpdate32808)
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);// 用于绘制的设备上下文
    CDC dcMemory;
    CBitmap * oldbmp; 
    dcMemory.CreateCompatibleDC(&dc);
    oldbmp = dcMemory.SelectObject(&boardbmp);
    boardbmp.GetObject(sizeof(bmInfo),&bmInfo);
    dc.StretchBlt(0,0,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,SRCCOPY);
    dcMemory.SelectObject(oldbmp);
    if(pGC != NULL) pGC->chessboard->DrawAll();
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
     
}


//void CChildView::OnNcPaint()
//{
//    CWindowDC dc(this);
//    dc.LineTo(10,10);
//    // TODO: 在此处添加消息处理程序代码
//    // 不为绘图消息调用 CWnd::OnNcPaint()
//}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    // TODO:  在此添加您专用的创建代码

    //Init();
	hwnd = this->m_hWnd;
    CString str;
    str.Format(_T("悔棋"));
    btn.Create(str.GetBuffer (),WS_CHILD | BS_DEFPUSHBUTTON|WS_VISIBLE|BS_OWNERDRAW ,CRect(660,170,150,80),this, 1); 
    btn.LoadBitmaps(IDB_NORMAL,IDB_DOWN);
    btn.SizeToContent ();
    //btn.Create(str.GetBuffer (),WS_CHILD | BS_DEFPUSHBUTTON|WS_VISIBLE ,CRect(100,30,150,80),this, 1); 
    //btn.SetBitmap(wchess.operator HBITMAP());
    
    return 0;
}
/*
void CChildView::Init(void)
{
    HGLRC hrc;
    m_hdc = new CClientDC(this);
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0,
    };
    int idpf = ChoosePixelFormat(m_hdc->GetSafeHdc(),&pfd);
    SetPixelFormat(m_hdc->GetSafeHdc(),idpf,&pfd);
    hrc = wglCreateContext(m_hdc->GetSafeHdc());
	wglMakeCurrent(m_hdc->GetSafeHdc(), hrc);
}
*/

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    int x = (point.x-46)/chess;
    int y = (point.y-46)/chess;
    pGC->TryPut(x+1,y+1);
    //CClientDC dc(this);
    //dc.Ellipse (x*chess+1,y*chess+1,x*chess+chess,y*chess+chess);
    CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::DrawChess(int x, int y,int color)
{
    /*if(color != WHITE && color != BLACK) return;
    x--;
    y--;
    CBrush *pbl = new CBrush;
    CBrush *cold;
    CClientDC dc(this);
    if(color == WHITE){
        pbl->CreateSolidBrush(RGB(255,255,255));
        cold = dc.SelectObject(pbl);
        dc.Ellipse (x*chess+chess/8,y*chess+chess/8,x*chess+7*chess/8,y*chess+7*chess/8);
    }
    if(color == BLACK){
        pbl->CreateSolidBrush(RGB(0,0,0));
        cold = dc.SelectObject(pbl);
        dc.Ellipse (x*chess+chess/8,y*chess+chess/8,x*chess+7*chess/8,y*chess+7*chess/8);
    }
    dc.SelectObject(cold);
    pbl->DeleteObject();*/
    if(color != WHITE && color != BLACK) return;
    x--;
    y--;
    CClientDC dc(this);
    CDC dcMemory;
    CBitmap * oldbmp;
    if(color == WHITE){
        wchess.GetObject(sizeof(bmInfo),&bmInfo);
        dcMemory.CreateCompatibleDC(&dc);
        oldbmp = dcMemory.SelectObject(&wchess);
        //dc.Ellipse (x*chess+chess/8,y*chess+chess/8,x*chess+7*chess/8,y*chess+7*chess/8);
        //dc.StretchBlt(x*chess+chess/8,y*chess+chess/8,3*chess/4,3*chess/4,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,SRCCOPY);
        dc.TransparentBlt(55+x*chess,59+y*chess,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
        //dc.BitBlt(x*chess+(chess-bmInfo.bmWidth)/2,y*chess+(chess-bmInfo.bmHeight)/2,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,SRCCOPY);
    }
    if(color == BLACK){
        bchess.GetObject(sizeof(bmInfo),&bmInfo);
        dcMemory.CreateCompatibleDC(&dc);
        oldbmp = dcMemory.SelectObject(&bchess);
        //dc.Ellipse (x*chess+chess/8,y*chess+chess/8,x*chess+7*chess/8,y*chess+7*chess/8);
        dc.TransparentBlt(55+x*chess,59+y*chess,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
       // dc.BitBlt(x*chess+(chess-bmInfo.bmWidth)/2,y*chess+(chess-bmInfo.bmHeight)/2,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,SRCCOPY);
    }
    dcMemory.SelectObject(oldbmp);
    //dcMemory.DeleteDC();
}

void CChildView::OnDoublegame()
{
    // TODO: 在此添加命令处理程序代码
    
    if(pGC != NULL) delete pGC;
    pGC = new GameControl();
    pGC->chessboard->pwnd = this;
    this->InvalidateRect(NULL);
    this->UpdateWindow();
}

void CChildView::OnNewgame()
{
    // TODO: 在此添加命令处理程序代码
	if(dif == 2) {
		if(pGC != NULL) delete pGC;
		pGC = new AI();
		pGC->chessboard->pwnd = this;
		this->InvalidateRect(NULL);
		this->UpdateWindow();
	}
	else {
		if(pGC != NULL) delete pGC;
	    pGC = new AiHm();
		pGC->chessboard->pwnd = this;
	    this->InvalidateRect(NULL);
		this->UpdateWindow();
	}
}

void CChildView::OnReg()
{
    if(pGC != NULL)
    {
        pGC->Regret();
    }
    // TODO: 在此添加命令处理程序代码
}

void CChildView::DrawTip(int x, int y)
{
    x--;
    y--;
    CClientDC dc(this);
    CDC dcMemory;
    CBitmap * oldbmp;
    tip.GetObject(sizeof(bmInfo),&bmInfo);
    dcMemory.CreateCompatibleDC(&dc);
    oldbmp = dcMemory.SelectObject(&tip);
    dc.TransparentBlt(55+x*chess,59+y*chess,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
    dcMemory.SelectObject(oldbmp);
    //dcMemory.DeleteDC();
}

void CChildView::OnSave()
{
    // TODO: 在此添加命令处理程序代码
     CString FilePathName;
     CFileDialog dlg(FALSE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
     if(dlg.DoModal()==IDOK)
     {
        FilePathName=dlg.GetPathName();
        std::ofstream ofile(FilePathName.GetBuffer());
        if(pGC != NULL)
        {
            pGC->WriteCM(ofile);
         }
    }
}

void CChildView::OnLoad()
{
    // TODO: 在此添加命令处理程序代码
    CString FilePathName;
     CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
     if(dlg.DoModal()==IDOK)
     {
        FilePathName=dlg.GetPathName();
        std::ifstream ifile(FilePathName.GetBuffer());
        if(pGC != NULL)
        {
            pGC->ReadCM(ifile);
         }
    }
}

void CChildView::DrawState(int state)
{
    CClientDC dc(this);
    CDC dcMemory;
    CBitmap * oldbmp;
    if(state == WHITE){
        wchess.GetObject(sizeof(bmInfo),&bmInfo);
        dcMemory.CreateCompatibleDC(&dc);
        oldbmp = dcMemory.SelectObject(&wchess);
        //dc.Ellipse (x*chess+chess/8,y*chess+chess/8,x*chess+7*chess/8,y*chess+7*chess/8);
        //dc.StretchBlt(x*chess+chess/8,y*chess+chess/8,3*chess/4,3*chess/4,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,SRCCOPY);
        dc.TransparentBlt(668,37,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
        //dc.BitBlt(x*chess+(chess-bmInfo.bmWidth)/2,y*chess+(chess-bmInfo.bmHeight)/2,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,SRCCOPY);
        dcMemory.SelectObject(oldbmp);
    }
    if(state == BLACK){
        bchess.GetObject(sizeof(bmInfo),&bmInfo);
        dcMemory.CreateCompatibleDC(&dc);
        oldbmp = dcMemory.SelectObject(&bchess);
        //dc.Ellipse (x*chess+chess/8,y*chess+chess/8,x*chess+7*chess/8,y*chess+7*chess/8);
        dc.TransparentBlt(668,37,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
       // dc.BitBlt(x*chess+(chess-bmInfo.bmWidth)/2,y*chess+(chess-bmInfo.bmHeight)/2,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,SRCCOPY);
        dcMemory.SelectObject(oldbmp);
    }
}

void CChildView::DrawNum(int num1,int num2)
{
    if(num1 > 64 || num1 < 0) return;
    if(num2 > 64 || num2 < 0) return;
    //CBitmap number;
    CBitmap * oldbmp;
    CClientDC dc(this);
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(&dc);
    int a,b;
    a = num1 % 10;
    b = num1 / 10;
    if(b != 0)
    {
        LoadNum(number,b);
        number.GetObject(sizeof(bmInfo),&bmInfo);
        oldbmp = dcMemory.SelectObject(&number);
        dc.TransparentBlt(650,123,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
        dcMemory.SelectObject(oldbmp);
    }
    LoadNum(number,a);
    number.GetObject(sizeof(bmInfo),&bmInfo);
    oldbmp = dcMemory.SelectObject(&number);
    dc.TransparentBlt(672,123,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
    dcMemory.SelectObject(oldbmp);
    a = num2 % 10;
    b = num2 / 10;
    if(b != 0)
    {
        LoadNum(number,b);
        number.GetObject(sizeof(bmInfo),&bmInfo);
        oldbmp = dcMemory.SelectObject(&number);
        dc.TransparentBlt(734,123,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
        dcMemory.SelectObject(oldbmp);
    }
    LoadNum(number,a);
    number.GetObject(sizeof(bmInfo),&bmInfo);
    oldbmp = dcMemory.SelectObject(&number);
    dc.TransparentBlt(756,123,bmInfo.bmWidth,bmInfo.bmHeight,&dcMemory,0,0,bmInfo.bmWidth,bmInfo.bmHeight,RGB(255,255,255));
    dcMemory.SelectObject(oldbmp);
}

LRESULT CChildView::OnRecv(WPARAM wParam, LPARAM lParam)
{
	char* conversation;
    char* msg = (char*)wParam;
    switch(msg[0])
	{
	case REGRET:
		if(msg[1] == pGC->step)
		{
			pGC->GameControl::Regret();
			pGC->step--;
		}
		break;
	case PUTCHESS:
		if(msg[1] == pGC->step)
		{
			pGC->GameControl::TryPut(msg[2],msg[3]);
			pGC->step++;
		}
		break;
	case CONVERSATION:
		conversation = msg+1;
		break;
	default:
		;
	}
	return 0;
}

void CChildView::LoadNum(CBitmap & number,int num)
{
    number.DeleteObject();
    switch(num)
    {
    case 0:
        number.LoadBitmap(NUM0);
        break;
    case 1:
        number.LoadBitmap(NUM1);
        break;
    case 2:
        number.LoadBitmap(NUM2);
        break;
    case 3:
        number.LoadBitmap(NUM3);
        break;
    case 4:
        number.LoadBitmap(NUM4);
        break;
    case 5:
        number.LoadBitmap(NUM5);
        break;
    case 6:
        number.LoadBitmap(NUM6);
        break;
    case 7:
        number.LoadBitmap(NUM7);
        break;
    case 8:
        number.LoadBitmap(NUM8);
        break;
    case 9:
        number.LoadBitmap(NUM9);
        break;
    default:
        break;
    }
}

void CChildView::On32802()
{
    // TODO: 在此添加命令处理程序代码
    if(pGC != NULL) delete pGC;
    pGC = new NetHm();
    pGC->color = BLACK;
    pGC->chessboard->pwnd = this;
    this->InvalidateRect(NULL);
    this->UpdateWindow();
}

void CChildView::On32803()
{
    // TODO: 在此添加命令处理程序代码
    if(pGC != NULL) delete pGC;
    pGC = new NetHm();
    pGC->color = WHITE;
    pGC->chessboard->pwnd = this;
    this->InvalidateRect(NULL);
    this->UpdateWindow();
}



void CChildView::On32804()
{
    // TODO: 在此添加命令处理程序代码
    if(pGC != NULL) delete pGC;
    pGC = new Watch();
    pGC->chessboard->pwnd = this;
    this->InvalidateRect(NULL);
    this->UpdateWindow();
}

void CChildView::On32806()
{
    dif = 1;
    // TODO: 在此添加命令处理程序代码
}

void CChildView::On32807()
{
    dif = 0;
    // TODO: 在此添加命令处理程序代码
}

void CChildView::OnUpdate32807(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if(dif == 0) 
    {
        pCmdUI->SetCheck();
    }else
    {
        pCmdUI->SetCheck(0);
    }

}

void CChildView::OnUpdate32806(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if(dif == 1)
    {
        pCmdUI->SetCheck();
    }else
    {
        pCmdUI->SetCheck(0);
    }
}

void CChildView::On32808()
{
	// TODO: Add your command handler code here
	dif = 2;
}

void CChildView::OnUpdate32808(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
    if(dif == 2)
    {
        pCmdUI->SetCheck();
    }else
    {
        pCmdUI->SetCheck(0);
    }
}

// ChildView.h : CChildView 类的接口
//


#pragma once
#include "Game.h"
#include "afxwin.h"

#define WM_RECV (WM_USER+100)
#define PUTCHESS		1
#define REGRET			-1
#define CONVERSATION	2

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 属性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
//    afx_msg void OnNcPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void Init(void);
    CClientDC* m_hdc;
    int chess;
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    GameControl* pGC;
    void DrawChess(int x, int y, int color);
private:
    CBitmap wchess;
    CBitmap bchess;
    CBitmap boardbmp;
    CBitmap number;
    BITMAP bmInfo;
    CBitmap tip;
public:
	static HWND hwnd;
    afx_msg void OnDoublegame();
    afx_msg void OnNewgame();
    afx_msg void OnReg();
    void DrawTip(int x, int y);
    afx_msg void OnSave();
    afx_msg void OnLoad();
    CBitmapButton btn;
    void DrawState(int state);
    void DrawNum(int num1,int num2);
    void LoadNum(CBitmap & bmp,int num);
    afx_msg LRESULT OnRecv(WPARAM wParam, LPARAM lParam);
    afx_msg void On32802();
    afx_msg void On32803();
    afx_msg void On32804();
    afx_msg void On32806();
    afx_msg void On32807();
    static int dif;
    afx_msg void OnUpdate32807(CCmdUI *pCmdUI);
    afx_msg void OnUpdate32806(CCmdUI *pCmdUI);
	afx_msg void On32808();
	afx_msg void OnUpdate32808(CCmdUI *pCmdUI);
};


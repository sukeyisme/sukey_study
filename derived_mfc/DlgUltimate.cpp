// DlgUltimate.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgUltimate.h"


// CDlgUltimate 对话框

IMPLEMENT_DYNAMIC(CDlgUltimate, CDialog)

CDlgUltimate::CDlgUltimate(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent),isActivating(0),_wndTextLength(0),wndWidth(0),wndHeight(0),
	_wndIcon(__icon_title__),_hasMinimizebox(FALSE),_hasMaximizebox(FALSE),_hasClose(FALSE),
	_wndStyle(0),_bmpBG(NULL),_bShadowCreated(FALSE)
{
	memset(_wndText,0,sizeof(_wndText));
	memset(rtSystem,0,sizeof(rtSystem));
}

CDlgUltimate::~CDlgUltimate()
{
	if(_wndIcon&&_wndIcon!=__icon_title__)
	{
		DeleteObject(_wndIcon);
	}
}

void CDlgUltimate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUltimate, CDialog)
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDlgUltimate::OnInitDialog()
{
	long a =0;
	CDialog::OnInitDialog();
	_wndStyle = CWnd::GetStyle();
	_hasMinimizebox = (_wndStyle&WS_MINIMIZEBOX)?TRUE:FALSE;
	_hasMaximizebox = (_wndStyle&WS_MAXIMIZEBOX)?TRUE:FALSE;
	_hasClose = (_wndStyle&WS_SYSMENU)?TRUE:FALSE;


	//_canSizing = (_wndStyle&WS_SIZEBOX)?TRUE:FALSE;
	//_allTitle = (_wndStyle&WS_DLGFRAME)?FALSE:TRUE;

	//ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);
	//_wndTextLength=::GetWindowText(m_hWnd,_wndText,64);

	//if(_wndStyle&WS_DLGFRAME)
	//	::MoveWindow(m_hWnd,(spiWorkarea.right-wndWidth)/2,
	//		(spiWorkarea.bottom-wndHeight)/2,wndWidth+1,wndHeight+1,FALSE);
	afxSubWindow(m_hWnd);//钩子函数 修改对话框上控件

	//if(*_appDialog)
	//	PostMessage(__um_setdialogpos__);
	return TRUE;
}



void CDlgUltimate::SetWindowText(LPCTSTR lpszString)
{
	CDialog::SetWindowText(lpszString);
	_wndTextLength=lstrlen(lstrcpyn(_wndText,lpszString,64));
	OnNcPaint();
}

void CDlgUltimate::OnPaint()
{
	if(!_bShadowCreated)
	{
		_wndShadow.Create(m_hWnd,TRUE);
		_bShadowCreated=TRUE;
	}
	//::GetClientRect(m_hWnd,&rtClient);
	::BeginPaint(m_hWnd,&wndps);
	//HDC hDC=CreateCompatibleDC(wndps.hdc);
	////HBITMAP bmpCompatible=CreateCompatibleBitmap(wndps.hdc,wndWidth,wndHeight);
	//HGDIOBJ oldCompatible=SelectObject(hDC,__bitmap_background);
	////::SetBkColor(hDC,__system_dialogback__);
	//::ExtTextOut(hDC,0,0,ETO_OPAQUE,&rtClient,NULL,0,NULL);
	//*if(_copyDCPart)
	//	uxsRgnBlt(m_hWnd,wndps.hdc,hDC,wndWidth,wndHeight);
	//else*/
	//long lWidth(0),lHeight(0);
	//GetBitmapRect(lWidth,lHeight,__bitmap_background);
	//BitBlt(wndps.hdc,0,0,wndWidth,wndHeight,hDC,lWidth>wndWidth?lWidth-wndWidth:0,lHeight>wndHeight ? lHeight-wndHeight:0,SRCCOPY);
	//::SelectObject(hDC,oldCompatible);
	////::DeleteObject(bmpCompatible);
	//::DeleteDC(hDC);
	::EndPaint(m_hWnd,&wndps);
}

void CDlgUltimate::OnNcPaint()
{
	long i(0);
	HDC wndDC=::GetWindowDC(m_hWnd);
	HDC hDC = CreateCompatibleDC(wndDC);
	HBITMAP bmpCompatible=CreateCompatibleBitmap(wndDC,wndWidth,wndHeight);
	HGDIOBJ oldCompatible=SelectObject(hDC,bmpCompatible);

	//标题栏上边框背景
	RECT rtDrawBack={dialog_cxleft,0,wndWidth,dialog_cycaption};
	HBRUSH hr = CreateSolidBrush(RGB(7,106,189));
	HGDIOBJ oldhr = SelectObject(hDC,&hr);

	FillRect(hDC,&rtDrawBack,hr);
	DeleteObject(SelectObject(hDC,oldhr));
	
	//画右上角三个图标
	for(i=0;i<3;++i)
	{
		if(rtSystem[i].right<1) continue;
		DrawIconEx(hDC,rtSystem[i].left,dialog_cycaption/2-(rtSystem[i].bottom-rtSystem[i].top),__icon_close__[0],16,16,0,0,DI_NORMAL);
	}
	
	//图标&标题
	RECT rtDraw={40,2,wndWidth-dialog_cxright-60,dialog_cycaption};
	if(_wndIcon)
	{
		DrawIconEx(hDC,10+dialog_cxleft,(dialog_cycaption-14)/2,_wndIcon,16,16,0,0,DI_NORMAL);
	}
	if(_wndTextLength)
	{
		HGDIOBJ oldFont=SelectObject(hDC,__sysfont_defaultgui__);
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,frame_clrcaption[isActivating]);
		DrawText(hDC,_wndText,_wndTextLength,&rtDraw,DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
		SelectObject(hDC,oldFont);
	}
	uxsCopyDCPart(wndDC,hDC,wndWidth,wndHeight,dialog_cxleft,dialog_cycaption,dialog_cxright,dialog_cybottom);
	::SelectObject(hDC,oldCompatible);
	::DeleteObject(bmpCompatible);
	::DeleteDC(hDC);
	::ReleaseDC(m_hWnd,wndDC);
}

BOOL CDlgUltimate::OnEraseBkgnd(CDC* pDC)
{
	return(TRUE);
}

void CDlgUltimate::OnSize(UINT nType, int cx, int cy)
{
	RECT rtWindow={0};
	CDialog::OnSize(nType, cx, cy);
	::GetWindowRect(m_hWnd,&rtWindow);
	wndWidth = rtWindow.right - rtWindow.left;
	wndHeight = rtWindow.bottom - rtWindow.top;
	
	long xPos = wndWidth-8;
	long cxImage(0),cyImage(0);
	if(__icon_close__[0])
		GetIconRect(cxImage,cyImage,__icon_close__[0]);
	if(_hasClose)
	{
		SetRect(&rtSystem[2],xPos-cxImage,6,xPos,cyImage); //关闭按钮
		xPos -= cxImage;
	}
	if(_hasMaximizebox)
	{
		SetRect(&rtSystem[1],xPos-cxImage,6,xPos,cyImage); //最大化
		xPos -= cxImage;
	}
	if(_hasMinimizebox)
		SetRect(&rtSystem[0],xPos-cxImage,6,xPos,cyImage); //最小化

	//if(wndRgn)	DeleteObject(wndRgn);
	//if(wndRgn=uxsGetRgnFromBitmap(m_hWnd,dialog_bitmap[isActivating?1:0],
	//	wndWidth,wndHeight,dialog_cxleft,dialog_cycaption,dialog_cxright,dialog_cybottom,
	//	__system_clrmask__,64,64,0,0))
	//		::SetWindowRgn(m_hWnd,wndRgn,_canSizing);
}

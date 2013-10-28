#include "stdafx.h"
#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

WndUltimate::WndUltimate(void):wndWidth(0),wndHeight(0),m_hWnd(NULL),_wndFont(NULL),_bmpBG(NULL),
_oldProc(NULL),_wndStyle(0),_wndStyleEx(0),_wndStatus(0),_wndParent(NULL),_lpText(NULL),_lnText(0),
_lnMalloc(0)
{
	memset(&_rtClient,0,sizeof(RECT));
}

WndUltimate::~WndUltimate(void)
{
	if(_lpText)
	{
		delete _lpText;
		_lpText=NULL;
	}
}

LRESULT WndUltimate::_Main(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return(CallWindowProc(_oldProc,m_hWnd,uMsg,wParam,lParam));
}

void WndUltimate::OnDraw(HDC hDC)		//ÄÚ´æDC
{
}

void WndUltimate::OnPaint(HDC wndDC)
{
	HDC hDC=CreateCompatibleDC(wndDC);
	HBITMAP bmpCompatible=CreateCompatibleBitmap(wndDC,wndWidth,wndHeight);
	HGDIOBJ oldCompatible=SelectObject(hDC,bmpCompatible);
	HGDIOBJ oldFont=SelectObject(hDC,_wndFont);
	SetBkMode(hDC,TRANSPARENT);
	ExtTextOut(hDC,0,0,ETO_OPAQUE,&_rtClient,NULL,0,NULL);
	OnDraw(hDC);
	BitBlt(wndDC,0,0,wndWidth,wndHeight,hDC,0,0,SRCCOPY);
	SelectObject(hDC,oldFont);
	SelectObject(hDC,oldCompatible);
	DeleteObject(bmpCompatible);
	DeleteDC(hDC);
}

void WndUltimate::SetButtonBG(HBITMAP hbitmap)
{
	
}

void WndUltimate::GetWindowSize(void)
{
	/*POINT point={0};
	GetWindowRect(m_hWnd,&_rtWnd);
	GetClientRect(m_hWnd,&_rtClient);

	point.x = _rtWnd.left;
	point.y = _rtWnd.top;
	ScreenToClient(m_hWnd,&point);
	_rtBorder.left = -point.x;
	_rtBorder.top = -point.y;

	point.x = _rtWnd.right;
	point.y = _rtWnd.bottom;
	ScreenToClient(m_hWnd,&point);
	_rtBorder.right = point.x-_rtClient.right;
	_rtBorder.bottom = point.y-_rtClient.bottom;

	_rtBorder.left %= scrollbar_width;
	_rtBorder.top %= scrollbar_width;
	_rtBorder.right %= scrollbar_width;
	_rtBorder.bottom %= scrollbar_width;

	OffsetRect(&_rtWnd,-_rtWnd.left,-_rtWnd.top);
	wndWidth=_rtClient.right-_rtClient.left;
	wndHeight=_rtClient.bottom-_rtClient.top;*/
}

LPTSTR WndUltimate::GetWindowText(void)
{
	long lnText=GetWindowTextLength(m_hWnd);
	if(lnText>=_lnMalloc)
	{
		_lnText = _lnMalloc = 0;
		delete(LPSTR&)_lpText;
		if(!(_lpText=(LPTSTR)new char[(lnText+1)*sizeof(TCHAR)]))return(NULL);
		_lnMalloc = (lnText+1)*sizeof(TCHAR);
		_lnMalloc /= sizeof(TCHAR);
	}
	_lnText=::GetWindowText(m_hWnd,_lpText,_lnMalloc);
	return(_lpText);
}
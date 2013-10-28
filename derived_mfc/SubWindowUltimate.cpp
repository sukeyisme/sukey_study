#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

LRESULT WINAPI _afxWndUltimate(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	WndUltimate*pWindow=(WndUltimate*)GetProp(hWnd,__UMyProp__);
	return(pWindow->_Main(uMsg,wParam,lParam));
}

WndUltimate*BindWindow(HWND hWnd,WndUltimate*pWindow,DWORD wndStyle,DWORD wndStyleEx)
{
	if(pWindow)
	{
		SetProp(hWnd,__UMyProp__,pWindow);
		pWindow->m_hWnd = hWnd;
		pWindow->_wndParent = GetParent(hWnd);
		pWindow->_wndStyle = wndStyle;
		pWindow->_wndStyleEx = wndStyleEx;
		pWindow->_oldProc = (WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)_afxWndUltimate);
		pWindow->GetWindowSize();
		pWindow->OnEnvReset();
	}
	return(pWindow);
}

static void rectScreenToClient(HWND hWnd,LPRECT lpRect)
{
	POINT point={lpRect->left,lpRect->top};
	::ScreenToClient(hWnd,&point);
	::OffsetRect(lpRect,point.x-lpRect->left,point.y-lpRect->top);
}

MFCDERIVED_API void afxSubClientWindow(HWND hChild)
{
	//if(GetProp(hChild,__UMyProp__)) return;	//已经绑定过了
	RECT rtWindow={0};
	TCHAR szName[64]={0};
	DWORD wndStyle=GetWindowLong(hChild,GWL_STYLE);
	DWORD wndStyleEx=GetWindowLong(hChild,GWL_EXSTYLE);
	::GetWindowRect(hChild,&rtWindow);
	rectScreenToClient(GetParent(hChild),&rtWindow);
	GetClassName(hChild,szName,64);
	if(!lstrcmp(szName,WC_BUTTON))
	{
		switch(wndStyle&SS_TYPEMASK)
		{
		case	BS_DEFPUSHBUTTON:	//默认按钮
		case	BS_PUSHBUTTON:		//普通按钮
			if(ButtonUltimate*pButton=new ButtonUltimate)
			{
				::MoveWindow(hChild,rtWindow.left,rtWindow.top,rtWindow.right-rtWindow.left,26,FALSE);
				BindWindow(hChild,pButton,wndStyle,wndStyleEx);
				if(::GetDlgCtrlID(hChild)==IDOK)
				{
					
				}
				if(::GetDlgCtrlID(hChild)==IDCANCEL)
				{
					
				}
			}
			break;
		}
		return;
	}
}

MFCDERIVED_API void afxSubWindow(HWND hWnd)
{
	HWND hChild=GetWindow(hWnd,GW_CHILD);
	for(;hChild;hChild=GetNextWindow(hChild,GW_HWNDNEXT))
	{
		afxSubClientWindow(hChild);
	}
}
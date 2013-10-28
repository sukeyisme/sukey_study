#include "stdafx.h"
#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
ButtonUltimate::ButtonUltimate(void):WndUltimate()
{

}

ButtonUltimate::~ButtonUltimate(void)
{

}

LRESULT ButtonUltimate::_Main(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return(WndUltimate::_Main(uMsg,wParam,lParam));
}

void ButtonUltimate::OnEnvReset(void)	//初始化或者重新加载环境用
{
	GetWindowText();
}

void ButtonUltimate::OnDraw(HDC hDC)
{
	
}

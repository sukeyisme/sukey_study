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

void ButtonUltimate::OnEnvReset(void)	//��ʼ���������¼��ػ�����
{
	GetWindowText();
}

void ButtonUltimate::OnDraw(HDC hDC)
{
	
}

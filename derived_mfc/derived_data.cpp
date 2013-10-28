#include "stdafx.h"
//系统字体
MFCDERIVED_API HFONT __sysfont_defaultgui__=NULL;	//系统界面字体
MFCDERIVED_API COLORREF frame_clrcaption[2]={0};	//标题颜色
MFCDERIVED_API HICON	__icon_title__=NULL;		//公司图标
MFCDERIVED_API HICON	__icon_close__[2];      //关闭图标
MFCDERIVED_API HBITMAP __bitmap_background; //背景图片
MFCDERIVED_API PAINTSTRUCT wndps={0};			//全局句柄(WM_PAINT时候用)
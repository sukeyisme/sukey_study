// dllmain.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE derived_mfcDLL = { NULL, NULL };
GdiplusStartupInput g_Gdiplus;
ULONG_PTR g_GdiplusToken=NULL;
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("derived_mfc.DLL 正在初始化!\n");
		
		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(derived_mfcDLL, hInstance))
			return 0;

		//获取路径
		long l(0);
		GetModuleFileName(hInstance,__main_home__,MAX_PATH);
		for(l=lstrlen(__main_home__);l>0&&__main_home__[l]!=_T('\\');__main_home__[l--]=0);
		SetCurrentDirectory(__main_home__);
		//开启GDI+
		GdiplusStartup(&g_GdiplusToken,&g_Gdiplus,NULL);
		//初始化阴影系统
		CWndShadow::Initialize(hInstance);
		//创建系统默认字体
		__sysfont_defaultgui__=CreateFont(15,0,0,0,
			FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("宋体"));
		if(!__sysfont_defaultgui__) __sysfont_defaultgui__=(HFONT)GetStockObject(DEFAULT_GUI_FONT);

		//窗口标题颜色
		frame_clrcaption[0] = RGB(255,255,255);
		frame_clrcaption[1] = RGB(255,255,255);

		//加载标题图标
		if(!__icon_title__)
			__icon_title__ = localLoadIcon(hInstance,IDI_ICON_TITLE);
		//加载关闭图标
		if(!__icon_close__[0])
			__icon_close__[0] = localLoadIcon(hInstance,IDI_ICON_CLOSE);
		if(!__icon_close__[1])
			__icon_close__[1] = localLoadIcon(hInstance,IDI_ICON_CLOSE);

		//加载背景图片
		if(!__bitmap_background)
		{
			TCHAR szPath[MAX_PATH] = {0};
			wsprintf(szPath,_T("%sres\\background.png"),__main_home__);
			__bitmap_background = uwpLoadImageFilePlus(szPath);
		}

		new CDynLinkLibrary(derived_mfcDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("derived_mfc.DLL 正在终止!\n");
		DestroyIcon(__icon_title__);
		for(long i=0;i<ARRAYSIZE(__icon_close__);++i)
			DestroyIcon(__icon_close__[i]);

		
		// 在调用析构函数之前终止该库
		DeleteObject(__sysfont_defaultgui__);
		AfxTermExtensionModule(derived_mfcDLL);
		GdiplusShutdown(g_GdiplusToken);
		g_GdiplusToken = NULL;
	}
	return 1;   // 确定
}

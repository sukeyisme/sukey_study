// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
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
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("derived_mfc.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(derived_mfcDLL, hInstance))
			return 0;

		//��ȡ·��
		long l(0);
		GetModuleFileName(hInstance,__main_home__,MAX_PATH);
		for(l=lstrlen(__main_home__);l>0&&__main_home__[l]!=_T('\\');__main_home__[l--]=0);
		SetCurrentDirectory(__main_home__);
		//����GDI+
		GdiplusStartup(&g_GdiplusToken,&g_Gdiplus,NULL);
		//��ʼ����Ӱϵͳ
		CWndShadow::Initialize(hInstance);
		//����ϵͳĬ������
		__sysfont_defaultgui__=CreateFont(15,0,0,0,
			FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("����"));
		if(!__sysfont_defaultgui__) __sysfont_defaultgui__=(HFONT)GetStockObject(DEFAULT_GUI_FONT);

		//���ڱ�����ɫ
		frame_clrcaption[0] = RGB(255,255,255);
		frame_clrcaption[1] = RGB(255,255,255);

		//���ر���ͼ��
		if(!__icon_title__)
			__icon_title__ = localLoadIcon(hInstance,IDI_ICON_TITLE);
		//���عر�ͼ��
		if(!__icon_close__[0])
			__icon_close__[0] = localLoadIcon(hInstance,IDI_ICON_CLOSE);
		if(!__icon_close__[1])
			__icon_close__[1] = localLoadIcon(hInstance,IDI_ICON_CLOSE);

		//���ر���ͼƬ
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
		TRACE0("derived_mfc.DLL ������ֹ!\n");
		DestroyIcon(__icon_title__);
		for(long i=0;i<ARRAYSIZE(__icon_close__);++i)
			DestroyIcon(__icon_close__[i]);

		
		// �ڵ�����������֮ǰ��ֹ�ÿ�
		DeleteObject(__sysfont_defaultgui__);
		AfxTermExtensionModule(derived_mfcDLL);
		GdiplusShutdown(g_GdiplusToken);
		g_GdiplusToken = NULL;
	}
	return 1;   // ȷ��
}

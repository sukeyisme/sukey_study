#pragma once
//拷贝对话框的4个边
void MFCDERIVED_API uxsCopyDCPart(HDC hDest,HDC hSrc,
							  long lWidth,long lHeight,
							  long cxLeft,long cyTop,long cxRight,long cyBottom);

//加载png资源
HBITMAP MFCDERIVED_API uwpLoadImageFilePlus(LPCTSTR filename);

//加载icon资源
HICON MFCDERIVED_API localLoadIcon(HINSTANCE uwhInstance,UINT uIDIcon);

//得到icon资源信息
BOOL MFCDERIVED_API GetIconRect(long& lWidth,long& lHeight,HICON hicon);

//得到bitmap资源信息
BOOL MFCDERIVED_API GetBitmapRect(long& lWidth,long& lHeight,HBITMAP hbitmap);

extern MFCDERIVED_API TCHAR __main_home__[MAX_PATH];		//主路径(TCHAR)
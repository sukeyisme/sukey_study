#pragma once
//�����Ի����4����
void MFCDERIVED_API uxsCopyDCPart(HDC hDest,HDC hSrc,
							  long lWidth,long lHeight,
							  long cxLeft,long cyTop,long cxRight,long cyBottom);

//����png��Դ
HBITMAP MFCDERIVED_API uwpLoadImageFilePlus(LPCTSTR filename);

//����icon��Դ
HICON MFCDERIVED_API localLoadIcon(HINSTANCE uwhInstance,UINT uIDIcon);

//�õ�icon��Դ��Ϣ
BOOL MFCDERIVED_API GetIconRect(long& lWidth,long& lHeight,HICON hicon);

//�õ�bitmap��Դ��Ϣ
BOOL MFCDERIVED_API GetBitmapRect(long& lWidth,long& lHeight,HBITMAP hbitmap);

extern MFCDERIVED_API TCHAR __main_home__[MAX_PATH];		//��·��(TCHAR)
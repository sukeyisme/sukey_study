#include "stdafx.h"
//ϵͳ����
MFCDERIVED_API HFONT __sysfont_defaultgui__=NULL;	//ϵͳ��������
MFCDERIVED_API COLORREF frame_clrcaption[2]={0};	//������ɫ
MFCDERIVED_API HICON	__icon_title__=NULL;		//��˾ͼ��
MFCDERIVED_API HICON	__icon_close__[2];      //�ر�ͼ��
MFCDERIVED_API HBITMAP __bitmap_background; //����ͼƬ
MFCDERIVED_API PAINTSTRUCT wndps={0};			//ȫ�־��(WM_PAINTʱ����)
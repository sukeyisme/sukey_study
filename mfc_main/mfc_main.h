
// mfc_main.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmfc_mainApp:
// �йش����ʵ�֣������ mfc_main.cpp
//

class Cmfc_mainApp : public CWinAppEx
{
public:
	Cmfc_mainApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_mainApp theApp;
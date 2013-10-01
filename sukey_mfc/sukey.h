#include "sukey_mfc.h"
class CMyWinApp:public CWinApp
{
public:
	CMyWinApp::CMyWinApp(){cout<<"CMyWinApp::Constructor\n";}
	CMyWinApp::~CMyWinApp(){cout<<"CMyWinApp::Desstructor\n";}
	virtual BOOL InitInstance();
};

class CMyFrameWnd:public CFrameWnd
{
public:
	CMyFrameWnd::CMyFrameWnd(){cout<<"CMyFrameWnd::Constructor\n";Create();}
	CMyFrameWnd::~CMyFrameWnd(){cout<<"CMyFrameWnd::Desstructor\n";}
};
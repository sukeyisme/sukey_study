#include "sukey.h"
CMyWinApp theApp;

BOOL CMyWinApp::InitInstance()
{
	cout<<"CMyWinApp::InitInstance\n";
	m_pMainWnd = new CMyFrameWnd;
	return TRUE;
}

int main()
{
	CWinApp* pApp = AfxGetApp();
	pApp->InitApplication();
	pApp->InitInstance();
	pApp->Run();
	return 0;
}
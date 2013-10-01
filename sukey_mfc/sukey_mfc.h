#include<iostream>
#include "sukey_def.h"
using namespace std;
class CObject
{
public:
	CObject::CObject(){cout<<"CObject::Constructor\n";}
	CObject::~CObject(){cout<<"CObject::Destructor\n";}
};

class CCmdTarget:public CObject
{
public:
	CCmdTarget::CCmdTarget(){cout<<"CCmdTarget::Constructor\n";}
	CCmdTarget::~CCmdTarget(){cout<<"CCmdTarget::Destructor\n";}
};

class CWinThread:public CCmdTarget
{
public:
	CWinThread::CWinThread(){cout<<"CWinThread::Constructor\n";}
	CWinThread::~CWinThread(){cout<<"CWinThread::Destructor\n";}
	virtual BOOL InitInstance(){cout<<"CWinThread::InitInstance\n";return TRUE;}
	virtual int Run(){cout<<"CWinThread::Run\n";return 1;}
};

class CWnd:public CCmdTarget
{
public:
	CWnd::CWnd(){cout<<"CWnd::Constructor\n";}
	CWnd::~CWnd(){cout<<"CWnd::Destructor\n";}
	virtual BOOL Create();
	BOOL CreateEx();
	virtual BOOL PreCreateWindow();
};

class CWinApp:public CWinThread
{
public:
	CWinApp* m_pCurrentWinApp;
	CWnd*	m_pMainWnd;
public:
	CWinApp::CWinApp(){m_pCurrentWinApp=this;cout<<"CWinApp::Constructor\n";}
	CWinApp::~CWinApp(){cout<<"CWinApp::Destructor\n";}
	virtual BOOL InitApplication(){cout<<"CWinApp::InitApplication\n";return TRUE;}
	virtual BOOL InitInstance(){cout<<"CWinApp::InitInstance\n";return TRUE;}
	virtual int Run(){cout<<"CWinApp::Run\n";return CWinThread::Run();}
};

class CView:public CWnd
{
public:
	CView::CView(){cout<<"CView::Constructor\n";}
	CView::~CView(){cout<<"CView::Destructor\n";}
};

class CFrameWnd:public CWnd
{
public:
	CFrameWnd::CFrameWnd(){cout<<"CFrameWnd::Constructor\n";}
	CFrameWnd::~CFrameWnd(){cout<<"CFrameWnd::Destructor\n";}
	BOOL Create();
	virtual BOOL PreCreateWindow();
};

class CDocument:public CCmdTarget
{
public:
	CDocument::CDocument(){cout<<"CDocument::Constructor\n";}
	CDocument::~CDocument(){cout<<"CDocument::Destructor\n";}
};

CWinApp* AfxGetApp();

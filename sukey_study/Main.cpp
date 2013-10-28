#include "include.h"


#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{
	//CDlgUltimate dlgTest(0);
	//dlgTest.DoModal();
	return 0;
}
#else
int main(int argc,char *argv[])
{
	CDlgUltimate dlgTest(0);
	dlgTest.DoModal();
	return 0;
}
#endif

//void ServiceMain(DWORD dvArgc,LPTSTR* lpszArgv)
//{
//
//}
#pragma once
class MFCDERIVED_API WndUltimate;
typedef void (*_pUMyDraw)(HDC hDC,WndUltimate*pWindow,HANDLE selfData);
class MFCDERIVED_API WndUltimate
{
public:
	WndUltimate(void);
	virtual~WndUltimate(void);

	virtual	LRESULT _Main(UINT uMsg,WPARAM wParam,LPARAM lParam);//窗口消息处理
	virtual	void OnEnvReset(void)=0;	//初始化或者重新加载环境用:必须要重载!
	enum
	{
		STATUS_HOVER=0x00000001,		//鼠标移过
		STATUS_LBTNDOWN=0x00000002,		//鼠标左键按下
		STATUS_FOCUS=0x00000004,		//获得焦点
	};
	LPTSTR GetWindowText(void);	//获取窗口标题
	void GetWindowSize(void);	//获取窗口大小

	void SetButtonBG(HBITMAP hbitmap);//设置背景
protected:
	virtual void OnPaint(HDC wndDC);	//窗口DC
	virtual void OnDraw(HDC hDC);		//内存DC

public:
	RECT	_rtClient;		//客户区域
	long	wndWidth,wndHeight;	//窗口宽和高
	HWND	m_hWnd;		//窗口句柄
	HWND	_wndParent;	//父窗口句柄
	HFONT	_wndFont;   //字体句柄
	WNDPROC	_oldProc;		//旧的回调函数
	_pUMyDraw	_procDraw;	//重绘函数指针

	LPTSTR	_lpText;//标题名称
	long	_lnText,_lnMalloc;//标题的字符长度

	DWORD	_wndStyle,_wndStyleEx;	//窗口属性
	DWORD	_wndStatus;				//窗口状态
private:
	HBITMAP _bmpBG;		//背景图片
	
};
#pragma once


// CDlgUltimate 对话框

class MFCDERIVED_API CDlgUltimate : public CDialog
{
	DECLARE_DYNAMIC(CDlgUltimate)

public:
	CDlgUltimate(UINT nIDTemplate,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUltimate();

	virtual BOOL OnInitDialog();

	//各种对话框设置
	void	SetWindowText(LPCTSTR lpszString);	//设置标题


	//各种消息
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//各种状态
	long	isActivating;		//窗口激活中
	long	wndWidth,wndHeight;	//窗口宽度和高度
	RECT	rtClient;			//客户区
	RECT	rtSystem[3];		//右上角三个图标位置
	BOOL	_hasMinimizebox;	//最下化
	BOOL	_hasMaximizebox;	//最大化
	BOOL	_hasClose;			//关闭
	DWORD	_wndStyle;			//对话框样式
	HBITMAP	_bmpBG;			//客户区背景图片

private:
	TCHAR	_wndText[64];	//窗口标题
	long	_wndTextLength; //窗口标题长度
	HICON	_wndIcon;		//标题图标
	CWndShadow _wndShadow;	//阴影类
	BOOL	_bShadowCreated;//是否已创建阴影

	DECLARE_MESSAGE_MAP()
};

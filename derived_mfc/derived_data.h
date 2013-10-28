extern MFCDERIVED_API HFONT __sysfont_defaultgui__;	//系统界面字体
extern MFCDERIVED_API COLORREF frame_clrcaption[2];	//标题颜色

//对话框四周框子的
const long dialog_cxleft(0);		//框架左侧边宽
const long dialog_cycaption(40);	//框架上侧标题条高
const long dialog_cxright(0);		//右侧边宽
const long dialog_cybottom(0);		//下侧底高
extern MFCDERIVED_API HICON	__icon_title__;		//公司图标
extern MFCDERIVED_API HICON	__icon_close__[2];      //关闭图标
extern MFCDERIVED_API HBITMAP __bitmap_background;	//背景图片
extern MFCDERIVED_API PAINTSTRUCT wndps;		//全局句柄(WM_PAINT时候用)
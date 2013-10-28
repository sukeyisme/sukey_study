#pragma once
class MFCDERIVED_API ButtonUltimate:public WndUltimate
{
protected:
	virtual void OnDraw(HDC hDC);
public:
	virtual	LRESULT _Main(UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual	void OnEnvReset(void);	//初始化或者重新加载环境用

	ButtonUltimate(void);
	virtual~ButtonUltimate(void);
};
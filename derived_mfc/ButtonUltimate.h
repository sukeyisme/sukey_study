#pragma once
class MFCDERIVED_API ButtonUltimate:public WndUltimate
{
protected:
	virtual void OnDraw(HDC hDC);
public:
	virtual	LRESULT _Main(UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual	void OnEnvReset(void);	//��ʼ���������¼��ػ�����

	ButtonUltimate(void);
	virtual~ButtonUltimate(void);
};
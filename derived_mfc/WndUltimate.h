#pragma once
class MFCDERIVED_API WndUltimate;
typedef void (*_pUMyDraw)(HDC hDC,WndUltimate*pWindow,HANDLE selfData);
class MFCDERIVED_API WndUltimate
{
public:
	WndUltimate(void);
	virtual~WndUltimate(void);

	virtual	LRESULT _Main(UINT uMsg,WPARAM wParam,LPARAM lParam);//������Ϣ����
	virtual	void OnEnvReset(void)=0;	//��ʼ���������¼��ػ�����:����Ҫ����!
	enum
	{
		STATUS_HOVER=0x00000001,		//����ƹ�
		STATUS_LBTNDOWN=0x00000002,		//����������
		STATUS_FOCUS=0x00000004,		//��ý���
	};
	LPTSTR GetWindowText(void);	//��ȡ���ڱ���
	void GetWindowSize(void);	//��ȡ���ڴ�С

	void SetButtonBG(HBITMAP hbitmap);//���ñ���
protected:
	virtual void OnPaint(HDC wndDC);	//����DC
	virtual void OnDraw(HDC hDC);		//�ڴ�DC

public:
	RECT	_rtClient;		//�ͻ�����
	long	wndWidth,wndHeight;	//���ڿ�͸�
	HWND	m_hWnd;		//���ھ��
	HWND	_wndParent;	//�����ھ��
	HFONT	_wndFont;   //������
	WNDPROC	_oldProc;		//�ɵĻص�����
	_pUMyDraw	_procDraw;	//�ػ溯��ָ��

	LPTSTR	_lpText;//��������
	long	_lnText,_lnMalloc;//������ַ�����

	DWORD	_wndStyle,_wndStyleEx;	//��������
	DWORD	_wndStatus;				//����״̬
private:
	HBITMAP _bmpBG;		//����ͼƬ
	
};
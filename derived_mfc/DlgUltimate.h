#pragma once


// CDlgUltimate �Ի���

class MFCDERIVED_API CDlgUltimate : public CDialog
{
	DECLARE_DYNAMIC(CDlgUltimate)

public:
	CDlgUltimate(UINT nIDTemplate,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUltimate();

	virtual BOOL OnInitDialog();

	//���ֶԻ�������
	void	SetWindowText(LPCTSTR lpszString);	//���ñ���


	//������Ϣ
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	//����״̬
	long	isActivating;		//���ڼ�����
	long	wndWidth,wndHeight;	//���ڿ�Ⱥ͸߶�
	RECT	rtClient;			//�ͻ���
	RECT	rtSystem[3];		//���Ͻ�����ͼ��λ��
	BOOL	_hasMinimizebox;	//���»�
	BOOL	_hasMaximizebox;	//���
	BOOL	_hasClose;			//�ر�
	DWORD	_wndStyle;			//�Ի�����ʽ
	HBITMAP	_bmpBG;			//�ͻ�������ͼƬ

private:
	TCHAR	_wndText[64];	//���ڱ���
	long	_wndTextLength; //���ڱ��ⳤ��
	HICON	_wndIcon;		//����ͼ��
	CWndShadow _wndShadow;	//��Ӱ��
	BOOL	_bShadowCreated;//�Ƿ��Ѵ�����Ӱ

	DECLARE_MESSAGE_MAP()
};

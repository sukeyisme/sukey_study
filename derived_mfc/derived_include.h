#pragma once
#ifdef MFCDERIVED_EXPORTS
	#define	MFCDERIVED_API	__declspec(dllexport)
#else
	#define	MFCDERIVED_API	__declspec(dllimport)
#endif
#include "WndShadow.h"//������Ӱ��
#include "derived_data.h"//ȫ�ֹ�������
#include "tools.h"	//�õ��ĸ��ַ���

#include "SubWindowUltimate.h"//�ؼ����Ӵ���
#include "WndUltimate.h"//������
#include "ButtonUltimate.h"//��ť��
#include "DlgUltimate.h"//�Ի���


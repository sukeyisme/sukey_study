#pragma once
#ifdef MFCDERIVED_EXPORTS
	#define	MFCDERIVED_API	__declspec(dllexport)
#else
	#define	MFCDERIVED_API	__declspec(dllimport)
#endif
#include "WndShadow.h"//窗口阴影类
#include "derived_data.h"//全局共享数据
#include "tools.h"	//用到的各种方法

#include "SubWindowUltimate.h"//控件钩子处理
#include "WndUltimate.h"//窗口类
#include "ButtonUltimate.h"//按钮类
#include "DlgUltimate.h"//对话框


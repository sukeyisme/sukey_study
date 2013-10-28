#include "stdafx.h"

void MFCDERIVED_API uxsCopyDCPart(HDC hDest,HDC hSrc,
							  long lWidth,long lHeight,
							  long cxLeft,long cyTop,long cxRight,long cyBottom)
{
	BitBlt(hDest,0,0,cxLeft,lHeight,hSrc,0,0,SRCCOPY);//left
	BitBlt(hDest,lWidth-cxRight,0,cxRight,lHeight,hSrc,lWidth-cxRight,0,SRCCOPY);
	BitBlt(hDest,0,lHeight-cyBottom,lWidth,cyBottom,hSrc,0,lHeight-cyBottom,SRCCOPY);
	BitBlt(hDest,0,0,lWidth,cyTop,hSrc,0,0,SRCCOPY); //Caption
}
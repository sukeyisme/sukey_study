#include "stdafx.h"
HBITMAP uwpLoadImageFilePlus(LPCTSTR filename)
{
	CImage image;
	HRESULT ret = image.Load(filename);
	if(ret!=S_OK) return NULL;
	return image.Detach();
}

HICON localLoadIcon(HINSTANCE uwhInstance,UINT uIDIcon)
{
	if(!uIDIcon) return(NULL);
	return((HICON)LoadImage(uwhInstance,
		    MAKEINTRESOURCE(uIDIcon),IMAGE_ICON,0,0,0));
}

BOOL GetIconRect(long& lWidth,long& lHeight,HICON hicon)
{
	if ( hicon )
    {
        ICONINFO info;
        if ( !::GetIconInfo(hicon, &info) ) return FALSE;    
        else
        {
            HBITMAP hbmp = info.hbmMask;
            if ( hbmp )
            {
                BITMAP bm;
                if ( ::GetObject(hbmp, sizeof(BITMAP), (LPSTR) &bm) )
                {
                    lWidth = bm.bmWidth;
                    lHeight = bm.bmHeight;
                }
                ::DeleteObject(info.hbmMask);
            }
            if ( info.hbmColor )
                ::DeleteObject(info.hbmColor);
			return TRUE;
        }
    }
	return FALSE;
}

BOOL GetBitmapRect(long& lWidth,long& lHeight,HBITMAP hbitmap)
{
	BITMAP bm;
	if(!hbitmap) return FALSE;
	if(!GetObject(hbitmap,sizeof(BITMAP),&bm)) return FALSE;
	lWidth = bm.bmWidth;
	lHeight = bm.bmHeight;
	return TRUE;
}
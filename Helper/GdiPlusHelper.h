#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// #20230613 :: Gdi+ helper class
//
// 
//
// 
// 
// 
// 
// 
// 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

class CGdiPlusHelper
{
// non-static
public:
	CGdiPlusHelper(void);
	~CGdiPlusHelper(void);

	BOOL				Init();

	INT32				DrawBall(HDC hDC, RectF& rcRect, COLORREF colorBall, BOOL bFlat);
	INT32				DrawGradientBackGound(HDC hDC, CRect& rcRect, COLORREF colorBackGround, Color colorHighLight, INT32 nFeather);
	
	INT32				SetTransform(LPCTSTR lpszPath, Bitmap** ppBitmap);

	int					GetEncoderClsid(const WCHAR* pszFormat, CLSID* pClsid);

private:
	ULONG_PTR			m_gdiToken;

};
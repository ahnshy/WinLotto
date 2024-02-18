#pragma once
#include <gdiplus.h>
#include <map>
using namespace std;
using namespace Gdiplus;

typedef map<INT32, RectF>				MapSize;

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

	BOOL					Init();

	INT32					DrawBall(HDC hDC, RectF& rcRect, COLORREF colorBall, CString strText, BOOL bFlat);
	INT32					DrawGradientBackGound(HDC hDC, CRect& rcRect, COLORREF colorBackGround, Color colorHighLight, INT32 nFeather);
	
	INT32					SetTransform(LPCTSTR lpszPath, Bitmap** ppBitmap);

	int						GetEncoderClsid(const WCHAR* pszFormat, CLSID* pClsid);

private:
	ULONG_PTR				m_gdiToken;

	Font*					m_pTextFont;
	
	//CArray<RectF, RectF&>	m_arrFontSize;
	MapSize					m_mapSize;

	void					MeasureFontSize();

};
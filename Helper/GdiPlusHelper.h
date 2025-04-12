#pragma once
#include <gdiplus.h>
#include <map>
using namespace std;
using namespace Gdiplus;

#define		COLOR_PAGE_SIMULATION_DLG_BG_RGB						RGB(5, 36, 63)
//#define		COLOR_PAGE_SIMULATION_DLG_BG							Color(255, 5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE			Color(255, 20, 77, 126)

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


	// Gdi+ Double Buffering Method
	INT32					BufferDrawContainer(CRect& rcRect, Gdiplus::PointF center, float radius);
	INT32					BufferDrawGradientBackGound(CRect& rcRect, COLORREF colorBackGround, Color colorHighLight, INT32 nFeather);
	INT32					BufferDrawBall(RectF& rcRect, COLORREF colorBall, CString strText, BOOL bFlat);
	INT32					BufferBitblt(HDC hDC, CRect& rcTarget);
	
	INT32					SetTransform(LPCTSTR lpszPath, Bitmap** ppBitmap);

	int						GetEncoderClsid(const WCHAR* pszFormat, CLSID* pClsid);

private:
	ULONG_PTR				m_gdiToken;

	Gdiplus::Font*			m_pTextFont;
	Bitmap*					m_pBufferBitmap;
	
	//CArray<RectF, RectF&>	m_arrFontSize;
	MapSize					m_mapSize;

	void					MeasureFontSize();

};
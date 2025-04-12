#include "StdAfx.h"

#include "GdiPlusHelper.h"

#pragma comment(lib, "gdiplus")
CGdiPlusHelper::CGdiPlusHelper()
{
	m_pTextFont = NULL;
	m_gdiToken = 0;
	m_mapSize.clear();

	m_pBufferBitmap = NULL;
}

CGdiPlusHelper::~CGdiPlusHelper()
{
	if (m_pBufferBitmap)
	{
		delete m_pBufferBitmap;
		m_pBufferBitmap = NULL;
	}

	m_mapSize.clear();

	if (m_pTextFont)
	{
		delete m_pTextFont;
		m_pTextFont = NULL;
	}

	if (m_gdiToken != 0)
	{
		GdiplusShutdown(m_gdiToken);
		m_gdiToken = 0;
	}
}

BOOL CGdiPlusHelper::Init()
{
	GdiplusStartupInput input;
	if (GdiplusStartup(&m_gdiToken, &input, NULL) != Ok)
		return FALSE;

	MeasureFontSize();

	if (m_pBufferBitmap)
	{
		delete m_pBufferBitmap;
		m_pBufferBitmap = NULL;
	}

	return TRUE;
}

void CGdiPlusHelper::MeasureFontSize()
{
	CPaintDC dc(CWnd::FromHandle(GetDesktopWindow()));

	Graphics g(dc.GetSafeHdc());

	TCHAR szStr[] = _T("50");
	RectF layoutRect(0.0f, 0.0f, 32767.0f, 32767.0f);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	RectF boundRect;

	for (REAL rSize = 1 ; rSize <= 72 ; rSize++)
	{
		m_pTextFont = new Gdiplus::Font(DEFAULT_FONT, rSize);
		if (!m_pTextFont)
			continue;

		// Measure the string.
		g.MeasureString(szStr, (INT32)_tcslen(szStr), m_pTextFont, layoutRect, &format, &boundRect);
		m_mapSize.insert(make_pair(rSize, boundRect));

		delete m_pTextFont;
		m_pTextFont = NULL;
	}
}

INT32 CGdiPlusHelper::DrawGradientBackGound(HDC hDC, CRect& rcRect, COLORREF colorBackGround, Color colorHighLight, INT32 nFeather)
{
	if (hDC == NULL)
		return -1;

	Graphics graphics(hDC);
	Bitmap mBitmap(rcRect.Width(), rcRect.Height());

	Graphics mGraphics(&mBitmap);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.SetInterpolationMode(InterpolationModeHighQuality);

	float const nMargin = 2;
	RectF rcTarget(-nMargin, -nMargin, (float)rcRect.Width() + nMargin, (float)rcRect.Height() + nMargin);

	SolidBrush brushBk(Color(255, GetRValue(colorBackGround), GetGValue(colorBackGround), GetBValue(colorBackGround)));
	mGraphics.FillRectangle(&brushBk, rcTarget);

	rcTarget.Inflate(nFeather, nFeather);
	Color colorBackGound(255, GetRValue(colorBackGround), GetGValue(colorBackGround), GetBValue(colorBackGround));
	Color colorCenter(colorHighLight);
	Color colorSurrounds[] = { colorBackGround };
	INT nColorCnt = _countof(colorSurrounds);

	GraphicsPath path;
	path.AddEllipse(rcTarget);

	PathGradientBrush brush(&path);
	brush.SetSurroundColors(colorSurrounds, &nColorCnt);
	brush.SetCenterPoint(PointF((rcRect.Width() / 2), (rcRect.Height() / 2)));
	brush.SetCenterColor(colorCenter);

	mGraphics.FillPath(&brush, &path);

	CachedBitmap cachedBitmap(&mBitmap, &graphics);
	graphics.DrawCachedBitmap(&cachedBitmap, 0, 0);

	return 0;
}

INT32 CGdiPlusHelper::DrawBall(HDC hDC, RectF& rcRect, COLORREF colorBall, CString strText, BOOL bFlat)
{
	if (hDC == NULL)
		return -1;

	Graphics graphics(hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQuality);

	float const nMargin = 0;
	RectF rcTarget(rcRect.X, rcRect.Y, (float)rcRect.Width + nMargin, (float)rcRect.Height + nMargin);
	GraphicsPath path;
	path.AddEllipse(rcTarget);

	if (bFlat)
	{
		SolidBrush brush(Color(255, GetRValue(colorBall), GetGValue(colorBall), GetBValue(colorBall)));
		graphics.FillPath(&brush, &path);
	}
	else
	{
		PathGradientBrush brush(&path);
		Color colorBackGound(255, GetRValue(colorBall), GetGValue(colorBall), GetBValue(colorBall));

		Color colorCenter(Color(255, 255, 255, 255));
		Color colorSurrounds[] = { colorBackGound };
		INT nColorCnt = _countof(colorSurrounds);

		brush.SetSurroundColors(colorSurrounds, &nColorCnt);
		brush.SetCenterPoint(PointF((rcRect.Width / 2), (rcRect.Height / 2)));
		brush.SetCenterColor(colorCenter);
		graphics.FillPath(&brush, &path);
	}

	
	for (MapSize::const_reverse_iterator itor = m_mapSize.rbegin() ; itor != m_mapSize.rend() ; ++itor)
	{
		RectF rect = itor->second;

		if (rect.Width < rcRect.Width && rect.Height < rcRect.Height)
		{
			m_pTextFont = new Gdiplus::Font(DEFAULT_FONT, itor->first);
			break;
		}
	}

	if (m_pTextFont)
	{
		//RectF layoutRect(0.0f, 0.0f, 32767.0f, 32767.0f);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		//RectF boundRect;
		//graphics.DrawString(strText)

		SolidBrush  solidBrush(Color(255, 0, 0, 0));
		graphics.DrawString(strText, strText.GetLength(), m_pTextFont, rcRect, &format, &solidBrush);
	}

	return 0;
}

INT32 CGdiPlusHelper::SetTransform(LPCTSTR lpszPath, Bitmap** ppBitmap)
{
	INT32 nRet = -1;

	*ppBitmap = Bitmap::FromFile(lpszPath);
	if ((*ppBitmap)->GetLastStatus() != Ok)
		return nRet;

	UINT uBufferSize = 0, uPropertyCount = 0;
	(*ppBitmap)->GetPropertySize(&uBufferSize, &uPropertyCount);

	PropertyItem* pItems = (PropertyItem*)malloc(uBufferSize);
	(*ppBitmap)->GetAllPropertyItems(uBufferSize, uPropertyCount, pItems);


	int const nPropertyId = 0x0112;
	short uOrient = 0;
	for (UINT uIndex = 0; uIndex < uPropertyCount; ++uIndex)
	{
		if (pItems[uIndex].id != nPropertyId)
			continue;

		{
			memcpy(&uOrient, pItems[uIndex].value, sizeof(uOrient));

			if (uOrient == 8) (*ppBitmap)->RotateFlip(Rotate270FlipNone);
			if (uOrient == 3) (*ppBitmap)->RotateFlip(Rotate180FlipNone);
			if (uOrient == 6) (*ppBitmap)->RotateFlip(Rotate90FlipNone);

			if (uOrient == 2) (*ppBitmap)->RotateFlip(RotateNoneFlipX);
			if (uOrient == 4) (*ppBitmap)->RotateFlip(Rotate180FlipX);
			if (uOrient == 5) (*ppBitmap)->RotateFlip(Rotate270FlipY);
			if (uOrient == 7) (*ppBitmap)->RotateFlip(Rotate90FlipY);

			break;
		}
	}

	if (uOrient >= 2 && uOrient <= 8)
	{
		(*ppBitmap)->RemovePropertyItem(nPropertyId);
		nRet = 0;
	}
	else
	{
		delete (*ppBitmap);
		(*ppBitmap) = NULL;
	}

	if (pItems)
	{
		free(pItems);
		pItems = NULL;
	}

	return nRet;
}

int CGdiPlusHelper::GetEncoderClsid(const WCHAR* pszFormat, CLSID* pClsid)
{
	UINT  nNum = 0;          // number of image encoders
	UINT  nSize = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&nNum, &nSize);
	if (nSize == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(nSize));
	if (!pImageCodecInfo)
		return -1;

	GetImageEncoders(nNum, nSize, pImageCodecInfo);

	for (UINT nIndex = 0; nIndex < nNum; ++nIndex)
	{
		if (wcscmp(pImageCodecInfo[nIndex].MimeType, pszFormat) == 0)
		{
			*pClsid = pImageCodecInfo[nIndex].Clsid;
			free(pImageCodecInfo);
			return nIndex;  // Success
		}
	}

	free(pImageCodecInfo);
	pImageCodecInfo = NULL;
	return -1;  // Failure
}

INT32 CGdiPlusHelper::BufferBitblt(HDC hDC, CRect& rcTarget)
{
	if (hDC == NULL)
		return -1;

	Graphics graphics(hDC);
	//Bitmap mBitmap(rc.Width(), rc.Height());

	CachedBitmap cachedBitmap(m_pBufferBitmap, &graphics);
	graphics.DrawCachedBitmap(&cachedBitmap, rcTarget.left, rcTarget.top);

	if (m_pBufferBitmap != NULL)
	{
		delete m_pBufferBitmap;
		m_pBufferBitmap = NULL;
	}

	return 0;
}

INT32 CGdiPlusHelper::BufferDrawGradientBackGound(CRect& rcRect, COLORREF colorBackGround, Color colorHighLight, INT32 nFeather)
{
	if (m_pBufferBitmap == NULL)
		m_pBufferBitmap = new Bitmap(rcRect.Width(), rcRect.Height());

	Graphics mGraphics(m_pBufferBitmap);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.SetInterpolationMode(InterpolationModeHighQuality);

	float const nMargin = 2;
	RectF rcTarget(-nMargin, -nMargin, (float)rcRect.Width() + nMargin, (float)rcRect.Height() + nMargin);

	SolidBrush brushBk(Color(255, GetRValue(colorBackGround), GetGValue(colorBackGround), GetBValue(colorBackGround)));
	mGraphics.FillRectangle(&brushBk, rcTarget);

	rcTarget.Inflate(nFeather, nFeather);
	Color colorBackGound(255, GetRValue(colorBackGround), GetGValue(colorBackGround), GetBValue(colorBackGround));
	Color colorCenter(colorHighLight);
	Color colorSurrounds[] = { colorBackGround };
	INT nColorCnt = _countof(colorSurrounds);

	GraphicsPath path;
	path.AddEllipse(rcTarget);

	PathGradientBrush brush(&path);
	brush.SetSurroundColors(colorSurrounds, &nColorCnt);
	brush.SetCenterPoint(PointF((rcRect.Width() / 2), (rcRect.Height() / 2)));
	brush.SetCenterColor(colorCenter);

	mGraphics.FillPath(&brush, &path);

	//CachedBitmap cachedBitmap(&mBitmap, &graphics);
	//graphics.DrawCachedBitmap(&cachedBitmap, 0, 0);

	return 0;
}

INT32 CGdiPlusHelper::BufferDrawBall(RectF& rcRect, COLORREF colorBall, CString strText, BOOL bFlat)
{
	if (m_pBufferBitmap == NULL)
		return -1;

	Graphics graphics(m_pBufferBitmap);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQuality);

	float const nMargin = 0;
	RectF rcTarget(rcRect.X, rcRect.Y, (float)rcRect.Width + nMargin, (float)rcRect.Height + nMargin);
	GraphicsPath path;
	path.AddEllipse(rcTarget);

	if (bFlat)
	{
		SolidBrush brush(Color(255, GetRValue(colorBall), GetGValue(colorBall), GetBValue(colorBall)));
		graphics.FillPath(&brush, &path);
	}
	else
	{
		PathGradientBrush brush(&path);
		Color colorBackGound(255, GetRValue(colorBall), GetGValue(colorBall), GetBValue(colorBall));

		Color colorCenter(Color(255, 255, 255, 255));
		Color colorSurrounds[] = { colorBackGound };
		INT nColorCnt = _countof(colorSurrounds);

		brush.SetSurroundColors(colorSurrounds, &nColorCnt);
		brush.SetCenterPoint(PointF((rcRect.Width / 2), (rcRect.Height / 2)));
		brush.SetCenterColor(colorCenter);
		graphics.FillPath(&brush, &path);
	}


	for (MapSize::const_reverse_iterator itor = m_mapSize.rbegin() ; itor != m_mapSize.rend() ; ++itor)
	{
		RectF rect = itor->second;

		if (rect.Width < rcRect.Width && rect.Height < rcRect.Height)
		{
			m_pTextFont = new Gdiplus::Font(DEFAULT_FONT, itor->first);
			break;
		}
	}

	if (m_pTextFont)
	{
		//RectF layoutRect(0.0f, 0.0f, 32767.0f, 32767.0f);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		//RectF boundRect;
		//graphics.DrawString(strText)

		SolidBrush  solidBrush(Color(255, 0, 0, 0));
		graphics.DrawString(strText, strText.GetLength(), m_pTextFont, rcRect, &format, &solidBrush);
	}

	return 0;
}

INT32 CGdiPlusHelper::BufferDrawContainer(CRect& rcRect, PointF center, float radius)
{
	if (m_pBufferBitmap == NULL)
		m_pBufferBitmap = new Bitmap(rcRect.Width(), rcRect.Height());

	Graphics graphics(m_pBufferBitmap);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	GraphicsPath path;
	path.AddEllipse(center.X - radius, center.Y - radius, radius * 2, radius * 2);

	PathGradientBrush pgb(&path);
	Color centerColor(255, 220, 220, 220);
	Color surroundColor(255, 80, 80, 80);
	pgb.SetCenterColor(centerColor);
	Color surroundColors[1] = { surroundColor };
	INT nEdgeWidth = 1;
	pgb.SetSurroundColors(surroundColors, &nEdgeWidth);

	graphics.FillPath(&pgb, &path);

	Pen pen(Color(255, 150, 150, 150), 2);
	graphics.DrawPath(&pen, &path);

	return 0;
}
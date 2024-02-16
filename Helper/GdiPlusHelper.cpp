#include "StdAfx.h"

#include "GdiPlusHelper.h"

#pragma comment(lib, "gdiplus")

CGdiPlusHelper::CGdiPlusHelper()
{
	m_gdiToken = 0;
}

CGdiPlusHelper::~CGdiPlusHelper()
{
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

	return TRUE;
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
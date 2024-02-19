/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: BallItem class
// CBallItem class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "BallItem.h"

#include "../Helper/PaserUtil.h"

#define _CRT_SECURE_NO_WARNINGS

CBallItem::CBallItem()
{
	SetEmpty();
}

CBallItem::~CBallItem()
{
}

void CBallItem::SetEmpty()
{
	m_dwRound = 0;
	m_strDate.Empty();
	m_mapNumbers.clear();
}

INT32 CBallItem::Parse(CString strRaw, CString strDelimeter)
{
	CStringArray arr;
	CPaserUtil::ParseString(strRaw, strDelimeter, arr);

	INT32 nIndex = 0;
	m_dwRound = _ttol(arr.GetAt(nIndex++));
	m_strDate = arr.GetAt(nIndex++);

	for(INT32 nKey =  0 ; nIndex < arr.GetCount() ; nIndex++, nKey++)
		m_mapNumbers.insert(make_pair(nKey, _ttoi(arr.GetAt(nIndex))));

	return 0;
}

INT32	CBallItem::GetWinNumbers(INT32 nIndex)
{
	return m_mapNumbers[nIndex];
}

INT32	CBallItem::SetWinNumbers(CString strNumber)
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: WinsItem class
// CWinsItem class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "WinsItem.h"

#include "../Helper/PaserUtil.h"

#define _CRT_SECURE_NO_WARNINGS

CWinsItem::CWinsItem()
{
	SetEmpty();
}

CWinsItem::~CWinsItem()
{
}

void CWinsItem::SetEmpty()
{
	m_dwRound = 0;
	m_nSumWithBonus = m_nSum = 0;
	m_strDate.Empty();
	m_mapNumbers.clear();
	m_nYear = m_nMonth = m_nDay = 0;
}

INT32 CWinsItem::Parse(CString strRaw, CString strDelimeter)
{
	CStringArray arr;
	CPaserUtil::ParseString(strRaw, strDelimeter, arr);

	INT32 nIndex = 0;
	m_dwRound = _ttol(arr.GetAt(nIndex++));
	m_strDate = arr.GetAt(nIndex++);

	for(INT32 nKey =  0 ; nIndex < arr.GetCount() ; nIndex++, nKey++)
		m_mapNumbers.insert(make_pair(nKey, _ttoi(arr.GetAt(nIndex))));

	return _ParseDate(m_strDate);
}

INT32 CWinsItem::_ParseDate(CString& strDate)
{
	_stscanf_s(strDate, _T("%d.%d.%d"), &m_nYear, &m_nMonth, &m_nDay);

	COleDateTime dt(m_nYear, m_nMonth, m_nDay, 0, 0, 0);
	if (dt.GetStatus() == COleDateTime::valid)
		return 0;
	else 
		return 1;
}

INT32	CWinsItem::GetWinNumbers(INT32 nIndex)
{
	return m_mapNumbers[nIndex];
}
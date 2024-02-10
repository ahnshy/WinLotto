/////////////////////////////////////////////////////////////////////////
// #20120227 Work Ahnshy :: Paser Utility
// CPaserUtil class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "PaserUtil.h"

#define _CRT_SECURE_NO_WARNINGS

BOOL CPaserUtil::ParseString(IN CString strRaw, IN CString strDelimeter, OUT CStringArray& arOut)
{
	arOut.RemoveAll();
	if (strRaw.IsEmpty() || strDelimeter.IsEmpty())
		return FALSE;

	TCHAR	szCopy[2048] = { NULL, };
	TCHAR*	pszToken = NULL;
	TCHAR*	pszDelimeter = NULL;
	CString strExt;

	_tcscpy_s(szCopy, 2048, strRaw);
	pszDelimeter = strDelimeter.GetBuffer(strDelimeter.GetLength());

	pszToken = _tcstok(szCopy, strDelimeter);
	while( pszToken != NULL)
	{
		arOut.Add(pszToken);
		pszToken = _tcstok(NULL, strDelimeter);
	}

	return TRUE;
}

BOOL CPaserUtil::HasString(IN CString strRaw, OUT CStringArray& strList)
{
	CString strBuffer;
	for (int nIdx = 0 ; nIdx < strList.GetSize() ; nIdx++)
	{
		strBuffer = strList.GetAt(nIdx);
		strBuffer.Trim();
		if (!strRaw.CompareNoCase(strBuffer))
			return TRUE;
	}

	return FALSE;
}

BOOL CPaserUtil::IsEqualDate(COleDateTime& dateDest1, COleDateTime& dateDest2)
{
	if (dateDest1.GetStatus() == COleDateTime::invalid || dateDest2.GetStatus() == COleDateTime::invalid)
		return FALSE;

	if (dateDest1.GetYear() == dateDest2.GetYear() &&
		dateDest1.GetMonth() == dateDest2.GetMonth() && 
		dateDest1.GetDay() == dateDest2.GetDay())
		return TRUE;

	return FALSE;
}

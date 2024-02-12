/////////////////////////////////////////////////////////////////////////
// #20120227 Work Ahnshy :: Ini Utility
// CIniUtil class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "IniUtil.h"

BOOL CIniUtil::GetBoolean(LPCTSTR lpStr)
{
	if (lpStr == NULL || lpStr[0] == NULL)
		return FALSE;

	if (!_tcsicmp(lpStr, _T("true")) || !_tcsicmp(lpStr, _T("yes")))
		return TRUE;
	else if (!_tcsicmp(lpStr, _T("false")) || !_tcsicmp(lpStr, _T("no")))
		return FALSE;
	
	return FALSE;
}

COLORREF CIniUtil::GetColorRef(DWORD dwColor)
{
	return RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
}

COLORREF CIniUtil::GetColorRef(CString strValue)
{
	return RGB(_ttoi(strValue.Left(3)), _ttoi(strValue.Mid(4,3)), _ttoi(strValue.Right(3)));
}

CString CIniUtil::Color2String(COLORREF clr)
{
	CString strRet;
	strRet.Format(_T("%03d:%03d:%03d"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
	return strRet;
}

CString CIniUtil::Boolean2String(BOOL bVal)
{
	CString strRet;
	if (bVal)
		strRet.FormatMessage(_T("true"));
	else
		strRet.FormatMessage(_T("false"));
	
	return strRet;
}
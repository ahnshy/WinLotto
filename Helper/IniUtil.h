#pragma once

/////////////////////////////////////////////////////////////////////////
// #20120227 Work Ahnshy :: Ini Utility
// CIniUtil class
// Bug Report :: ahnshy@naver.com

class CIniUtil
{
public:
	static	BOOL			GetBoolean(LPCTSTR lpStr);
	static	COLORREF		GetColorRef(DWORD dwColor);
	static	COLORREF		GetColorRef(CString strValue);
	static	CString			Color2String(COLORREF clr);
	static	CString			Boolean2String(BOOL bVal);
};
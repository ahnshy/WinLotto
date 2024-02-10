#pragma once

/////////////////////////////////////////////////////////////////////////
// #20120227 Work Ahnshy :: Paser Utility
// CPaserUtil class
// Bug Report :: ahnshy@naver.com

class CPaserUtil
{
public:
	static	BOOL			HasString(IN CString strRaw, OUT CStringArray& pArray);
	static	BOOL			IsEqualDate(COleDateTime& dateDest1, COleDateTime& dateDest2);
	static	BOOL			ParseString(IN CString strRaw, IN CString strDelimeter, OUT CStringArray &arOut);
};
#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: WinsItem class
// CWinsItem class
// Bug Report :: ahnshy@naver.com
#include <map>
using namespace std;


typedef map<DWORD, INT32>				MapWinsNumber;

class CWinsItem
{
public:
	CWinsItem();
	~CWinsItem();

	void	SetEmpty();

	DWORD	GetRound()				{ return m_dwRound; }
	CString	GetDate()				{ return m_strDate; }
	INT32	GetNumberCount()		{ return m_mapNumbers.size(); }

	INT32	Parse(CString strRaw, CString strDelimeter);
	INT32	GetWinNumbers(INT32 nIndex);

	INT32	SetWinNumbers(CString strNumber);

private:
	DWORD				m_dwRound;
	CString				m_strDate;
	MapWinsNumber		m_mapNumbers;
};
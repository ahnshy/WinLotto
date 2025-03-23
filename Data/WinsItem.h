#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: WinsItem class
// CWinsItem class
// Bug Report :: ahnshy@naver.com
#include <set>
#include <map>
using namespace std;

typedef set<UINT32>						SetWinsYears;
typedef map<DWORD, INT32>				MapWinsNumber;

class CWinsItem
{
public:
	CWinsItem();
	~CWinsItem();

	void				SetEmpty();

	DWORD				GetRound()						{ return m_dwRound; }
	CString				GetDate()						{ return m_strDate; }
	INT32				GetYear()						{ return m_nYear;	}
	INT32				GetMonth()						{ return m_nMonth;	}
	INT32				GetDay()						{ return m_nDay;	}
	INT32				GetNumberCount()				{ return m_mapNumbers.size(); }
	SetWinsYears&		GetYearSet()					{ return m_setYears;}
	MapWinsNumber&		GetNumberMap()					{ return m_mapNumbers; }
	INT32				GetSum()						{ return m_nSum; }
	INT32				GetSumWithBonus()				{ return m_nSumWithBonus; }

	INT32				Parse(CString strRaw, CString strDelimeter);
	INT32				GetWinNumbers(INT32 nIndex);

	void				SetRound(DWORD dwRound)			{ m_dwRound = dwRound; }
	void				SetDate(CString strDate)		{ m_strDate = strDate; }

	void				SetTotal(DWORD dwSum)			{ m_nSum = dwSum; }
	void				SetTotalWithBonus(DWORD dwSum)	{ m_nSumWithBonus = dwSum; }

private:
	INT32				_ParseDate(CString& strDate);

private:
	DWORD				m_dwRound;
	CString				m_strDate;
	MapWinsNumber		m_mapNumbers;
	SetWinsYears		m_setYears;

	INT32				m_nSum;
	INT32				m_nSumWithBonus;
	
	INT32				m_nYear;
	INT32				m_nMonth;
	INT32				m_nDay;
};
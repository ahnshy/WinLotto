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

	void				SetEmpty();

	DWORD				GetRound()						{ return m_dwRound; }
	CString				GetDate()						{ return m_strDate; }
	INT32				GetNumberCount()				{ return m_mapNumbers.size(); }
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
	DWORD				m_dwRound;
	CString				m_strDate;
	MapWinsNumber		m_mapNumbers;

	INT32				m_nSum;
	INT32				m_nSumWithBonus;
};
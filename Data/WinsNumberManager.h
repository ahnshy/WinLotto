#pragma once

/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class

#include "WinsItem.h"

#include <map>
#include <vector>
#include <algorithm>

typedef pair<int, int> pairDataType;

#define MAX_BALLS				45

typedef map<DWORD, CWinsItem*>				MapRounds;
typedef map<INT32, INT32>					MapFrequency;
typedef map<INT32, MapFrequency*>			MapFrequencyPerDate;

//typedef vector<pairDataType>	vectorFrequency;

class CWinsNumberManager
{
public:
	CWinsNumberManager(void);
	~CWinsNumberManager(void);

	static CWinsNumberManager*	GetInstance();
	static void					DestroyInstance();

	// Init
	INT32						Initialize(CStringArray& arrRounds);
	INT32						SortFrequncyRanking();

	void						SaveConfig();
	void						ReadConfig();

	// Getter / Setter
	MapRounds&					GetRoundMap()					{ return m_mapRounds; }
	const vector<pairDataType>*	GetFrequencyVector()			{ return m_pFrequency; }
	const vector<pairDataType>*	GetFrequencyBonusRoundVector()	{ return m_pFrequencyWithBonus; }
	MapFrequencyPerDate&		GetFrequencyPerMonthMap()		{ return m_mapFrequencyPerMonth; }

	DWORD						GetTotalCount()					{ return m_dwTotalCount; }

	void						RemoveAll();

protected:
	DWORD						m_dwTotalCount;
	MapRounds					m_mapRounds;
	MapFrequency				m_mapFrequency;
	MapFrequency				m_mapFrequencyWithBonus;

	MapFrequencyPerDate			m_mapFrequencyPerYear;
	MapFrequencyPerDate			m_mapFrequencyPerMonth;
	MapFrequencyPerDate			m_mapFrequencyPerDay;
	
	vector<pairDataType>*		m_pFrequency;
	vector<pairDataType>*		m_pFrequencyWithBonus;

	static CWinsNumberManager*	m_pWinsNumberManager;
};

enum eMonth
{
	JANUARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER,
	MAX_MONTH = DECEMBER
};
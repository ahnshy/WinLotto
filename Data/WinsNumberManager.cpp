/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class

/////////////////////////////////////////////////////////////////////////////
// Architecture - will be updated
//
//

#include "StdAfx.h"

//#include <algorithm>

#include "WinsNumberManager.h"

#include "../Helper/IniUtil.h"
#include "../Helper/PaserUtil.h"

// FileInfo
#define CONFIG_WINLOTTO_FILENAME							TEXT("WinLotto.ini")
#define CONFIG_SECTION_CONFIG								TEXT("Config")

//bool fnCompare(const <INT32, INT32>& a, const <INT32, INT32>& b)
//{
//	if (a.second == b.second) return a.first < b.first;
		//return a.second < b.second;
//}

CWinsNumberManager*	CWinsNumberManager::m_pWinsNumberManager= NULL;
/////////////////////////////////////////////////////////////////////////////
// Management Instance
CWinsNumberManager* CWinsNumberManager::GetInstance()
{
	if (m_pWinsNumberManager != NULL)
		return m_pWinsNumberManager;

	m_pWinsNumberManager = new CWinsNumberManager;
	return m_pWinsNumberManager;
}

void CWinsNumberManager::DestroyInstance()
{
	if (NULL == m_pWinsNumberManager)
		return;

	delete m_pWinsNumberManager;
	m_pWinsNumberManager = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWinsNumberManager
CWinsNumberManager::CWinsNumberManager(void)
{	
	m_dwTotalCount = 0;
	m_pFrequency = m_pFrequencyWithBonus = NULL;

	RemoveAll();

	if (m_pWinsNumberManager != NULL)
		ASSERT(FALSE);
	else
		m_pWinsNumberManager = this;
}

CWinsNumberManager::~CWinsNumberManager(void)
{
	RemoveAll();
}

void CWinsNumberManager::RemoveAll()
{
	for (MapRounds::iterator itor = m_mapRounds.begin() ; itor != m_mapRounds.end() ; ++itor)	
		delete itor->second;

	for (MapFrequencyPerDate::iterator itor = m_mapFrequencyPerYear.begin(); itor != m_mapFrequencyPerYear.end(); ++itor)
		delete itor->second;

	for (MapFrequencyPerDate::iterator itor = m_mapFrequencyPerMonth.begin(); itor != m_mapFrequencyPerMonth.end(); ++itor)
		delete itor->second;

	for (MapFrequencyPerDate::iterator itor = m_mapFrequencyPerDay.begin(); itor != m_mapFrequencyPerDay.end(); ++itor)
		delete itor->second;

	m_mapRounds.clear();
	m_mapFrequency.clear();
	m_mapFrequencyWithBonus.clear();
	
	m_mapFrequencyPerYear.clear();
	m_mapFrequencyPerMonth.clear();
	m_mapFrequencyPerDay.clear();


	if (m_pFrequency)
	{
		m_pFrequency->clear();
		delete m_pFrequency;
		m_pFrequency = NULL;
	}

	if (m_pFrequencyWithBonus)
	{
		m_pFrequencyWithBonus->clear();
		delete m_pFrequencyWithBonus;
		m_pFrequencyWithBonus = NULL;
	}
}

CString CWinsNumberManager::GetLastestDate()
{
	CString strResult(_T(""));
	if (m_mapRounds.empty())
		return strResult;

	auto it = m_mapRounds.rbegin();
	return it->second->GetDate();
}

void CWinsNumberManager::ReadConfig()
{
	// to do...
}

void CWinsNumberManager::SaveConfig()
{
	// to do...
}

INT32 CWinsNumberManager::Initialize(CStringArray& arrRounds)
{
	CString strBuffer;
	CWinsItem *pItem = NULL;
	CStringArray arr;
	const CString strDelimeter = _T(",");
	INT32 nSum = 0, nSumWithBonus = 0;
	for (INT32 nBallCount = 0; nBallCount <= MAX_BALLS; nBallCount++)
		m_mapFrequencyWithBonus[nBallCount] = m_mapFrequency[nBallCount] = 0;

	MapFrequency* pFrequncyMonthly = NULL;
	for (INT32 nMonth = JANUARY; nMonth <= MAX_MONTH; nMonth++)
	{
		pFrequncyMonthly = new MapFrequency();
		if (pFrequncyMonthly)
		{
			for (INT32 nBallCount = 1; nBallCount <= MAX_BALLS; nBallCount++)
				pFrequncyMonthly->insert(make_pair(nBallCount, 0));

			m_mapFrequencyPerMonth.insert(make_pair(nMonth, pFrequncyMonthly));
		}
	}

	MapFrequency* pFrequencyPerDay = NULL;
	for (INT32 nDays = 1; nDays <= MAX_DAYS; nDays++)
	{
		pFrequencyPerDay = new MapFrequency();
		if (pFrequencyPerDay)
		{
			for (INT32 nBallCount = 1; nBallCount <= MAX_BALLS; nBallCount++)
				pFrequencyPerDay->insert(make_pair(nBallCount, 0));

			m_mapFrequencyPerDay.insert(make_pair(nDays, pFrequencyPerDay));
		}
	}

	for(INT32 nIndex =  0 ; nIndex < arrRounds.GetCount() ; nIndex++)
	{
		strBuffer = arrRounds.GetAt(nIndex);
		if (strBuffer.IsEmpty())
			continue;

		pItem = new CWinsItem();
		if (pItem)
		{
			pItem->Parse(strBuffer, _T(","));
			m_mapRounds.insert(make_pair(pItem->GetRound(), pItem));
			
			pFrequncyMonthly = m_mapFrequencyPerMonth[pItem->GetMonth()];
			pFrequencyPerDay = m_mapFrequencyPerDay[pItem->GetDay()];

			nSumWithBonus = nSum = 0;
			for (MapWinsNumber::iterator itor = pItem->GetNumberMap().begin(); itor != pItem->GetNumberMap().end(); ++itor)
			{
				m_mapFrequencyWithBonus[itor->second]++;
				nSumWithBonus += itor->second;
				m_dwTotalCount++;

				if (pFrequncyMonthly)
					(*pFrequncyMonthly)[itor->second]++;

				if (pFrequencyPerDay)
					(*pFrequencyPerDay)[itor->second]++;

				if (itor->first == 6)
					continue;

				m_mapFrequency[itor->second]++;
				nSum += itor->second;
			}

			pItem->SetTotal(nSum);
			pItem->SetTotalWithBonus(nSumWithBonus);
		}
	}

	SortFrequncyRanking();
	return 0;
}

INT32 CWinsNumberManager::SortFrequncyRanking()
{
	if (m_pFrequency)
	{
		m_pFrequency->clear();
		delete m_pFrequency;
		m_pFrequency = NULL;
	}

	{
		m_pFrequency = new vector<pairDataType>(m_mapFrequency.begin(), m_mapFrequency.end());
		sort(m_pFrequency->begin(), m_pFrequency->end(), [](pairDataType nBefore, pairDataType nAfter){
			return nBefore.second > nAfter.second;
		});
	}

	if (m_pFrequencyWithBonus)
	{
		m_pFrequencyWithBonus->clear();
		delete m_pFrequencyWithBonus;
		m_pFrequencyWithBonus = NULL;   
	}

	{
		m_pFrequencyWithBonus = new vector<pairDataType>(m_mapFrequencyWithBonus.begin(), m_mapFrequencyWithBonus.end());
		sort(m_pFrequencyWithBonus->begin(), m_pFrequencyWithBonus->end(), [](pairDataType nBefore, pairDataType nAfter){
			return nBefore.second > nAfter.second;
		});
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class

/////////////////////////////////////////////////////////////////////////////
// Architecture - will be updated
//
//

#include "StdAfx.h"
#include "WinsNumberManager.h"

#include "../Helper/IniUtil.h"
#include "../Helper/PaserUtil.h"

// FileInfo
#define CONFIG_WINLOTTO_FILENAME							TEXT("WinLotto.ini")
#define CONFIG_SECTION_CONFIG								TEXT("Config")

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
	SetEmpty();

	if (m_pWinsNumberManager != NULL)
		ASSERT(FALSE);
	else
		m_pWinsNumberManager = this;
}

CWinsNumberManager::~CWinsNumberManager(void)
{
	SetEmpty();
}

void CWinsNumberManager::SetEmpty()
{
	for (MapRounds::iterator itor = m_mapRounds.begin() ; itor != m_mapRounds.end() ; ++itor)
		delete itor->second;

	m_mapRounds.clear();
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
		}
	}

	return 0;
}
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
	// to do...
}

void CWinsNumberManager::ReadConfig()
{
	// to do...
}

void CWinsNumberManager::SaveConfig()
{
	// to do...
}

INT32 CWinsNumberManager::Initialize(CString strRaw, CString strDelimeter)
{
	CStringArray arr;
	CPaserUtil::ParseString(strRaw, strDelimeter, arr);

	//INT32 nIndex = 0;
	//m_dwRound = _ttol(arr.GetAt(nIndex++));
	//m_strDate = arr.GetAt(nIndex++);

	//for(INT32 nKey =  0 ; nIndex < arr.GetCount() ; nIndex++, nKey++)
	//	m_mapRounds.insert(make_pair(nKey, _ttoi(arr.GetAt(nIndex))));

	return 0;
}
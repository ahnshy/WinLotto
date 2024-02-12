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

// FileInfo
#define CONFIG_WINLOTTO_FILENAME							TEXT("WinLotto.ini")
#define CONFIG_SECTION_CONFIG								TEXT("Config")

#define CONFIG_WINLOTTO_PATH								TEXT("path")

// About KeyString
static const CString	PathName;

// Static :: Magnify Init
const CString CWinsNumberManager::PathName						= CONFIG_WINLOTTO_PATH;

CWinsNumberManager*	CWinsNumberManager::m_pWinsNumberManager	= NULL;
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

void CWinsNumberManager::Initialize()
{	
	// to do...
}
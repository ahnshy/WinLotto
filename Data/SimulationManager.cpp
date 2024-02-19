/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: Simulation Manager Manage Class
// CSimulationManager class

/////////////////////////////////////////////////////////////////////////////
// Architecture - will be updated
//
//

#include "StdAfx.h"
#include "SimulationManager.h"

#include "../Helper/IniUtil.h"
#include "../Helper/PaserUtil.h"

CSimulationManager*	CSimulationManager::m_pSimulationManager= NULL;
/////////////////////////////////////////////////////////////////////////////
// Management Instance
CSimulationManager* CSimulationManager::GetInstance()
{
	if (m_pSimulationManager != NULL)
		return m_pSimulationManager;

	m_pSimulationManager = new CSimulationManager;
	return m_pSimulationManager;
}

void CSimulationManager::DestroyInstance()
{
	if (NULL == m_pSimulationManager)
		return;

	delete m_pSimulationManager;
	m_pSimulationManager = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CSimulationManager
CSimulationManager::CSimulationManager(void)
{
	SetEmpty();

	if (m_pSimulationManager != NULL)
		ASSERT(FALSE);
	else
		m_pSimulationManager = this;
}

CSimulationManager::~CSimulationManager(void)
{
	SetEmpty();
}

void CSimulationManager::SetEmpty()
{
//	for (MapRounds::iterator itor = m_mapRounds.begin() ; itor != m_mapRounds.end() ; ++itor)
		//delete itor->second;

	//m_mapRounds.clear();
}

void CSimulationManager::ReadConfig()
{
	// to do...
}

void CSimulationManager::SaveConfig()
{
	// to do...
}

INT32 CSimulationManager::Initialize()
{
	return 0;
}
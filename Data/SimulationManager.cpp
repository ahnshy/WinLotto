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

#define MAX_BALL_COUNT										45

const INT32 CSimulationManager::m_nMaxBalls				= MAX_BALL_COUNT;

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
	//for (MapBalls::iterator itor = m_mapSimulationWinBalls.begin() ; itor != m_mapSimulationWinBalls.end() ; ++itor)
		//delete itor->second;

	m_mapSimulationWinBalls.clear();
	m_mapRemainBalls.clear();
}

INT32 CSimulationManager::Initialize()
{
	SetEmpty();

	for (INT32 nNumber = 1 ; nNumber <= CSimulationManager::m_nMaxBalls ; nNumber++)
	{
		if (nNumber / 10 == 0)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, RGB(251, 196, 0)));
		else if (nNumber / 10 == 1)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, RGB(105, 200, 242)));
		else if (nNumber / 10 == 2)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, RGB(255, 114, 114)));
		else if (nNumber / 10 == 3)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, RGB(170, 170, 170)));
		else if (nNumber / 10 == 4)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, RGB(176, 216, 64)));
		else
		{
			// to do....
		}
	}

	return 0;
}
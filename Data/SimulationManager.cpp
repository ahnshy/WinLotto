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
	for (MapBalls::iterator itor = m_mapSimulationWinBalls.begin() ; itor != m_mapSimulationWinBalls.end() ; ++itor)
		delete itor->second;

	m_mapSimulationWinBalls.clear();
	m_mapRemainBalls.clear();
}

INT32 CSimulationManager::Initialize(CRect& rc)
{
	SetEmpty();

	double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;
	double fMargin = fDiameter;

	for (INT32 nNumber = 1 ; nNumber <= CSimulationManager::m_nMaxBalls ; nNumber++)
	{	
		RectF rcInit((((nNumber % 10)*fDiameter) +nNumber +fMargin), fMargin + (((nNumber / 10)) * fDiameter*2), fDiameter, fDiameter);
		if (nNumber / 10 == 0)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, new CBallItem(rcInit, RGB(251, 196, 0))));
		else if (nNumber / 10 == 1)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, new CBallItem(rcInit, RGB(105, 200, 242))));
		else if (nNumber / 10 == 2)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, new CBallItem(rcInit, RGB(255, 114, 114))));
		else if (nNumber / 10 == 3)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, new CBallItem(rcInit, RGB(170, 170, 170))));
		else if (nNumber / 10 == 4)
			m_mapSimulationWinBalls.insert(make_pair(nNumber, new CBallItem(rcInit, RGB(176, 216, 64))));
		else
		{
			// to do....
		}
	}

	return 0;
}

INT32 CSimulationManager::SetBallDeployment(CRect rc, INT32 nMargin)
{
	double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;
	//double fMargin = fDiameter;

	srand(time(NULL));
	rc.DeflateRect(nMargin, nMargin, nMargin, nMargin);
	for (MapBalls::iterator itor = m_mapSimulationWinBalls.begin() ; itor != m_mapSimulationWinBalls.end() ; ++itor)
	{
		RectF rcDeploy(rand()%rc.Width(), rand()%rc.Height(), fDiameter, fDiameter);
		itor->second->SetRect(rcDeploy);
		itor->second->GetDirection().cx = (rand()%7)+1;
		itor->second->GetDirection().cy = (rand()%7)+1;
	}
	
	return 0;
}

INT32 CSimulationManager::AdjustBallPos(CRect rc)
{
	double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;

	RectF rcBall;
	for (MapBalls::iterator itor = m_mapSimulationWinBalls.begin() ; itor != m_mapSimulationWinBalls.end() ; ++itor)
	{
		rcBall = itor->second->GetRect();
		if (rcBall.X < 0)
		{
			rcBall.X = rcBall.Width;
			itor->second->GetDirection().cx = abs(itor->second->GetDirection().cx);
		}
		else if (rcBall.X + rcBall.Width > rc.Width())
		{
			rcBall.X = rc.Width() - rcBall.Width*2;
			itor->second->GetDirection().cx = -abs(itor->second->GetDirection().cx);
		}

		if (rcBall.Y < 0)
		{
			rcBall.Y = rcBall.Height;
			itor->second->GetDirection().cy = -abs(itor->second->GetDirection().cy);
		}
		else if (rcBall.Y + rcBall.Height > rc.Height())
		{
			rcBall.Y = rc.Height() - rcBall.Height*2;
			itor->second->GetDirection().cy = abs(itor->second->GetDirection().cy);
		}

		//rcBall.X += itor->second->GetDirection().cx;
		//rcBall.Y += itor->second->GetDirection().cy;

		itor->second->SetRect(rcBall);
	}

	return 0;
}
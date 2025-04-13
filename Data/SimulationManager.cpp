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
	float centerX = rc.Width() / 2.0f;
	float centerY = rc.Height() / 2.0f;
	float containerRadius = min(rc.Width(), rc.Height()) / 2.0f - nMargin;

	srand((unsigned)time(NULL));
	rc.DeflateRect(nMargin, nMargin, nMargin, nMargin);
	for (MapBalls::iterator itor = m_mapSimulationWinBalls.begin(); itor != m_mapSimulationWinBalls.end(); ++itor)
	{
		bool bValid = false;
		int retry = 0;
		RectF rcDeploy;
		float posX, posY;

		while (!bValid && retry++ < 1000)
		{
			float theta = (rand() / (float)RAND_MAX) * 2 * 3.14159265f;
			float r = sqrt(rand() / (float)RAND_MAX) * (containerRadius - fDiameter / 2 - 2);
			posX = centerX + r * cos(theta) - fDiameter / 2;
			posY = centerY + r * sin(theta) - fDiameter / 2;
			rcDeploy = RectF(posX, posY, fDiameter, fDiameter);

			PointF c1(posX + fDiameter / 2.f, posY + fDiameter / 2.f);
			bValid = true;

			for (MapBalls::iterator check = m_mapSimulationWinBalls.begin(); check != itor; ++check)
			{
				RectF rcOther = check->second->GetRect();
				PointF c2(rcOther.X + rcOther.Width / 2.f, rcOther.Y + rcOther.Height / 2.f);

				float dx = c2.X - c1.X;
				float dy = c2.Y - c1.Y;
				float dist = sqrt(dx * dx + dy * dy);

				if (dist < fDiameter + 2.f)
				{
					bValid = false;
					break;
				}
			}
		}

		itor->second->SetRect(rcDeploy);

		float vx = ((rand() / (float)RAND_MAX) * 10.0f - 5.0f);
		float vy = ((rand() / (float)RAND_MAX) * 10.0f - 5.0f);

		itor->second->SetVelocity(PointF(vx, vy));
		itor->second->SetSleeping(false);
	}

	return 0;
}


INT32 CSimulationManager::AdjustBallPos(CRect rc)
{
	/*
	double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;

	RectF rcBall;
	for (MapBalls::iterator itor = m_mapSimulationWinBalls.begin() ; itor != m_mapSimulationWinBalls.end() ; ++itor)
	{
		rcBall = itor->second->GetRect();
		if (rcBall.X < 0)
		{
			rcBall.X = rcBall.Width;
			itor->second->GetVelocity().cx = abs(itor->second->GetVelocity().cx);
		}
		else if (rcBall.X + rcBall.Width > rc.Width())
		{
			rcBall.X = rc.Width() - rcBall.Width*2;
			itor->second->GetVelocity().cx = -abs(itor->second->GetVelocity().cx);
		}

		if (rcBall.Y < 0)
		{
			rcBall.Y = rcBall.Height;
			itor->second->GetVelocity().cy = -abs(itor->second->GetVelocity().cy);
		}
		else if (rcBall.Y + rcBall.Height > rc.Height())
		{
			rcBall.Y = rc.Height() - rcBall.Height*2;
			itor->second->GetVelocity().cy = abs(itor->second->GetVelocity().cy);
		}

		//rcBall.X += itor->second->GetDirection().cx;
		//rcBall.Y += itor->second->GetDirection().cy;

		itor->second->SetRect(rcBall);
	}
	*/

	return 0;
}
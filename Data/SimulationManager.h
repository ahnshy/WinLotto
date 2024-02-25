#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: Winning Number Manage Class
// CSimulationManager class

#include "BallItem.h"
#include <map>
using namespace std;

typedef map<DWORD, CBallItem*>	MapBalls;

class CSimulationManager
{
public:
	CSimulationManager(void);
	~CSimulationManager(void);

	static CSimulationManager*	GetInstance();
	static void					DestroyInstance();

	// Init
	INT32						Initialize(CRect& rc);

	// Getter / Setter
	MapBalls&					GetBalls()			{		return m_mapSimulationWinBalls;		}
	
	void						SetEmpty();
	INT32						SetBallDeployment(CRect rc, INT32 nMargin);

	INT32						AdjustBallPos(CRect rc);

protected:
	MapBalls					m_mapRemainBalls;
	MapBalls					m_mapSimulationWinBalls;

	static const INT32			m_nMaxBalls;

	static CSimulationManager*	m_pSimulationManager;
};
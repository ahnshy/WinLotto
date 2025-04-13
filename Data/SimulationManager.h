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
	//void						SetBallDeployment(const CRect& rcClient, int numBalls);
	INT32						SetBallDeployment(CRect rc, INT32 nMargin);
	COLORREF					GetBallColor(int number);

	INT32						AdjustBallPos(CRect rc);

protected:
	MapBalls					m_mapRemainBalls;
	MapBalls					m_mapSimulationWinBalls;

	std::map<int, CBallItem*> m_mapBalls;

	static const INT32			m_nMaxBalls;

	static CSimulationManager*	m_pSimulationManager;
};
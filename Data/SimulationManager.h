#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: Winning Number Manage Class
// CSimulationManager class

#include "BallItem.h"

typedef map<DWORD, COLORREF>	MapBalls;

class CSimulationManager
{
public:
	CSimulationManager(void);
	~CSimulationManager(void);

	static CSimulationManager*	GetInstance();
	static void					DestroyInstance();

	// Init
	INT32						Initialize();

	// Getter / Setter
	MapBalls&					GetBalls()			{		return m_mapSimulationWinBalls;		}
	void						SetEmpty();

protected:
	MapBalls					m_mapRemainBalls;
	MapBalls					m_mapSimulationWinBalls;

	static const INT32			m_nMaxBalls;

	static CSimulationManager*	m_pSimulationManager;
};
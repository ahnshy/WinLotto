#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: Winning Number Manage Class
// CSimulationManager class

#include "BallItem.h"

typedef map<DWORD, CBallItem*>	MapBalls;

class CSimulationManager
{
public:
	CSimulationManager(void);
	~CSimulationManager(void);

	static CSimulationManager*	GetInstance();
	static void					DestroyInstance();

	// Init
	INT32						Initialize();

	void						SaveConfig();
	void						ReadConfig();

	// Getter / Setter
	MapBalls&					GetBalls()			{		return m_mapBalls;		}
	void						SetEmpty();

protected:
	MapBalls					m_mapBalls;

	static CSimulationManager*	m_pSimulationManager;
};
#pragma once

/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class

#include "WinsItem.h"

typedef map<DWORD, CWinsItem*>	MapRounds;

class CWinsNumberManager
{
public:
	CWinsNumberManager(void);
	~CWinsNumberManager(void);

	static CWinsNumberManager*	GetInstance();
	static void					DestroyInstance();

	// Init
	INT32						Initialize(CStringArray& arrRounds);

	void						SaveConfig();
	void						ReadConfig();

	// Getter / Setter
	MapRounds&					GetRoundMap()			{		return m_mapRounds;		}
	void						SetEmpty();

protected:
	MapRounds					m_mapRounds;

	static CWinsNumberManager*	m_pWinsNumberManager;
};
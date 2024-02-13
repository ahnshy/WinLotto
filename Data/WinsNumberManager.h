#pragma once

/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class

#include "WinsItem.h"

typedef map<DWORD, CWinsItem*>	MapWins;

class CWinsNumberManager
{
public:
	CWinsNumberManager(void);
	~CWinsNumberManager(void);

	static CWinsNumberManager*	GetInstance();
	static void					DestroyInstance();

	// Init
	void						Initialize();
	void						SetEmpty();

	void						SaveConfig();
	void						ReadConfig();

	// Getter / Setter


	// Static :: Path
	static const CString		PathName;

protected:
	static CWinsNumberManager*	m_pWinsNumberManager;
};
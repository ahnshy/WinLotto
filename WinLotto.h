
// WinLotto.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWinLottoApp:
// See WinLotto.cpp for the implementation of this class
//

class CWinLottoApp : public CWinApp
{
public:
	CWinLottoApp();

// Overrides
public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	INT32			UpdateWinsNumber(BOOL bIsDeleteCache= FALSE);

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWinLottoApp theApp;
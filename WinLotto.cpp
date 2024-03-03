
// WinLotto.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinLotto.h"
#include "WinLottoDlg.h"

#include "Helper/HttpHelper.h"
#include "Helper/HtmlParser.h"

#include "Data/PathManager.h"
#include "Data/WinsNumberManager.h"
#include "Data/SimulationManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinLottoApp

BEGIN_MESSAGE_MAP(CWinLottoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWinLottoApp construction

CWinLottoApp::CWinLottoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CWinLottoApp object

CWinLottoApp theApp;


// CWinLottoApp initialization

BOOL CWinLottoApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("WinLotto"));

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	CMFCButton::EnableWindowsTheming();

	SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);

	CString strRoundFIle;
	CPathManager* pPathManager = CPathManager::GetInstance();
	if (pPathManager)
	{
		pPathManager->Initialize();
		strRoundFIle = pPathManager->GetRoundFilePath();
	}

	if (!strRoundFIle.IsEmpty() && !PathFileExists(strRoundFIle))
	{
		CHttpHelper http;
		http.GetHttpFile(_T("https://dhlottery.co.kr/gameResult.do?method=allWinExel&gubun=byWin&nowPage=&drwNoStart=1&drwNoEnd=9999"), strRoundFIle);
	}

	CHtmlParser dom;
	CStringArray strArray;
	dom.GetRounds(strRoundFIle, strArray);

	CWinsNumberManager* pNumberManager = CWinsNumberManager::GetInstance();
	if (pNumberManager)
		pNumberManager->Initialize(strArray);

	CWinLottoDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CWinLottoApp::ExitInstance()
{
	CSimulationManager* pSimulationManager = CSimulationManager::GetInstance();
	if (pSimulationManager)
		pSimulationManager->DestroyInstance();

	CPathManager* pPathManager = CPathManager::GetInstance();
	if (pPathManager)
		pPathManager->DestroyInstance();

	CWinsNumberManager* pNumberManager = CWinsNumberManager::GetInstance();
	if (pNumberManager)
		pNumberManager->DestroyInstance();

	CMFCVisualManager::DestroyInstance();

	return CWinApp::ExitInstance();
}
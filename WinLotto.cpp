// TreePropSheetEx_Demo.cpp : Defines the class behaviors for the application.
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2004 by Yves Tkaczyk
// (http://www.tkaczyk.net - yves@tkaczyk.net)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// Documentation: http://www.codeproject.com/property/treepropsheetex.asp
// CVS tree:      http://sourceforge.net/projects/treepropsheetex
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLotto.h"
#include "WinLottoDlg.h"

#include "UI/PropertySheet/TreePropSheet.h"
#include "UI/PropertySheet/TreePropSheetEx.h"
#include "UI/PropertySheet/TreePropSheetOffice2003.h"
#include "UI/PropertySheet/ResizableSheet.h"

#include "UI/PropertyPage/PageWins.h"
//#include "UI/PropertyPage/PageContact.h"
//#include "UI/PropertyPage/PageEmail.h"
#include "UI/PropertyPage/PagePhone.h"
//#include "UI/PropertyPage/PageNote.h"
//#include "UI/PropertyPage/PageDates.h"
//#include "UI/PropertyPage/PageCustomize.h"

//#include "ChildSheetsDlg.h"

#include "Helper/HttpHelper.h"
#include "Helper/HtmlParser.h"

#include "Data/WinsNumberManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreePropSheetEx_DemoApp

BEGIN_MESSAGE_MAP(CTreePropSheetEx_DemoApp, CWinApp)
	//{{AFX_MSG_MAP(CTreePropSheetEx_DemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreePropSheetEx_DemoApp construction

CTreePropSheetEx_DemoApp::CTreePropSheetEx_DemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTreePropSheetEx_DemoApp object

CTreePropSheetEx_DemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTreePropSheetEx_DemoApp initialization

BOOL CTreePropSheetEx_DemoApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1300
	#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif // _MSC_VER < 0x0700

	//CTreePropSheetEx_DemoDlg dlg;
	//m_pMainWnd = &dlg;

	TreePropSheet::CTreePropSheetOffice2003 sheet(IDS_STRING_PROJECT);
	sheet.SetTreeViewMode(TRUE, TRUE, TRUE);
	//sheet.SetEmptyPageText(_T("Select a sub-page"));
	//sheet.SetTreeDefaultImages( IDB_EMPTY_IMAGE_LIST, 16, RGB( 255, 255, 255 ) );

	CStringArray strArray;
	const CString strPath = _T("c:\\index.html");
	CHttpHelper http;
	http.GetHttpFile(_T("https://dhlottery.co.kr/gameResult.do?method=allWinExel&gubun=byWin&nowPage=&drwNoStart=1&drwNoEnd=9999"), strPath);

	CHtmlParser dom;
	//MapWins wins;
	dom.GetRounds(strPath, strArray);

	CWinsNumberManager* pManager = CWinsNumberManager::GetInstance();
	if (pManager)
		pManager->Initialize(strArray);

	CPageWins pageWins;
	//pageWins.SetData(&wins);
	//CPagePhone pagePhone;
	//CPageEmail pageEmail;
	//CPageContact pageContact(&pagePhone, &pageEmail);
	//CPageNote pageNote;
	//CPageDates pageDates;
	//CPageCustomize pageCustomize( sheet );

	pageWins.SetHasWhiteBackground(true);
	//pagePhone.SetHasWhiteBackground(true);
	//pageContact.SetHasWhiteBackground(true);
	//pagePhone.SetHasWhiteBackground(true);
	//pageEmail.SetHasWhiteBackground(true);
	//pageNote.SetHasWhiteBackground(true);
	//pageDates.SetHasWhiteBackground(true);
	//pageCustomize.SetHasWhiteBackground(true);

	sheet.AddPage(&pageWins);
	//sheet.AddPage(&pageContact);
	//sheet.AddPage(&pagePhone);
	//sheet.AddPage(&pageEmail);
	//sheet.AddPage(&pageNote);
	//sheet.AddPage(&pageDates);
	//sheet.AddPage(&pageCustomize);

	sheet.SetIsResizable(true);
	sheet.SetTreeWidth(170);  
	sheet.SetPaneMinimumSizes(100, 180);
	sheet.SetMinSize(CSize( 480, 530 ));
	sheet.SetAutoExpandTree(true);

	int nResponse = sheet.DoModal();
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

	if (pManager)
		pManager->DestroyInstance();

	return FALSE;
}

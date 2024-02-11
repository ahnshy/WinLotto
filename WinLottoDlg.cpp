// TreePropSheetEx_DemoDlg.cpp : implementation file
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

#include "UI/PropertyPage/PageContact.h"
#include "UI/PropertyPage/PageEmail.h"
#include "UI/PropertyPage/PagePhone.h"
#include "UI/PropertyPage/PageNote.h"
#include "UI/PropertyPage/PageDates.h"
#include "UI/PropertyPage/PageCustomize.h"

#include "ChildSheetsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreePropSheetEx_DemoDlg dialog

CTreePropSheetEx_DemoDlg::CTreePropSheetEx_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreePropSheetEx_DemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreePropSheetEx_DemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_CONTACT);
}

CTreePropSheetEx_DemoDlg::~CTreePropSheetEx_DemoDlg()
{
	// Delete all modeless sheets.
  for( std::vector<CPropertySheet*>::iterator itSheet = m_contSheet.begin(); m_contSheet.end() != itSheet; ++itSheet )
  {
    // Delete the pages.
    int nCount = (*itSheet)->GetPageCount();
    for( int nCurrent = 0; nCurrent < nCount; ++nCurrent )
    {
      CPropertyPage* pPage = (*itSheet)->GetPage( 0 );
      (*itSheet)->RemovePage( 0 );
      delete pPage;
    }

    // Destroy the associated window.
    if( ::IsWindow( (*itSheet)->GetSafeHwnd() ) )
      ::DestroyWindow( (*itSheet)->GetSafeHwnd() );
    // Delete the sheet object.
    delete *itSheet;
  }
}

void CTreePropSheetEx_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreePropSheetEx_DemoDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTreePropSheetEx_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CTreePropSheetEx_DemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLASSIC_TREEPROPSHEET, OnShowTreepropsheet)
	ON_BN_CLICKED(IDC_RESIZABLE_PROPERTYSHEET, OnResizablePropertysheet)
	ON_BN_CLICKED(IDC_RESIZABLE_TREEPROPSHEETEX, OnResizableTreepropsheetex)
	ON_BN_CLICKED(IDC_RESIZABLE_TABBED_TREEPROPSHEETEX, OnResizableTabbedTreepropsheetex)
	ON_BN_CLICKED(IDC_RESIZABLE_WIZARD_TREEPROPSHEETEX, OnResizableWizardTreepropsheetex)
	ON_BN_CLICKED(IDC_CHILD_TREEPROPSHEETEX, OnChildTreepropsheetex)
	ON_BN_CLICKED(IDC_MODELESS, OnModeless)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_TREEPROPSHEETOFFICE2003, OnTreepropsheetoffice2003)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreePropSheetEx_DemoDlg message handlers

BOOL CTreePropSheetEx_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTreePropSheetEx_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTreePropSheetEx_DemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTreePropSheetEx_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTreePropSheetEx_DemoDlg::OnShowTreepropsheet() 
{
  CPageContact pageContact;
  CPagePhone pagePhone;
  CPageEmail pageEmail;
  CPageNote pageNote;
  CPageDates pageDates;

  TreePropSheet::CTreePropSheet sheet( _T("TreePropSheet demo"), this );
  sheet.SetTreeViewMode( TRUE, TRUE, TRUE);
  sheet.SetTreeDefaultImages( IDB_EMPTY_IMAGE_LIST, 16, RGB( 255, 255, 255 ) );

  sheet.AddPage( &pageContact );
  sheet.AddPage( &pagePhone );
  sheet.AddPage( &pageEmail );
  sheet.AddPage( &pageNote );
  sheet.AddPage( &pageDates );

  sheet.DoModal();
}

void CTreePropSheetEx_DemoDlg::OnResizablePropertysheet() 
{
  CPageContact pageContact;
  CPagePhone pagePhone;
  CPageEmail pageEmail;
  CPageNote pageNote;
  CPageDates pageDates;

  CResizableSheet sheet( _T("Resizable property sheet demo"), this );

  sheet.AddPage( &pageContact );
  sheet.AddPage( &pagePhone );
  sheet.AddPage( &pageEmail );
  sheet.AddPage( &pageNote );
  sheet.AddPage( &pageDates );

  sheet.DoModal();
}

void CTreePropSheetEx_DemoDlg::OnResizableTreepropsheetex() 
{
  TreePropSheet::CTreePropSheetEx sheet( _T("TreePropSheet demo"), this );
  sheet.SetTreeViewMode( TRUE, TRUE, TRUE);
  sheet.SetEmptyPageText( _T("Select a sub-page") );
  sheet.SetTreeDefaultImages( IDB_EMPTY_IMAGE_LIST, 16, RGB( 255, 255, 255 ) );

  CPagePhone pagePhone;
  CPageEmail pageEmail;
  CPageContact pageContact( &pagePhone, &pageEmail );
  CPageNote pageNote;
  CPageDates pageDates;
  CPageCustomize pageCustomize( sheet );

  sheet.AddPage( &pageContact );
  sheet.AddPage( &pagePhone );
  sheet.AddPage( &pageEmail );
  sheet.AddPage( &pageNote );
  sheet.AddPage( &pageDates );
  sheet.AddPage( &pageCustomize );

  sheet.SetIsResizable( true );
  sheet.SetTreeWidth( 200 );  
  sheet.SetPaneMinimumSizes( 135, 180 );
  sheet.SetMinSize( CSize( 340, 320 ) );

  sheet.DoModal();
}

void CTreePropSheetEx_DemoDlg::OnResizableTabbedTreepropsheetex() 
{
  CPageContact pageContact;
  CPagePhone pagePhone;
  CPageEmail pageEmail;
  CPageNote pageNote;

  TreePropSheet::CTreePropSheetEx sheet( _T("TreePropSheet demo"), this );
  sheet.SetTreeViewMode( FALSE, FALSE, FALSE);

  sheet.AddPage( &pageContact );
  sheet.AddPage( &pagePhone );
  sheet.AddPage( &pageEmail );
  sheet.AddPage( &pageNote );

  sheet.SetIsResizable( true );

  sheet.DoModal();
}

void CTreePropSheetEx_DemoDlg::OnResizableWizardTreepropsheetex() 
{
  CPageContact pageContact;
  CPagePhone pagePhone;
  CPageEmail pageEmail;
  CPageNote pageNote;

  TreePropSheet::CTreePropSheetEx sheet( _T("TreePropSheet demo"), this );
  sheet.SetTreeViewMode( FALSE, FALSE, FALSE);
  sheet.SetWizardMode();

  sheet.AddPage( &pageContact );
  sheet.AddPage( &pagePhone );
  sheet.AddPage( &pageEmail );
  sheet.AddPage( &pageNote );

  sheet.SetIsResizable( true );

  sheet.DoModal();
}

void CTreePropSheetEx_DemoDlg::OnChildTreepropsheetex() 
{
  CChildSheetsDlg dlg;
  dlg.DoModal();
}

void CTreePropSheetEx_DemoDlg::OnModeless() 
{
  TreePropSheet::CTreePropSheetEx* pSheet = new TreePropSheet::CTreePropSheetEx( _T("TreePropSheet demo"), this );
  m_contSheet.push_back( pSheet );

  pSheet->SetTreeViewMode( TRUE, TRUE, TRUE);
  pSheet->SetEmptyPageText( _T("Select a sub-page") );
  pSheet->SetTreeDefaultImages( IDB_EMPTY_IMAGE_LIST, 16, RGB( 255, 255, 255 ) );

  pSheet->AddPage( new CPageContact() );
  pSheet->AddPage( new CPagePhone );
  pSheet->AddPage( new CPageEmail );
  pSheet->AddPage( new CPageNote );
  pSheet->AddPage( new CPageDates );
  pSheet->AddPage( new CPageCustomize( *pSheet ) );

  pSheet->SetIsResizable( true );
  pSheet->SetTreeWidth( 200 );
  pSheet->SetMinSize( CSize( 460, 350 ) );

  pSheet->Create( this, WS_POPUP|WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_VISIBLE, 0 );
  pSheet->Invalidate( TRUE );
}

void CTreePropSheetEx_DemoDlg::OnTreepropsheetoffice2003() 
{
  TreePropSheet::CTreePropSheetOffice2003 sheet( _T("TreePropSheet demo"), this );
  sheet.SetTreeViewMode( TRUE, TRUE, TRUE);
  sheet.SetEmptyPageText( _T("Select a sub-page") );
  sheet.SetTreeDefaultImages( IDB_EMPTY_IMAGE_LIST, 16, RGB( 255, 255, 255 ) );

  CPagePhone pagePhone;
  CPageEmail pageEmail;
  CPageContact pageContact( &pagePhone, &pageEmail );
  CPageNote pageNote;
  CPageDates pageDates;
  CPageCustomize pageCustomize( sheet );

  pageContact.SetHasWhiteBackground( true );
  pagePhone.SetHasWhiteBackground( true );
  pageEmail.SetHasWhiteBackground( true );
  pageNote.SetHasWhiteBackground( true );
  pageDates.SetHasWhiteBackground( true );
  pageCustomize.SetHasWhiteBackground( true );

  sheet.AddPage( &pageContact );
  sheet.AddPage( &pagePhone );
  sheet.AddPage( &pageEmail );
  sheet.AddPage( &pageNote );
  sheet.AddPage( &pageDates );
  sheet.AddPage( &pageCustomize );

  sheet.SetIsResizable( true );
  sheet.SetTreeWidth( 105 );  
  sheet.SetPaneMinimumSizes( 100, 180 );
  sheet.SetMinSize( CSize( 300, 320 ) );
  sheet.SetAutoExpandTree( true );

  sheet.DoModal();
}

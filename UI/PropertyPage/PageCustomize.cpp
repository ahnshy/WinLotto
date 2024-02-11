// PageCustomize.cpp : implementation file
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
//#include "WinLotto.h"
#include "PageCustomize.h"
#include "../PropertySheet/TreePropSheetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageCustomize property page

IMPLEMENT_DYNAMIC(CPageCustomize, CResizablePage)

CPageCustomize::CPageCustomize(TreePropSheet::CTreePropSheetEx& rSheet)
 : CResizablePage(CPageCustomize::IDD),
    m_sheet(rSheet)
{
  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= PSP_USEHICON;
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_PREFERENCES );
  m_psp.hIcon = hIconTab;

  //{{AFX_DATA_INIT(CPageCustomize)
	m_nTreeResizingMode = -1;
	m_bRealtimeRefreshMode = FALSE;
	m_bSkipEmptyPages = FALSE;
	m_bResizableTree = FALSE;
	//}}AFX_DATA_INIT
}

CPageCustomize::~CPageCustomize()
{
}

void CPageCustomize::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageCustomize)
	DDX_Radio(pDX, IDC_FIXED, m_nTreeResizingMode);
	DDX_Check(pDX, IDC_REAL_TIME_REFRESH_MODE, m_bRealtimeRefreshMode);
	DDX_Check(pDX, IDC_SKIP_EMPTY_PAGES, m_bSkipEmptyPages);
	DDX_Check(pDX, IDC_CHK_TREE_RESIZABLE, m_bResizableTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageCustomize, CResizablePage)
	//{{AFX_MSG_MAP(CPageCustomize)
	ON_BN_CLICKED(IDC_REAL_TIME_REFRESH_MODE, OnRealTimeRefreshMode)
	ON_BN_CLICKED(IDC_SKIP_EMPTY_PAGES, OnSkipEmptyPages)
	ON_BN_CLICKED(IDC_FIXED, OnChangeTreeResizingMode)
	ON_BN_CLICKED(IDC_CHK_TREE_RESIZABLE, OnChkTreeResizable)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PROPORTIONAL, OnChangeTreeResizingMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageCustomize message handlers

BOOL CPageCustomize::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
  m_nTreeResizingMode = m_sheet.GetTreeResizingMode()==TreePropSheet::TSM_Fixed?0:1;
	m_bRealtimeRefreshMode = m_sheet.IsRealTimeSplitter();
  m_bSkipEmptyPages = m_sheet.IsSkippingEmptyPages();
  m_bResizableTree = m_sheet.IsTreeResizable();

  UpdateData( FALSE );

	// Preset layout
	AddAnchor(IDC_RESIZING_FRAME, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_FIXED, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_PROPORTIONAL, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_REFRESH_FRAME, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_REAL_TIME_REFRESH_MODE, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_EMPTY_PAGES_FRAME, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_SKIP_EMPTY_PAGES, TOP_LEFT, TOP_LEFT);
  AddAnchor(IDC_CHK_TREE_RESIZABLE, TOP_LEFT, TOP_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageCustomize::OnRealTimeRefreshMode() 
{
  UpdateData( TRUE );

  m_sheet.SetRealTimeSplitter( m_bRealtimeRefreshMode?true:false );
}

void CPageCustomize::OnSkipEmptyPages() 
{
  UpdateData( TRUE );

  m_sheet.SetSkipEmptyPages( m_bSkipEmptyPages?true:false );
}

void CPageCustomize::OnChangeTreeResizingMode() 
{
  UpdateData( TRUE );

  m_sheet.SetTreeResizingMode( m_nTreeResizingMode?TreePropSheet::TSM_Proportional:TreePropSheet::TSM_Fixed );
}

void CPageCustomize::OnChkTreeResizable() 
{
	UpdateData( TRUE );

  m_sheet.SetTreeIsResizable( m_bResizableTree?true:false );
	
}

HBRUSH CPageCustomize::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( HasWhiteBackground() )
  {
    pDC->SetBkMode(TRANSPARENT);
    return ::GetSysColorBrush( COLOR_WINDOW );
  }

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

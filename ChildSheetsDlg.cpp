// ChildSheetsDlg.cpp : implementation file
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
#include "ChildSheetsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildSheetsDlg dialog


CChildSheetsDlg::CChildSheetsDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CChildSheetsDlg::IDD, pParent),
    m_splitter( 2, SSP_VERT, 150, 5 )
{
	//{{AFX_DATA_INIT(CChildSheetsDlg)
	//}}AFX_DATA_INIT
}


void CChildSheetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildSheetsDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildSheetsDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CChildSheetsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
bool CChildSheetsDlg::RegisterSheet(UINT nID, CPropertySheet& rSheet)
{
 	CWnd* pWnd = GetDlgItem( nID );
	if( !pWnd )
	{
		ASSERT( FALSE );
		return false;
	}

	rSheet.Create( this, WS_CHILD | WS_VISIBLE , 0 );

	if( 0 != rSheet.GetPageCount() )
	{
		rSheet.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
		rSheet.ModifyStyle( 0, WS_TABSTOP );

		CRect rcSheet;
		pWnd->GetWindowRect( &rcSheet );
		ScreenToClient( &rcSheet );
//		rSheet.SetWindowPos( NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE );
		rSheet.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE );
	}

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CChildSheetsDlg message handlers

BOOL CChildSheetsDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// Initialize and add the sheets
  m_sheetTop.AddPage( &m_pageNoteTop );
  m_sheetTop.SetTreeViewMode( TRUE, TRUE, TRUE );
  m_sheetTop.SetIsResizable( true );
  m_sheetTop.SetPaneMinimumSizes( 50, 200 );
  RegisterSheet( IDC_TOP_SHEET, m_sheetTop );
  
  m_sheetBottom.AddPage( &m_pageNoteBottom );
  m_sheetBottom.SetTreeViewMode( TRUE, TRUE, TRUE );
  m_sheetBottom.SetIsResizable( true );
  RegisterSheet( IDC_BOTTOM_SHEET, m_sheetBottom );

  // Create splitter
  CRect rect, rectButton;
  GetClientRect( &rect );
  GetDlgItem( IDOK )->GetWindowRect( rectButton );
  ScreenToClient( rectButton );
  VERIFY( m_splitter.Create( this, CRect(5, 5, rect.Width() - 5, rectButton.top - 5 ) ) );
  m_splitter.SetPane( 0, &m_sheetTop );
  m_splitter.SetPane( 1, &m_sheetBottom );

	// Preset layout
  AddAnchor( m_splitter, TOP_LEFT, BOTTOM_RIGHT );
  AddAnchor( IDOK, BOTTOM_RIGHT );
	AddAnchor( IDCANCEL, BOTTOM_RIGHT );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

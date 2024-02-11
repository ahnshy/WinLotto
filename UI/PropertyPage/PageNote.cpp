// PageNote.cpp : implementation file
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
#include "PageNote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageNote property page

IMPLEMENT_DYNCREATE(CPageNote, CResizablePage)

CPageNote::CPageNote() : CResizablePage(CPageNote::IDD)
{
	//{{AFX_DATA_INIT(CPageNote)
	//}}AFX_DATA_INIT

  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= PSP_USEHICON;
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_NOTE );
  m_psp.hIcon = hIconTab;
}

CPageNote::~CPageNote()
{
}

void CPageNote::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageNote)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageNote, CResizablePage)
	//{{AFX_MSG_MAP(CPageNote)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageNote message handlers

BOOL CPageNote::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	// Preset layout
	AddAnchor(IDC_NOTE, TOP_LEFT, BOTTOM_RIGHT);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPageNote::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( HasWhiteBackground() )
  {
    pDC->SetBkMode(TRANSPARENT);
    return ::GetSysColorBrush( COLOR_WINDOW );
  }

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

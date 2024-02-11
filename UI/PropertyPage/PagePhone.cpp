// PagePhone.cpp : implementation file
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
#include "PagePhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPagePhone property page

IMPLEMENT_DYNCREATE(CPagePhone, CResizablePage)

CPagePhone::CPagePhone() : CResizablePage(CPagePhone::IDD)
{
	//{{AFX_DATA_INIT(CPagePhone)
	//}}AFX_DATA_INIT

  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= ( PSP_USEHICON );
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_TELEPHONE );
  m_psp.hIcon = hIconTab;
}

CPagePhone::~CPagePhone()
{
}

void CPagePhone::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagePhone)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPagePhone, CResizablePage)
	//{{AFX_MSG_MAP(CPagePhone)
	ON_WM_CTLCOLOR()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPagePhone message handlers

BOOL CPagePhone::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	// Preset layout
	AddAnchor(IDC_PHONE1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_PHONE2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_PHONE3, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_COMBO_DEFAULT_PHONE, TOP_LEFT, TOP_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPagePhone::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( HasWhiteBackground() )
  {
    pDC->SetBkMode(TRANSPARENT);
    return ::GetSysColorBrush( COLOR_WINDOW );
  }

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPagePhone::OnEnable(BOOL bEnable) 
{
	CResizablePage::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	
}

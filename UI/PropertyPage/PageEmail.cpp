// PageEmail.cpp : implementation file
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
#include "PageEmail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageEmail property page

IMPLEMENT_DYNCREATE(CPageEmail, CResizablePage)

CPageEmail::CPageEmail() : CResizablePage(CPageEmail::IDD)
{
	//{{AFX_DATA_INIT(CPageEmail)
	//}}AFX_DATA_INIT

  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= ( PSP_USEHICON );
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_MAIL );
  m_psp.hIcon = hIconTab;
}

CPageEmail::~CPageEmail()
{
}

void CPageEmail::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageEmail)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageEmail, CResizablePage)
	//{{AFX_MSG_MAP(CPageEmail)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageEmail message handlers

BOOL CPageEmail::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	// Preset layout
	AddAnchor(IDC_EMAIL1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EMAIL2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EMAIL3, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_COMBO_DEFAULT_EMAIL, TOP_LEFT, TOP_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPageEmail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( HasWhiteBackground() )
  {
    pDC->SetBkMode(TRANSPARENT);
    return ::GetSysColorBrush( COLOR_WINDOW );
  }

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

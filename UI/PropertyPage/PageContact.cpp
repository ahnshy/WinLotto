// PageContact.cpp : implementation file
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
#include "PageContact.h"
#include "../PropertySheet/TreePropSheetBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageContact property page

IMPLEMENT_DYNCREATE(CPageContact, CResizablePage)

CPageContact::CPageContact()
 : CResizablePage(CPageContact::IDD),
   m_pPagePhone(NULL),
   m_pPageEmail(NULL)

{
	//{{AFX_DATA_INIT(CPageContact)
	//}}AFX_DATA_INIT

  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= PSP_USEHICON;
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_CONTACT );
  m_psp.hIcon = hIconTab;
}

CPageContact::CPageContact(CPropertyPage* pPagePhone,CPropertyPage* pPageEmail)
 : CResizablePage(CPageContact::IDD),
   m_pPagePhone(pPagePhone),
   m_pPageEmail(pPageEmail)
{
  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= PSP_USEHICON;
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_CONTACT );
  m_psp.hIcon = hIconTab;
}

CPageContact::~CPageContact()
{
}

void CPageContact::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageContact)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageContact, CResizablePage)
	//{{AFX_MSG_MAP(CPageContact)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_EMAIL, OnCheckEmail)
	ON_BN_CLICKED(IDC_CHECK_PHONE, OnCheckPhone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageContact message handlers

BOOL CPageContact::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	// Preset layout
	AddAnchor(IDC_NAME, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_ADDRESS, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CHECK_PHONE, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_CHECK_EMAIL, BOTTOM_LEFT, BOTTOM_LEFT);
	
  // Enable and check the check boxes based on the presence of the 'child pages'.
  ::EnableWindow( GetDlgItem( IDC_CHECK_EMAIL)->GetSafeHwnd(), NULL != m_pPageEmail );
  CheckDlgButton( IDC_CHECK_EMAIL, 1 );
  ::EnableWindow( GetDlgItem( IDC_CHECK_PHONE)->GetSafeHwnd(), NULL != m_pPagePhone );
  CheckDlgButton( IDC_CHECK_PHONE, 1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPageContact::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( HasWhiteBackground() )
  {
    pDC->SetBkMode(TRANSPARENT);
    return ::GetSysColorBrush( COLOR_WINDOW );
  }

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPageContact::OnCheckEmail() 
{
  // Enable/disable the email page.
	if( NULL == m_pPageEmail )
    return;

  TreePropSheet::Send_EnablePage( GetParent()->GetSafeHwnd(), m_pPageEmail, 0 != IsDlgButtonChecked( IDC_CHECK_EMAIL ) );
}

void CPageContact::OnCheckPhone() 
{
  // Enable/disable the phone page.
	if( NULL == m_pPagePhone )
    return;

  TreePropSheet::Send_EnablePage( GetParent()->GetSafeHwnd(), m_pPagePhone, 0 != IsDlgButtonChecked( IDC_CHECK_PHONE ) );
}

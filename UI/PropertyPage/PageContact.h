// PageContact.h : header file
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

#pragma once

#include "ResizablePage.h"
#include "../PropertySheet/TreePropSheetUtil.hpp"

/////////////////////////////////////////////////////////////////////////////
// CPageContact dialog

class CPageContact
 : public CResizablePage,
   public TreePropSheet::CWhiteBackgroundProvider
{
	DECLARE_DYNCREATE(CPageContact)

// Construction
public:
	CPageContact();
	CPageContact(CPropertyPage* pPagePhone, CPropertyPage* pPageEmail);
	~CPageContact();

// Dialog Data
	//{{AFX_DATA(CPageContact)
	enum { IDD = IDD_PAGE_CONTACT };
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageContact)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageContact)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckEmail();
	afx_msg void OnCheckPhone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Members
private:
  // 'Child property pages' to allow enabling/disabling according to current page settings.
  CPropertyPage* m_pPagePhone;
  CPropertyPage* m_pPageEmail;
};
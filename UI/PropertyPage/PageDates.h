// PageDates.h : header file
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

#include "../PropertySheet/TreePropSheetUtil.hpp"

/////////////////////////////////////////////////////////////////////////////
// CPageDates dialog

class CPageDates
 : public CPropertyPage,
   public TreePropSheet::CWhiteBackgroundProvider
{
	DECLARE_DYNCREATE(CPageDates)

// Construction
public:
	CPageDates();
	~CPageDates();

// Dialog Data
	//{{AFX_DATA(CPageDates)
	enum { IDD = IDD_PAGE_MISC_DATES };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDates)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDates)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
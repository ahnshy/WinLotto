// PageCustomize.h : header file
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
// CPageCustomize dialog

namespace TreePropSheet
{
  class CTreePropSheetEx;
}

class CPageCustomize
 : public CResizablePage,
   public TreePropSheet::CWhiteBackgroundProvider
{
	DECLARE_DYNAMIC(CPageCustomize)

// Construction
public:
	CPageCustomize(TreePropSheet::CTreePropSheetEx& rSheet);
	~CPageCustomize();

// Dialog Data
	//{{AFX_DATA(CPageCustomize)
	enum { IDD = IDD_PAGE_CUSTOMIZE_SHEET };
	int		m_nTreeResizingMode;
	BOOL	m_bRealtimeRefreshMode;
	BOOL	m_bSkipEmptyPages;
	BOOL	m_bResizableTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageCustomize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageCustomize)
	afx_msg void OnRealTimeRefreshMode();
	afx_msg void OnSkipEmptyPages();
	afx_msg void OnChangeTreeResizingMode();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkTreeResizable();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Members
private:
  TreePropSheet::CTreePropSheetEx& m_sheet;
};
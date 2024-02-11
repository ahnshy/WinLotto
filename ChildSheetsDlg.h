// ChildSheetsDlg.h : header file
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

#ifndef _TREEPROPSHEET_CHILDSHEETSDLG_H__INCLUDED_
#define _TREEPROPSHEET_CHILDSHEETSDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableDialog.h"
#include "UI/PropertySheet/TreePropSheetEx.h"
#include "UI/PropertySheet/TreePropSheetBordered.h"
#include "UI/PropertyPage/PageNote.h"
#include "UI/PropertySheet/TreePropSheetSplitter.h"

/////////////////////////////////////////////////////////////////////////////
// CChildSheetsDlg dialog

class CChildSheetsDlg
 : public CResizableDialog
{
// Construction
public:
	CChildSheetsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChildSheetsDlg)
	enum { IDD = IDD_DLG_CHILD_SHEETS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildSheetsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChildSheetsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Helpers
private:
  bool RegisterSheet(UINT nID,CPropertySheet& rSheet);

// Members
private:
  TreePropSheet::CTreePropSheetEx m_sheetTop;
  TreePropSheet::CTreePropSheetBordered m_sheetBottom;

  CPageNote m_pageNoteTop;
  CPageNote m_pageNoteBottom;

  // Splitter control.
  TreePropSheet::CTreePropSheetSplitter m_splitter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _TREEPROPSHEET_CHILDSHEETSDLG_H__INCLUDED_

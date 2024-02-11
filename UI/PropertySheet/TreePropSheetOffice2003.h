// TreePropSheetOffice2003.h: interface for the CTreePropSheetOffice2003 class.
//
//////////////////////////////////////////////////////////////////////
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
#include "TreePropSheetEx.h"

namespace TreePropSheet
{
/*! @brief TreePropSheetEx with Office 2003 option dialog look and feel.

  The version of CTreePropSheetEx has the following additions:
  - Use CPropPageFrameOffice2003 for page frame.
  - Tree style is modified to show for row selection. 
  The property pages should be modified in order to return a COLOR_WINDOW 
  background. This can be done as follows:
  <code>
  HBRUSH CAPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
  {
    pDC->SetBkMode(TRANSPARENT);
    return ::GetSysColorBrush( COLOR_WINDOW );
  }
  </code>

@version 0.1 Initial release
@author Yves Tkaczyk <yves@tkaczyk.net>
@date 09/2004 */
class CTreePropSheetOffice2003
 : public CTreePropSheetEx  
{
// Construction/Destruction
DECLARE_DYNAMIC(CTreePropSheetOffice2003)

public:
	CTreePropSheetOffice2003();
	CTreePropSheetOffice2003(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTreePropSheetOffice2003(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTreePropSheetOffice2003();

// Overrided implementation helpers
protected:
	/**
	Will be called during creation process, to create the object, that
	is responsible for drawing the frame around the pages, drawing the
	empty page message and the caption.

	Allows you to inject your own CPropPageFrame-derived classes.

	This default implementation simply creates a CPropPageFrameTab with
	new and returns it.
	*/
	virtual CPropPageFrame* CreatePageFrame();

	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// Overridings
protected:
	//{{AFX_VIRTUAL(CTreePropSheetOffice2003)
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Message handlers
protected:
	//{{AFX_MSG(CTreePropSheetOffice2003)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

}; // namespace TreePropSheet

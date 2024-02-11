// TreePropSheetBordered.h.
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

#ifndef _TREEPROPSHEET_TREEPROPSHEETBORDERED_H__INCLUDED_
#define _TREEPROPSHEET_TREEPROPSHEETBORDERED_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreePropSheetEx.h"

namespace TreePropSheet
{
/*! @brief Base on CTreePropSheetEx, use CPropPageFrameBordered

  CTreePropSheetBordered overrides the CreatePageFrame method to create a
  CPropPageFrameBordered frame class.

@version 0.1 Initial release
@version 0.4 Added DECLARE_DYNAMIC
@author Yves Tkaczyk <yves@tkaczyk.net>
@date 08/2004 */
class CTreePropSheetBordered
 : public CTreePropSheetEx  
{
// Construction/Destruction
DECLARE_DYNAMIC(CTreePropSheetBordered)

public:
	CTreePropSheetBordered();
	CTreePropSheetBordered(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTreePropSheetBordered(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
  virtual ~CTreePropSheetBordered();

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
};

}; // namespace TreePropSheet

#endif // _TREEPROPSHEET_TREEPROPSHEETBORDERED_H__INCLUDED_

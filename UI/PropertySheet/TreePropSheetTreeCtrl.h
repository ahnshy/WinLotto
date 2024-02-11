// TreePropSheetTreeCtrl.h
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
// Based on code by:
//
//  /////////////////////////////////////////////////////////////
//  //	
//  //	Author:		Sami (M.ALSAMSAM), ittiger@ittiger.net
//  //
//  //	Filename:	TreeCtrlEx.h
//  //
//  //	http	 :	www.ittiger.net
//  //
//  //////////////////////////////////////////////////////////////
// 
// Documentation: http://www.codeproject.com/property/treepropsheetex.asp
// CVS tree:      http://sourceforge.net/projects/treepropsheetex
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _TREEPROPSHEETTREECTRL_H__INCLUDED_
#define _TREEPROPSHEETTREECTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

namespace TreePropSheet
{
/*! @brief Tree control with individual color item support.

  CTreePropSheetTreeCtrl is greatly inspired from CTreeCtrlEx. 
  However, the control only allows changing the color of an 
  item (not the font).

  @version 0.1 Initial version
  @author Yves Tkaczyk <yves@tkaczyk.net>
  @date 04/2005 */
class CTreePropSheetTreeCtrl
 : public CTreeCtrl
{
// Construction/Destruction
  DECLARE_DYNAMIC(CTreePropSheetTreeCtrl)

public:
  CTreePropSheetTreeCtrl();
  virtual ~CTreePropSheetTreeCtrl();

// Properties
	void SetItemBold(const HTREEITEM hItem, const bool bBold);
	BOOL GetItemBold(const HTREEITEM hItem) const;

	void SetItemColor(const HTREEITEM hItem, const COLORREF color);
	COLORREF GetItemColor(const HTREEITEM hItem) const;

// Members
protected:
  // Inner class storing individual tree item.
	struct sTreeItemProperties
  {
    sTreeItemProperties()
     : color((COLORREF)0xFFFFFFFF)
    {
    }

    //! Tree  item color.
		COLORREF color;
	};

  typedef std::map<HTREEITEM, sTreeItemProperties> tTreeItemProperties;
	tTreeItemProperties m_mapTreeItemProperties;

  protected:
  //{{AFX_MSG(CTreePropSheetTreeCtrl)
    afx_msg void OnPaint();
	afx_msg LRESULT OnSetItemColor(WPARAM wParam, LPARAM lParam);
 //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

}  //namespace TreePropSheet
#endif // _TREEPROPSHEETTREECTRL_H__INCLUDED_

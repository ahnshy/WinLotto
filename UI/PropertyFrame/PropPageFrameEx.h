// PropPageFrameEx.h: interface for the CPropPageFrameEx class.
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
//  /********************************************************************
//  *
//  * Copyright (c) 2002 Sven Wiegand <forum@sven-wiegand.de>
//  *
//  * You can use this and modify this in any way you want,
//  * BUT LEAVE THIS HEADER INTACT.
//  *
//  * Redistribution is appreciated.
//  *
//  * $Workfile:$
//  * $Revision: 1.4 $
//  * $Modtime:$
//  * $Author: ytkaczyk $
//  *
//  * Revision History:
//  *	$History:$
//  *
//  *********************************************************************/
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _TREEPROPSHEET_PROPPAGEFRAMEEX_H__INCLUDED_
#define _TREEPROPSHEET_PROPPAGEFRAMEEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropPageFrame.h"

namespace TreePropSheet
{
//! Forward declaration of helper class for XP Theme.
class CThemeLibEx;

/*! @brief Default frame window for CTreePropSheetEx sheets

  This class is based on CPropPageFrame. It differs in 
  the following:
  - Reduced flickering by using double-buffering. This is 
    especially noticeable in the gradient portion while 
    resizing.
  - Draw gradient using GDI GradientFill or original owner
    drawn gradient (compile time switch).

  @version 0.1
  @author Sven Wiegand (original CPropPageFrame)
  @author Yves Tkaczyk <yves@tkaczyk.net> 
  @date 07/2004 */
class CPropPageFrameEx : 
	public CWnd, 
	public CPropPageFrame  
{
// Construction/Destruction
public:
	CPropPageFrameEx();
	virtual ~CPropPageFrameEx();

// CPropPageFrame implementation
public:
	/*! Create the Windows window for the frame.
	  @param dwWindowStyle Specifies the window style attributes
	  @param rect The size and position of the window, in client coordinates of pParentWnd.
	  @param pwndParent The parent window. This cannot be NULL.
	  @param nID The ID of the child window.
	  @return Nonzero if successful; otherwise 0. */
  virtual BOOL Create(DWORD dwWindowStyle, const RECT &rect, CWnd *pwndParent, UINT nID);
	
	/*! a pointer to the window object, that represents the frame.
	  @return Pointer to the window object. */
  virtual CWnd* GetWnd();

// CPropPageFrame overridings
public:
  virtual void SetCaption(LPCTSTR lpszCaption, HICON hIcon = NULL);
protected:
	virtual CRect CalcMsgArea();
	virtual CRect CalcCaptionArea();
	virtual void DrawCaption(CDC *pDc, CRect rect, LPCTSTR lpszCaption, HICON hIcon);

// Methods
protected:
  /*! Returns the helper class for XP theme.
    @internal The helper class is stored as a static member.
    @return Pointer to the helper class. */
  const CThemeLibEx& GetThemeLib() const;

// Overridables
protected:
  /*! Draw the background.
    @param pDC Pointer to device context. */
  virtual void DrawBackground(CDC* pDC);

// Helpers
protected:
	/*! Fill the rectangle with a gradient using custom algorithm. */ 
  static void FillOwnerDrawnGradientRectH(CDC *pDc, const RECT &rect, COLORREF clrLeft, COLORREF clrRight);
  /*! Fill the rectangle with a gradient using GDI GradientFill function. */
	static void FillGDIGradientRectH(CDC *pDc, const RECT &rect, COLORREF clrLeft, COLORREF clrRight);
  /*! Initialize which gradient method to use. The decision is made at 
      compile time.
      @internal This will be extended to support runtime algorithm selection, 
                with GDI as default method and fall back to owner draw if
                GDI not available. */
  void InitialGradientDrawingFunction();

// message handlers
protected:
	//{{AFX_MSG(CPropPageFrameEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** 
	Image list that contains only the current icon or nothing if there
	is no icon.
	*/
	CImageList m_Images;

  /*! Gradient function. */
  typedef void tGradientFn(CDC *pDc, const RECT &rect, COLORREF clrLeft, COLORREF clrRigh);
  tGradientFn* m_pGradientFn;
};

};    // Namespace TreePropSheet

#endif // !defined(_TREEPROPSHEET_PROPPAGEFRAMEEX_H__INCLUDED_)


// TreePropSheetResizableLibHook.h: interface for the CTreePropSheetResizableLibHook class.
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

#include "HookWnd.h"
#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"
#include "ResizableState.h"

namespace TreePropSheet
{
  //********************************************************************
  // CResizableSheetHook
  //********************************************************************

  /*! \brief Window hook for CPropertySheet and ResizableLib

    @version 0.1
    @author Yves Tkaczyk <yves@tkaczyk.net> 
    @date 07/2004 */
  class CResizableSheetHook
    : public CHookWnd, 
      public CResizableLayout,
      public CResizableGrip, 
      public CResizableMinMax,
      public CResizableState
  {
  // Construction/Destruction
  public:
    CResizableSheetHook(CPropertySheet* const pSheet);
    virtual ~CResizableSheetHook();

  // Methods
    /*! Attach the hook to the window. */
    void Initialize();

  // Exposed methods from base-class.
    void SetMinSize(const CSize& size);
    void SetMaxSize(const CSize& size);

  // CHookWnd overrides
  protected:
    virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

  // CResizableLayout implementation
  protected:
    virtual CWnd* GetResizableWnd();
    virtual BOOL ArrangeLayoutCallback(LayoutInfo &layout);

  public:
    // Make AddAnchor public.
    using CResizableLayout::AddAnchor;

  // Overridable
  protected:
    virtual void PresetLayout();

  // Helpers
  protected:
	  void PrivateConstruct();
	  void SavePage();
	  void LoadPage();
	  BOOL IsWizard();

  // callable from derived classes
  protected:
	  // section to use in app's profile
	  void EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE, BOOL bWithPage = FALSE);
	  int GetMinWidth();	// minimum width to display all buttons

  // Members
  protected:
    //! The parent window.
    CPropertySheet* m_pSheet;

	  // support for temporarily hiding the grip
	  DWORD m_dwGripTempState;

	  // flags
	  BOOL m_bEnableSaveRestore;
	  BOOL m_bRectOnly;
	  BOOL m_bSavePage;

	  // layout vars
	  CSize m_sizePageTL, m_sizePageBR;

	  // internal status
	  CString m_sSection;			// section name (identifies a parent window)
  };
  
  // Forward declaration of CTreePropSheetEx.
  class CTreePropSheetEx;

  //********************************************************************
  // CTreePropSheetResizableLibHook
  //********************************************************************

  /*! \brief Window hook for CTreePropSheetEx and ResizableLib

  @version 0.1
  @author Yves Tkaczyk <yves@tkaczyk.net> 
  @date 07/2004 */
  class CTreePropSheetResizableLibHook
    : public CResizableSheetHook
  {
  // Construction/Destruction
  public:
    CTreePropSheetResizableLibHook(TreePropSheet::CTreePropSheetEx* pTreePropSheet);
    ~CTreePropSheetResizableLibHook();

  // Overridable
  protected:
    /*! Register the different sheet's controls with the layout manager. The
        splitter control is registered with the layout manager as well as the
        sheet's buttons. */
    virtual void PresetLayout();

  // CResizableLayout implementation
  protected:
    /*! Layout manager callback method. This is used to position the active page 
        and the tab control. 
    \param layout Reference to LayoutInfo. The callback method populates this structure
                  by setting the HWND of the window to be move as well as resizing 
                  information.
    \retval TRUE if the LayoutInfo was populated successfully, FALSE otherwise. */
    virtual BOOL ArrangeLayoutCallback(LayoutInfo &layout);

  // Helpers
  protected:
    /*! Callback implementation for tree mode. 
    \retval TRUE if callback handled provided layout. */
    BOOL TreeModeCallbacks(LayoutInfo &layout);

  // Members
  protected:
    //! The parent window.
    TreePropSheet::CTreePropSheetEx* m_pTreePropSheet;
    //! Margin between frame and page in tree mode.
    CRect m_rectFramePageMargins;
  };

};
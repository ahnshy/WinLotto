// TreePropSheetUtil.hpp
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


#ifndef _TREEPROPSHEET_TREEPROPSHEETUTIL_HPP__INCLUDED_
#define _TREEPROPSHEET_TREEPROPSHEETUTIL_HPP__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TreePropSheet
{
	//////////////////////////////////////////////////////////////////////
  /*! \brief Scope class that prevent control redraw.
    
    This class prevents update of a window by sending WM_SETREDRAW.
     
    @author Yves Tkaczyk (yves@tkaczyk.net)
    @date 04/2004 */
  class CWindowRedrawScope
  {
  // Construction/Destruction
  public:
    //@{
    /*! 
    @param hWnd Handle of window for which update should be disabled.
    @param bInvalidateOnUnlock Flag indicating if window should be
                               invalidating after update is re-enabled. */
	  CWindowRedrawScope(HWND hWnd, const bool bInvalidateOnUnlock)
     : m_hWnd(hWnd),
       m_bInvalidateOnUnlock(bInvalidateOnUnlock)
    {
      ::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0L);
    }

    /*! 
      @param pWnd Window for which update should be disabled.
      @param bInvalidateOnUnlock Flag indicating if window should be
                                 invalidating after update is re-enabled. */
	  CWindowRedrawScope(CWnd* pWnd, const bool bInvalidateOnUnlock)
     : m_hWnd(pWnd->GetSafeHwnd()),
       m_bInvalidateOnUnlock(bInvalidateOnUnlock)
    {
      ::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0L);
    }
    //@}


	  ~CWindowRedrawScope()
    {
      ::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)TRUE, 0L);

      if( m_bInvalidateOnUnlock )
      {
        ::InvalidateRect(m_hWnd, NULL, TRUE);
      }
    }

  // Members
  private:
    /*! Handle of the window for which update should be disabled and enabled. */
    HWND m_hWnd;
    /*! Flag indicating if window should be invalidating after update is re-enabled. */
    const bool m_bInvalidateOnUnlock;
  };



	//////////////////////////////////////////////////////////////////////
  /*! \brief Scope class incrementing the provided value.
    
    This class increments the provided value in its contructor and
    decrement in destructor.
     
    @author Yves Tkaczyk (yves@tkaczyk.net)
    @date 04/2004 */
  class CIncrementScope
  {
  // Construction/Destruction
  public:
    /*! 
    @param rValue Reference to value to be manipulated.
    */
	  CIncrementScope(int& rValue)
     : m_value(rValue)
    {
      ++m_value;
    }

	  ~CIncrementScope()
    {
      --m_value;
    }

  // Members
  private:
    /*! Value to be manipulated. */
    int& m_value;
  };

	//////////////////////////////////////////////////////////////////////
  /*! \brief Helper class for managing background in property page.
    
    This class simply manage a state that can be used by each property 
    page to determine how the background should be drawn. 
    @author Yves Tkaczyk (yves@tkaczyk.net)
    @date 09/2004 */
  class CWhiteBackgroundProvider
  { 
  // Construction/Destruction
  public:
    CWhiteBackgroundProvider()
      : m_bHasWhiteBackground(false)
    {
    }

  protected:
    ~CWhiteBackgroundProvider()
    {
    }

  // Methods
  public:
    void SetHasWhiteBackground(const bool bHasWhiteBackground)
    {
      m_bHasWhiteBackground = bHasWhiteBackground;
    }

    bool HasWhiteBackground() const
    {
      return m_bHasWhiteBackground;
    }
  
  //Members
  private:
    /*! Flag indicating background drawing state. */
    bool m_bHasWhiteBackground;
  };

	//////////////////////////////////////////////////////////////////////
  /* \brief Expand/Collapse items in tree.

    ExpandTreeItem expand or collapse all the tree items under the provided 
    tree item.
  
    @param pTree THe tree control.
    @param htiThe tree item from which the opearation is applied.
    @param nCode Operation code:
                 - <pre>TVE_COLLAPSE</pre> Collapses the list.
                 - <pre>TVE_COLLAPSERESET</pre> Collapses the list and removes the child items.
                 - <pre>TVE_EXPAND</pre> Expands the list.
                 - <pre>TVE_TOGGLE</pre> Collapses the list if it is currently expanded or expands it if it is currently collapsed. 

    @author Yves Tkaczyk (yves@tkaczyk.net)
    @date 09/2004 */
  template<typename Tree>
    void ExpandTreeItem(Tree* pTree, HTREEITEM hti, UINT nCode)
  {
	  bool bOk= true;

	  bOk= pTree->Expand(hti, nCode) != 0;

	  if (bOk)
	  {
		  HTREEITEM htiChild= pTree->GetChildItem(hti);

		  while (htiChild != NULL)
		  {
			  ExpandTreeItem(pTree, htiChild, nCode);

			  htiChild= pTree->GetNextSiblingItem(htiChild);
		  }

	  }
		
    HTREEITEM htiSibling = pTree->GetNextSiblingItem(hti);
    if( htiSibling )
    {
      ExpandTreeItem(pTree, htiSibling, nCode);
    }
  }

};
#endif // _TREEPROPSHEET_TREEPROPSHEETUTIL_HPP__INCLUDED_
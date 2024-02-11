// TreePropSheetBase.h
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



#ifndef _TREEPROPSHEET_TREEPROPSHEETBASE_H__INCLUDED_
#define _TREEPROPSHEET_TREEPROPSHEETBASE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma warning (push)
#pragma warning (disable:4786)

#include "../PropertyFrame/PropPageFrame.h"
#include <afxtempl.h>
#include <map>

class CTreeCtrlEx; 

namespace TreePropSheet
{

  // Registered message to communicate with the library
  // (defined so that in the same executable it is initialized only once)
  
  // Message handled by CTreePropSheetBase to enable/disable property pages.
  // WPARAM: Pointer to property page
  // LPARAM: Enable/disable state
  const UINT WMU_ENABLEPAGE     = ::RegisterWindowMessage(_T("WMU_TPS_ENABLEPAGE"));

  // Message handled by the tree to change the color of the specified item.
  // WPARAM: HTREEITEM of the tree item.
  // LPARAM: COLORREF Color of the tree item text.
  const UINT WMU_ENABLETREEITEM = ::RegisterWindowMessage(_T("WMU_TPS_ENABLETREEITEM"));

  /////////////////////////////////////////////////////////////////////////////
  // utility functions
  inline BOOL Send_EnablePage(HWND hWndTarget, CPropertyPage* pPage, const bool bEnable)
  {
	  ASSERT(::IsWindow(hWndTarget));
    return (0 != ::SendMessage(hWndTarget, WMU_ENABLEPAGE, (WPARAM)pPage, (LPARAM)bEnable));
  }

/*! @brief Base class for CTreePropSheetEx
  Base class for property sheet allowing navigation using an
  optional tree control on the left of the window.

  See CTreePropSheet for more details on class functionality.

  @internal 
    CTreePropSheetBase is an update of CTreePropSheet. 
    Accessibility of several methods has been changed as well as 
    some implementation details. New functionality (such as
    skipping empty pages) has also been added. 
    This class was introduced in order 
    to leave CTreePropSheet intact hence allowing using 
    CTreePropSheet and CTreePropSheetEx in the same project 
    without risking to break the existing code using CTreePropSheet.

  @sa CTreePropSheet CTreePropSheetEx
  @version 0.1
  @author Yves Tkaczyk <yves@tkaczyk.net>
  @author Sven Wiegand (original CTreePropSheet)
  @date 07/2004 */
class CTreePropSheetBase
 : public CPropertySheet
{
	DECLARE_DYNAMIC(CTreePropSheetBase)

// Construction/Destruction
public:
	CTreePropSheetBase();
	CTreePropSheetBase(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTreePropSheetBase(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTreePropSheetBase();

// Operations
public:
	/**
	Call this method, if you would like to use a tree control to browse
	the pages, instead of the tab control.

	This method needs to be called, before DoModal() or Create(). If the 
	window has already been created, the method will fail.

	@param bTreeViewMode
		Pass TRUE to provide a tree view control instead of a tab control
		to browse the pages, pass FALSE to use the normal tab control.
	@param bPageCaption
		TRUE if a caption should be displayed for each page. The caption
		contains the page title and an icon if specified with the page.
		Ignored if bTreeViewMode is FALSE.
	@param bTreeImages
		TRUE if the page icons should be displayed in the page tree, 
		FALSE if there should be no icons in the page tree. Ignored if
		bTreeViewMode is FALSE. If not all of your pages are containing
		icons, or if there will be empty pages (parent nodes without a
		related page, you need to call SetTreeDefaultImages() to avoid 
		display errors.

	@return
		TRUE on success or FALSE, if the window has already been created.
	*/
	BOOL SetTreeViewMode(BOOL bTreeViewMode = TRUE, BOOL bPageCaption = FALSE, BOOL bTreeImages = FALSE);

  //@{
  /** Indicates if empty pages should be skipped when navigating the tree. 
  @param
    bSkipEmptyPages Skip empty pages if true. */
  void SetSkipEmptyPages(const bool bSkipEmptyPages);

  /** Returns true if empty pages are skipped and false if the default
      message is displayed. 
  @return True if empty pages are skipped, false if 'Empty Page Text' is 
          displayed. */
  bool IsSkippingEmptyPages() const;

  /**
	Specifies the text to be drawn on empty pages (pages for tree view
	items, that are not related to a page, because they are only 
	parents for other items). This is only needed in tree view mode.

	The specified text can contains a single "%s" placeholder which 
	will be replaced with the title of the empty page.
	*/
	void SetEmptyPageText(LPCTSTR lpszEmptyPageText);

	/**
	Allows you to specify, how the empty page message (see 
	SetEmptyPageText()) should be drawn.

	@param dwFormat
		A combination of the DT_* flags available for the Win32-API
		function DrawText(), that should be used to draw the text.
		The default value is:
		\code
		DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE
		\endcode

	@return
		The previous format.
	*/
	DWORD SetEmptyPageTextFormat(DWORD dwFormat);
  //@}

	//@{
	/**
	Defines the images, that should be used for pages without icons and
	for empty parent nodes. The list contains exactly to images:
	1. An image that should be used for parent tree nodes, without a
	   page assigned.
	2. An image that should be used for pages, which are not specifying
	   any icons.
	Standard image size is 16x16 Pixels, but if you call this method 
	before creating the sheet, the size of image 0 in this list will
	be assumed as your preferred image size and all other icons must
	have the same size.

	@param pImages
		Pointer to an image list with exactly to images, that should be
		used as default images. The images are copied to an internal 
		list, so that the given list can be deleted after this call.
	@param unBitmapID
		Resource identifier for the bitmap, that contains the default
		images. The resource should contain exactly to images.
	@param cx
		Width of a singe image in pixels.
	@param crMask
		Color that should be interpreted as transparent.
	
	@return
		TRUE on success, FALSE otherwise.
	*/
	BOOL SetTreeDefaultImages(CImageList *pImages);
	BOOL SetTreeDefaultImages(UINT unBitmapID, int cx, COLORREF crMask);
	//@}

	/**
	Returns a pointer to the tree control, when the sheet is in
	tree view mode, NULL otherwise.
  @return 
    Tree control if in tree mode, NULL otherwise.
	*/
	CTreeCtrl* GetPageTreeControl();

	/**
	Returns a pointer to the frame control, when the sheet is in
	tree view mode, NULL otherwise.
  @return 
    Frame control if in tree mode, NULL otherwise.
	*/
  CPropPageFrame* GetFrameControl();

  /** 
  Indicate if the sheet is in tree view mode.
    
  @return
    Returns true if in tree view mode, false otherwise. 
  */
  bool IsTreeViewMode() const;

  /**
  Returns the tree width set by SetTreeWidth if the tree control has not
  yet been created or the actual tree control width if the tree exists.

  @return
    Width of the tree control.
  */
  int GetTreeWidth() const;

  /** 
  Set if all items in the tree should be expanded when the tree
  is displayed. 
  @param bAutoExpandTree True to have the tree in expanded automatically. */
  void SetAutoExpandTree(const bool bAutoExpandTree);

  /**
  Returns true if the tree will be expanded automatically when displayed. 

  @return 
    True if the tree will be expanded automatically when displayed. Default 
    is false. */
  bool IsAutoExpandTree() const;

 	//@{
  /** 
  Enable/disable the specified property page. The property page window does not have to 
  exist. Once a page is disabled, the tree is updated and the page is disabled (i.e. 
  the user cannot edit control in the page. 
  If the current page is disabled, the next page is selected. Because of this, it is not 
  valid to disable all the property pages. In this case, disabling the last property page
  will fail.

  @param pPage Property page for which the status is to be changed.
  @param bEnable Desired new status (true for enabled, false for disabled).
  @return Previous enable status of the property page. */
  bool EnablePage(const CPropertyPage* const pPage, const bool bEnable);

  /** 
  Returns the enabled status of the specified property page. 

  @param pPage Property page for which the status is to be changed.
  @return True if the page is enabled, false if disabled. */
  bool IsPageEnabled(const CPropertyPage* const pPage) const;
	//@}


// Public helpers
public:
	//@{
	/**
	This helper allows you to easily set the icon of a property page.

	This static method does nothing more, than extracting the specified
	image as an icon from the given image list and assign the 
	icon-handle to the hIcon property of the pages PROPSHEETPAGE
	structure (m_psp) and modify the structures flags, so that the 
	image will be recognized.

	You need to call this method for a page, before adding the page
	to a property sheet.

	@attention
	If you are using the CImageList-version, you are responsible for
	destroying the extracted icon with DestroyIcon() or the static
	DestroyPageIcon() method.

	@see DestroyPageIcon()

	@param pPage
		Property page to set the image for.
	@param hIcon
		Handle to icon that should be set for the page.
	@param unIconId
		Resource identifier for the icon to set.
	@param Images
		Reference of the image list to extract the icon from.
	@param nImage
		Zero based index of the image in pImages, that should be used
		as an icon.

	@return
		TRUE on success, FALSE if an error occurred.
	*/
	static BOOL SetPageIcon(CPropertyPage *pPage, HICON hIcon);
	static BOOL SetPageIcon(CPropertyPage *pPage, UINT unIconId);
	static BOOL SetPageIcon(CPropertyPage *pPage, CImageList &Images, int nImage);
	//@}

	/**
	Checks, if the PSP_USEHICON flag is set in the PROPSHEETPAGE struct;
	If this is the case, the flag will be removed and the icon 
	specified by the hIcon attribute of the PROPSHEETPAGE struct will
	be destroyed using DestroyIcon().

	@note
	You only have to call DestroyIcon() for icons, that have been
	created using CreateIconIndirect() (i.e. used by 
	CImageList::ExtractIcon()).

	@return
		TRUE on success, FALSE if the PSP_USEHICON flag was not set or
		if the icon handle was NULL.
	*/
	static BOOL DestroyPageIcon(CPropertyPage *pPage);

// Overridables
public:
	/**
	Specifies the width of the tree control, when the sheet is in tree
	view mode. The default value (if this method is not called) is 150
	pixels.

	This method needs to be called, before DoModal() or Create(). 
	Otherwise it will fail.

	@param nWidth
		The width in pixels for the page tree. 

	@return
		TRUE on success, FALSE otherwise (if the window has already been
		created).

  @internal
    Made virtual in order to support resizing after the control has 
    been created.
	*/
	virtual BOOL SetTreeWidth(int nWidth);

// Overridable implementation helpers
protected:
	/**
	Will be called to generate the message, that should be displayed on
	an empty page, when the sheet is in tree view mode

	This default implementation simply returns lpszEmptyPageMessage 
	with the optional "%s" placeholder replaced by lpszCaption.

	@param lpszEmptyPageMessage
		The string, set by SetEmptyPageMessage(). This string may contain
		a "%s" placeholder.
	@param lpszCaption
		The title of the empty page.
	*/
	virtual CString GenerateEmptyPageMessage(LPCTSTR lpszEmptyPageMessage, LPCTSTR lpszCaption);

	/**
	Will be called during creation process, to create the CTreeCtrl
	object (the object, not the window!).

	Allows you to inject your own CTreeCtrl-derived classes.

	This default implementation simply creates a CTreeCtrl with new
	and returns it.
	*/
	virtual CTreeCtrl* CreatePageTreeObject();

	/**
	Will be called during creation process, to create the object, that
	is responsible for drawing the frame around the pages, drawing the
	empty page message and the caption.

	Allows you to inject your own CPropPageFrame-derived classes.

	This default implementation simply creates a CPropPageFrameTab with
	new and returns it.
	*/
	virtual CPropPageFrame* CreatePageFrame();

  /** 
  Method called for each tree item from RefreshPageTree. The default implementation
  only refreshes the text color of the item based on the enabled status of the 
  associated property page. Override this method if you want expand the behavior - 
  for instance refreshing the text itself.
  @param hItem Item to be refreshed. */
  virtual void RefreshTreeItem(HTREEITEM hItem);


// Implementation helpers
protected:
	/**
	Moves all children by the specified amount of pixels.

	@param nDx
		Pixels to move the children in horizontal direction (can be 
		negative).
	@param nDy
		Pixels to move the children in vertical direction (can be 
		negative).
	*/
	void MoveChildWindows(int nDx, int nDy);

	/**
	Refills the tree that contains the entries for the several pages.
  During this operation, the tree is not redrawn and it is refreshed
  once all the removal and insertion has been performed.
	*/
	void RefillPageTree();

  /** 
  Refreshes the tree control. This is called during enabling and 
  disabling a property page. This method calls RefreshTreeItem for 
  each item in the tree.
  @param hItem The specified item and its children to be refreshed.
  @param bRedraw Invalidate the tree upon exiting the method.
  */
  void RefreshPageTree(HTREEITEM hItem = TVI_ROOT, const bool bRedraw = true);

	/**
	Creates the specified path in the page tree and returns the handle
	of the most child item created.

	@param lpszPath
		Path of the item to create (see description of this class).
	@param hParent
		Handle of the item under which the path should be created or 
		TVI_ROOT to start from the root.
	*/
	HTREEITEM CreatePageTreeItem(LPCTSTR lpszPath, HTREEITEM hParent = TVI_ROOT);

	/**
	Splits the given path into the topmost item and the rest. See 
	description of this class for detailed path information.

	I.e. when given the string "Appearance::Toolbars::Customize", the
	method will return "Appearance" and after the call strRest will
	be "Toolbars::Customize".
	*/
	CString SplitPageTreePath(CString &strRest);

	/**
	Tries to deactivate the current page, and hides it if successful,
	so that an empty page becomes visible.
	
	@return
		TRUE if the current page has been deactivated successfully,
		FALSE if the currently active page prevents a page change.
	*/
	BOOL KillActiveCurrentPage();

	/**
	Returns the page tree item, that represents the specified page
	or NULL, if no such item exists.

	@param nPage
		Zero based page index, for which the item to retrieve.
	@param hRoot
		Item to start the search at or TVI_ROOT to search the whole
		tree.
	*/
	HTREEITEM GetPageTreeItem(int nPage, HTREEITEM hRoot = TVI_ROOT);

	/**
	Selects and shows the item, representing the specified page.

	@param nPage
		Zero based page index.

	@return
		TRUE on success, FALSE if no item does exist for the specified
		page.
	*/
	BOOL SelectPageTreeItem(int nPage);

	/**
	Selects and shows the tree item for the currently active page.

	@return
		TRUE on success, FALSE if no item exists for the currently active
		page or if it was not possible to get information about the 
		currently active page.
	*/
	BOOL SelectCurrentPageTreeItem();

	/**
	Updates the caption for the currently selected page (if the caption 
	is enabled).
	*/
	void UpdateCaption();

	/**
	Activates the previous page in the page order or the last one, if
	the current one is the first. The method takes into account the
  setting indicating if blank pages should be skipped. 

	This method does never fail.
	*/
	void ActivatePreviousPage();

 	/**
	Activates the page located before the page designated by the provided
  tree item. This call might be recursive. The method takes into account 
  the setting indicating if blank pages should be skipped. 

	This method does never fail.
	*/
  void ActivatePreviousPage(HTREEITEM hItem);

	/**
	Activates the next page in the page order or the first one, if the
	current one is the last. The method takes into account the
  setting indicating if blank pages should be skipped. 

	This method does never fail.
	*/
	void ActivateNextPage();

 	/**
	Activates the page located after the page designated by the provided
  tree item. This call might be recursive. The method takes into account 
  the setting indicating if blank pages should be skipped. 

	This method does never fail.
	*/
  void ActivateNextPage(HTREEITEM hItem);

  /** Helper method indicating if the sheet is in wizard mode.
  @return True if in wizard mode, false otherwise (in tab or 
          tree view mode). */
  bool IsWizardMode() const;

  /** Given a tree item, returns the index of the page representing 
      the next property page. The search is performed depth first.
      If there is no valid page, 0 is returned. 
    @param hTreeItem Tree item where the search should start.
    @return Index of the next valid page or 0 if none. */
  int GetNextValidPageIndex(HTREEITEM hTreeItem) const;

  /** Returns the next tree item, searching first for children then
      sibling.
    @param hItem ITem where the search starts.
    @return Next tree item or NULL if none. */
  HTREEITEM	GetNextItem(HTREEITEM hItem) const;
  
  /** Given a tree item, returns the tree item representing the next 
      property page. The search is performed depth first. If there 
      is no valid page, NULL is returned. 
    @param hTreeItem Tree item where the search should start.
    @return Tree item of the next valid page. */
  HTREEITEM GetNextValidPageTreeItem(HTREEITEM hTreeItem) const;

  /** Helper indicating if the provided tree item has a valid property
      page associated with it or an empty page. This is done by 
      retrieving the data item and if it is different from 
      kNoPageAssociated, the tree item has a property page associated.

    @param hItem Tree item to check.
    @return True if the tree item has a property page associated with it. */
  bool HasValidPropertyPageAssociated(HTREEITEM hItem) const;

  /** Helper indicating if the page associated with the item is enabled
      or disabled. 
    @param hItem Tree item to check.
    @return True if the tree item has not page or a page that is enabled,
            false if the page is disabled. */
  bool IsAssociatedPropertyPageEnabled(HTREEITEM hItem) const;

  /** Helper indicating if the provided item is expanded or collapsed.

    @param hItem Tree item to check.
    @return True if the item is expanded, false if not (or if it does not
            have any children. */
  bool IsItemExpanded(HTREEITEM hItem) const;

  /** Helper indicating if the specified tree item is displayable. This takes 
      into account the following:
      - check if item without property page should be displayed,
      - check if item with disabled property pages should be displayed.
      This is introduces so that if behavior is changed by introducing new checks, 
      only this method should need updating. 
    @param hItem Tree item to check
    @return True if the item is associated with a displayable page. */
  bool IsTreeItemDisplayable(const HTREEITEM hItem) const;

  /** Helper initializing members. */
  void Init();

// Overridings
protected:
	//{{AFX_VIRTUAL(CTreePropSheetBase)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Message handlers
protected:
	//{{AFX_MSG(CTreePropSheetBase)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnAddPage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemovePage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCurSel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCurSelId(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIsDialogMessage(WPARAM wParam, LPARAM lParam);

 	afx_msg void OnPageTreeSelChanging(NMHDR *pNotifyStruct, LRESULT *plResult);
	afx_msg void OnPageTreeSelChanged(NMHDR *pNotifyStruct, LRESULT *plResult);

	afx_msg LRESULT OnEnablePage(WPARAM wParam, LPARAM lParam);
  
  DECLARE_MESSAGE_MAP()

// Inner class
private:
  /*! \brief Class containing extra information regarding each property pages. */
  class CPageInformation
  {
  // Construction/Destruction
  public:
    CPageInformation() 
     : m_bEnabled(true)
    {
    }

  // Properties
    bool IsEnabled() const {return m_bEnabled; }
    void SetIsEnabled(const bool bEnabled) { m_bEnabled = bEnabled; }

  // Members
  private:
    /*! Flag indicating page enabled status. */
    bool m_bEnabled;
  };

  /*! Typedef for container of page information. */
  typedef std::map<const CPropertyPage* const, CPageInformation> tMapPageInformation;

// Properties
private:
  /** TRUE if we should use the tree control instead of the tab ctrl. */
	BOOL m_bTreeViewMode;

	/** The tree control */
	CTreeCtrl *m_pwndPageTree;

	/** The frame around the pages */
	CPropPageFrame *m_pFrame;

	/** TRUE if a page caption should be displayed, FALSE otherwise. */
	BOOL m_bPageCaption;

	/** TRUE if images should be displayed in the tree. */
	BOOL m_bTreeImages;

	/** Images to be displayed in the tree control. */
	CImageList m_Images;

	/** Default images. */
	CImageList m_DefaultImages;

	/** 
	Message to be displayed on empty pages. May contain a "%s" 
	placeholder which will be replaced by the caption of the empty 
	page.
	*/
	CString m_strEmptyPageMessage;

  /*! Value indicating that active page should not be set from the tree during
      tree content initialization. When the value is different from 0, 
      OnPageTreeSelChanging does not process new selection. This is set when
      the tree is filled (in RefillPageTree).
  */
  int m_nRefillingPageTreeContent;

protected:
  /** The width of the page tree control in pixels. */
	int m_nPageTreeWidth;

  /** Separator width. */
  int m_nSeparatorWidth;

  /*! Skip empty pages. */
  bool m_bSkipEmptyPages;

  /*! Tree in auto-expand mode. */
  bool m_bAutoExpandTree;

  /*! Map keep extra page information. */
  tMapPageInformation m_mapPageInformation;

  /*! Flag indicating if disabled pages should be skipped. */
  bool m_bSkipDisabledPages;

// Static Properties
private:
	/** The id of the tree view control, that shows the pages. */
	static const UINT s_unPageTreeId;
};


} //namespace TreePropSheet

//#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // _TREEPROPSHEET_TREEPROPSHEETBASE_H__INCLUDED_

// TreePropSheetBase.cpp
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
// This code is an update of CTreePropSheet written by Sven Wiegand and published 
// on code project at http://www.codetools.com/property/treepropsheet.asp.
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
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreePropSheetBase.h"
#include "../PropertyFrame/PropPageFrameEx.h"
#include "TreePropSheetTreeCtrl.h"
#include "HighColorTab.hpp"
#include "TreePropSheetUtil.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



namespace TreePropSheet
{
const DWORD kNoPageAssociated = 0xFFFFFFFF;         //! Value for tree item data when no page is associated in the tree. 

//-------------------------------------------------------------------
// class CTreePropSheetBase
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CTreePropSheetBase, CPropertySheet)
	//{{AFX_MSG_MAP(CTreePropSheetBase)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(PSM_ADDPAGE, OnAddPage)
	ON_MESSAGE(PSM_REMOVEPAGE, OnRemovePage)
	ON_MESSAGE(PSM_SETCURSEL, OnSetCurSel)
	ON_MESSAGE(PSM_SETCURSELID, OnSetCurSelId)
	ON_MESSAGE(PSM_ISDIALOGMESSAGE, OnIsDialogMessage)
	
	ON_NOTIFY(TVN_SELCHANGINGA,  s_unPageTreeId, OnPageTreeSelChanging)
	ON_NOTIFY(TVN_SELCHANGINGW,  s_unPageTreeId, OnPageTreeSelChanging)
	ON_NOTIFY(TVN_SELCHANGEDA,   s_unPageTreeId, OnPageTreeSelChanged)
	ON_NOTIFY(TVN_SELCHANGEDW,   s_unPageTreeId, OnPageTreeSelChanged)

  ON_REGISTERED_MESSAGE(WMU_ENABLEPAGE, OnEnablePage)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CTreePropSheetBase, CPropertySheet)

const UINT CTreePropSheetBase::s_unPageTreeId = 0x7EEE;

CTreePropSheetBase::CTreePropSheetBase()
:	CPropertySheet()
{
  Init();
}


CTreePropSheetBase::CTreePropSheetBase(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
:	CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  Init();
}


CTreePropSheetBase::CTreePropSheetBase(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:	CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  Init();
}

CTreePropSheetBase::~CTreePropSheetBase()
{
	if( m_Images.GetSafeHandle() )
    m_Images.DeleteImageList();

  //Fix for TN017
  if( m_pwndPageTree )
    delete m_pwndPageTree;
  m_pwndPageTree = NULL;
  
  if( m_pFrame )
    delete m_pFrame;
  m_pFrame = NULL;
}


/////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////

BOOL CTreePropSheetBase::SetTreeViewMode(BOOL bTreeViewMode /* = TRUE */, BOOL bPageCaption /* = FALSE */, BOOL bTreeImages /* = FALSE */)
{
	if (IsWindow(m_hWnd))
	{
		// needs to be called, before the window has been created
		ASSERT(FALSE);
		return FALSE;
	}

	m_bTreeViewMode = bTreeViewMode;
	if (m_bTreeViewMode)
	{
		m_bPageCaption = bPageCaption;
		m_bTreeImages = bTreeImages;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SetTreeWidth(int nWidth)
{
	if (IsWindow(m_hWnd))
	{
		// needs to be called, before the window is created.
		ASSERT(FALSE);
		return FALSE;
	}

	m_nPageTreeWidth = nWidth;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::SetEmptyPageText(LPCTSTR lpszEmptyPageText)
{
	m_strEmptyPageMessage = lpszEmptyPageText;
}


/////////////////////////////////////////////////////////////////////
//
DWORD	CTreePropSheetBase::SetEmptyPageTextFormat(DWORD dwFormat)
{
	DWORD	dwPrevFormat = m_pFrame->GetMsgFormat();
	m_pFrame->SetMsgFormat(dwFormat);
	return dwPrevFormat;
}


/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SetTreeDefaultImages(CImageList *pImages)
{
	if (pImages->GetImageCount() != 2)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_DefaultImages.GetSafeHandle())
		m_DefaultImages.DeleteImageList();
	m_DefaultImages.Create(pImages);

	// update, if necessary
	if (IsWindow(m_hWnd))
		RefillPageTree();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SetTreeDefaultImages(UINT unBitmapID, int cx, COLORREF crMask)
{
	if (m_DefaultImages.GetSafeHandle())
		m_DefaultImages.DeleteImageList();
	if (!m_DefaultImages.Create(unBitmapID, cx, 0, crMask))
		return FALSE;

	if (m_DefaultImages.GetImageCount() != 2)
	{
		m_DefaultImages.DeleteImageList();
		return FALSE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////
//
CTreeCtrl* CTreePropSheetBase::GetPageTreeControl()
{
	return m_pwndPageTree;
}

/////////////////////////////////////////////////////////////////////
//
CPropPageFrame* CTreePropSheetBase::GetFrameControl()
{
  return m_bTreeViewMode?m_pFrame:NULL;
}

/////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsTreeViewMode() const
{
  return m_bTreeViewMode?true:false;
}

/////////////////////////////////////////////////////////////////////
//
int CTreePropSheetBase::GetTreeWidth() const
{
  if( m_pwndPageTree->GetSafeHwnd() )
  {
    CRect rectTree;
    m_pwndPageTree->GetWindowRect( &rectTree );
    return rectTree.Width();
  }
  else
  {
    return m_nPageTreeWidth;
  }
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::SetAutoExpandTree(const bool bAutoExpandTree)
{
	m_bAutoExpandTree = bAutoExpandTree;
}

/////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsAutoExpandTree() const
{
	return m_bAutoExpandTree;
}

/////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::EnablePage(const CPropertyPage* const pPage,const bool bEnable)
{
	/* - Based on flag value:
       - If disabling, if the current page is the page that is being disabled, 
         activate the next page. If the new page is the initial
         page, the call fails as there is no available new page. If the current
         is not the page being disabled, just disable it. Act accordingly
         and set flag, return previous status.
       - If enabling, just switch the flag.
    - Refresh tree. */
  CPageInformation& pageInformation = m_mapPageInformation[pPage];
  bool bPreviousStatus = pageInformation.IsEnabled();

  if( bEnable )
  {
    pageInformation.SetIsEnabled( true );
  }
  else
  {
    // Active next page if disabling the current page.
    if( GetActivePage() == pPage )
    {
      ActivateNextPage();
      // This is the only active page left, it cannot be disabled.
      if( GetActivePage() == pPage )
      {
        return bPreviousStatus;
      }
    }
    // We are now sure that the active page is not the page being disabled, 
    // we can proceed.
    pageInformation.SetIsEnabled( false );
  }

  // Enable/disable the page if it exists.
  if( pPage->GetSafeHwnd() )
  {
    ::EnableWindow( pPage->GetSafeHwnd(), bEnable?TRUE:FALSE );
  }

  // Refresh the tree control.
  RefreshPageTree();

  return bPreviousStatus;
}

/////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsPageEnabled(const CPropertyPage* const pPage) const
{
  /* If the page is in the information map, return the status from the map 
     otherwise returns true (the page has never been disabled. Implementation 
     is such that querying the page status will not any unnecessary add entry 
     into the map.*/
  tMapPageInformation::const_iterator citPage = m_mapPageInformation.find( pPage );
  if( m_mapPageInformation.end() == citPage )
    return true; 

  return citPage->second.IsEnabled();
}

/////////////////////////////////////////////////////////////////////
// public helpers
/////////////////////////////////////////////////////////////////////

BOOL CTreePropSheetBase::SetPageIcon(CPropertyPage *pPage, HICON hIcon)
{
	pPage->m_psp.dwFlags|= PSP_USEHICON;
	pPage->m_psp.hIcon = hIcon;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SetPageIcon(CPropertyPage *pPage, UINT unIconId)
{
	HICON	hIcon = AfxGetApp()->LoadIcon(unIconId);
	if (!hIcon)
		return FALSE;

	return SetPageIcon(pPage, hIcon);
}


/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SetPageIcon(CPropertyPage *pPage, CImageList &Images, int nImage)
{
	HICON	hIcon = Images.ExtractIcon(nImage);
	if (!hIcon)
		return FALSE;

	return SetPageIcon(pPage, hIcon);
}


/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::DestroyPageIcon(CPropertyPage *pPage)
{
	if (!pPage || !(pPage->m_psp.dwFlags&PSP_USEHICON) || !pPage->m_psp.hIcon)
		return FALSE;

	DestroyIcon(pPage->m_psp.hIcon);
	pPage->m_psp.dwFlags&= ~PSP_USEHICON;
	pPage->m_psp.hIcon = NULL;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////
// Overridable implementation helpers
/////////////////////////////////////////////////////////////////////

CString CTreePropSheetBase::GenerateEmptyPageMessage(LPCTSTR lpszEmptyPageMessage, LPCTSTR lpszCaption)
{
	CString	strMsg;
	strMsg.Format(lpszEmptyPageMessage, lpszCaption);
	return strMsg;
}


/////////////////////////////////////////////////////////////////////
//
CTreeCtrl* CTreePropSheetBase::CreatePageTreeObject()
{
  // Create the tree control.
  return new CTreePropSheetTreeCtrl;
}

/////////////////////////////////////////////////////////////////////
//
CPropPageFrame* CTreePropSheetBase::CreatePageFrame()
{
  return new CPropPageFrameEx;
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::RefreshTreeItem(HTREEITEM hItem)
{
	ASSERT(hItem);
	if (hItem)
	{
		DWORD dwItemData = m_pwndPageTree->GetItemData( hItem );
    if( kNoPageAssociated != dwItemData )
    {
      // Set font if disabled.
      CPropertyPage* pPage = GetPage( dwItemData );
      ASSERT( pPage );
      if( NULL != pPage )
      {
        // Set text color
        ::SendMessage( m_pwndPageTree->GetSafeHwnd(), WMU_ENABLETREEITEM, (WPARAM)hItem, (LPARAM)( IsPageEnabled( pPage ) ? m_pwndPageTree->GetTextColor():GetSysColor( COLOR_GRAYTEXT ) ) );
      }
    }
	}
}

/////////////////////////////////////////////////////////////////////
// Implementation helpers
/////////////////////////////////////////////////////////////////////

void CTreePropSheetBase::MoveChildWindows(int nDx, int nDy)
{
	CWnd	*pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		CRect	rect;
		pWnd->GetWindowRect(rect);
		rect.OffsetRect(nDx, nDy);
		ScreenToClient(rect);
		pWnd->MoveWindow(rect);

		pWnd = pWnd->GetNextWindow();
	}
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::RefillPageTree()
{
	if (!IsWindow(m_hWnd))
		return;

  // TreePropSheetEx: OnPageTreeSelChanging does not process message.
  TreePropSheet::CIncrementScope RefillingPageTreeContentGuard( m_nRefillingPageTreeContent );
  // TreePropSheetEx: End OnPageTreeSelChanging does not process message.

	m_pwndPageTree->DeleteAllItems();

	CTabCtrl	*pTabCtrl = GetTabControl();
	if (!IsWindow(pTabCtrl->GetSafeHwnd()))
	{
		ASSERT(FALSE);
		return;
	}

	const int	nPageCount = pTabCtrl->GetItemCount();

	// rebuild image list
	if (m_bTreeImages)
	{
		for (int i = m_Images.GetImageCount()-1; i >= 0; --i)
			m_Images.Remove(i);

		// add page images
		CImageList	*pPageImages = pTabCtrl->GetImageList();
		if (pPageImages)
		{
			for (int nImage = 0; nImage < pPageImages->GetImageCount(); ++nImage)
			{
				HICON	hIcon = pPageImages->ExtractIcon(nImage);
				m_Images.Add(hIcon);
				DestroyIcon(hIcon);
			}
		}

		// add default images
		if (m_DefaultImages.GetSafeHandle())
		{	
			HICON	hIcon;

			// add default images
			hIcon = m_DefaultImages.ExtractIcon(0);
			if (hIcon)
			{
				m_Images.Add(hIcon);
				DestroyIcon(hIcon);
			}
			hIcon = m_DefaultImages.ExtractIcon(1);
			{
				m_Images.Add(hIcon);
				DestroyIcon(hIcon);
			}
		}
	}

	// insert tree items
	for (int nPage = 0; nPage < nPageCount; ++nPage)
	{
		// Get title and image of the page
		CString	strPagePath;

		TCITEM	ti;
		ZeroMemory(&ti, sizeof(ti));
		ti.mask = TCIF_TEXT|TCIF_IMAGE;
		ti.cchTextMax = MAX_PATH;
		ti.pszText = strPagePath.GetBuffer(ti.cchTextMax);
		ASSERT(ti.pszText);
		if (!ti.pszText)
			return;

		pTabCtrl->GetItem(nPage, &ti);
		strPagePath.ReleaseBuffer();

		// Create an item in the tree for the page
		HTREEITEM	hItem = CreatePageTreeItem(ti.pszText);
		ASSERT(hItem);
		if (hItem)
		{
			m_pwndPageTree->SetItemData(hItem, nPage);

			// set image
			if (m_bTreeImages)
			{
				int	nImage = ti.iImage;
				if (nImage < 0 || nImage >= m_Images.GetImageCount())
					nImage = m_DefaultImages.GetSafeHandle()? m_Images.GetImageCount()-1 : -1;

				m_pwndPageTree->SetItemImage(hItem, nImage, nImage);
			}

      // Set font if disabled.
      CPropertyPage* pPage = GetPage( nPage );
      ASSERT( pPage );
      if( NULL != pPage && !IsPageEnabled( pPage ) )
      {
        ::SendMessage( m_pwndPageTree->GetSafeHwnd(), WMU_ENABLETREEITEM, (WPARAM)hItem, (LPARAM)GetSysColor(COLOR_GRAYTEXT) );
      }
		}
	}
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::RefreshPageTree(HTREEITEM hItem /* = TVI_ROOT */, const bool bRedraw /* = true */)
{
  // If TVI_ROOT, get the first item.
  if (hItem == TVI_ROOT)
  {
    hItem = m_pwndPageTree->GetNextItem(NULL, TVGN_ROOT);
  }

  while( hItem )
	{
		RefreshTreeItem( hItem );
		if( m_pwndPageTree->ItemHasChildren( hItem ) )
		{
      RefreshPageTree( m_pwndPageTree->GetNextItem(hItem, TVGN_CHILD), false );
		}

		hItem = m_pwndPageTree->GetNextItem(hItem, TVGN_NEXT);
	}

  if( bRedraw )
  {
    m_pwndPageTree->Invalidate( TRUE );
  }
}

/////////////////////////////////////////////////////////////////////
//
HTREEITEM CTreePropSheetBase::CreatePageTreeItem(LPCTSTR lpszPath, HTREEITEM hParent /* = TVI_ROOT */)
{
	CString		strPath(lpszPath);
	CString		strTopMostItem(SplitPageTreePath(strPath));
	
	// Check if an item with the given text does already exist
	HTREEITEM	hItem = NULL;
	HTREEITEM	hChild = m_pwndPageTree->GetChildItem(hParent);
	while (hChild)
	{
		if (m_pwndPageTree->GetItemText(hChild) == strTopMostItem)
		{
			hItem = hChild;
			break;
		}
		hChild = m_pwndPageTree->GetNextItem(hChild, TVGN_NEXT);
	}

	// If item with that text does not already exist, create a new one
	if (!hItem)
	{
		hItem = m_pwndPageTree->InsertItem(strTopMostItem, hParent);
		m_pwndPageTree->SetItemData(hItem, kNoPageAssociated);
		if (!strPath.IsEmpty() && m_bTreeImages && m_DefaultImages.GetSafeHandle())
			// set folder image
			m_pwndPageTree->SetItemImage(hItem, m_Images.GetImageCount()-2, m_Images.GetImageCount()-2);
	}
	if (!hItem)
	{
		ASSERT(FALSE);
		return NULL;
	}

	if (strPath.IsEmpty())
		return hItem;
	else
		return CreatePageTreeItem(strPath, hItem);
}

/////////////////////////////////////////////////////////////////////
//
CString CTreePropSheetBase::SplitPageTreePath(CString &strRest)
{
	int	nSeperatorPos = 0;
	while (TRUE)
	{
		nSeperatorPos = strRest.Find(_T("::"), nSeperatorPos);
		if (nSeperatorPos == -1)
		{
			CString	strItem(strRest);
			strRest.Empty();
			return strItem;
		}
		else if (nSeperatorPos>0)
		{
			// if there is an odd number of backslashes in front of the
			// separator, than do not interpret it as separator
			int	nBackslashCount = 0;
			for (int nPos = nSeperatorPos-1; nPos >= 0 && strRest[nPos]==_T('\\'); --nPos, ++nBackslashCount);
			if (nBackslashCount%2 == 0)
				break;
			else
				++nSeperatorPos;
		}
	}

	CString	strItem(strRest.Left(nSeperatorPos));
	strItem.Replace(_T("\\::"), _T("::"));
	strItem.Replace(_T("\\\\"), _T("\\"));
	strRest = strRest.Mid(nSeperatorPos+2);
	return strItem;
}

/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::KillActiveCurrentPage()
{
	HWND	hCurrentPage = PropSheet_GetCurrentPageHwnd(m_hWnd);
	if (!IsWindow(hCurrentPage))
	{
		ASSERT(FALSE);
		return TRUE;
	}

	// Check if the current page is really active (if page is invisible
	// an virtual empty page is the active one.
	if (!::IsWindowVisible(hCurrentPage))
		return TRUE;

	// Try to deactivate current page
	PSHNOTIFY	pshn;
	pshn.hdr.code = PSN_KILLACTIVE;
	pshn.hdr.hwndFrom = m_hWnd;
	pshn.hdr.idFrom = GetDlgCtrlID();
	pshn.lParam = 0;
	if (::SendMessage(hCurrentPage, WM_NOTIFY, pshn.hdr.idFrom, (LPARAM)&pshn))
		// current page does not allow page change
		return FALSE;

	// Hide the page
	::ShowWindow(hCurrentPage, SW_HIDE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//
HTREEITEM CTreePropSheetBase::GetPageTreeItem(int nPage, HTREEITEM hRoot /* = TVI_ROOT */)
{
	// Special handling for root case
	if (hRoot == TVI_ROOT)
		hRoot = m_pwndPageTree->GetNextItem(NULL, TVGN_ROOT);

	// Check parameters
	if (nPage < 0 || nPage >= GetPageCount())
	{
		ASSERT(FALSE);
		return NULL;
	}

	if (hRoot == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	// we are performing a simple linear search here, because we are
	// expecting only little data
	HTREEITEM	hItem = hRoot;
	while (hItem)
	{
		if ((signed)m_pwndPageTree->GetItemData(hItem) == nPage)
			return hItem;
		if (m_pwndPageTree->ItemHasChildren(hItem))
		{
			HTREEITEM	hResult = GetPageTreeItem(nPage, m_pwndPageTree->GetNextItem(hItem, TVGN_CHILD));
			if (hResult)
				return hResult;
		}

		hItem = m_pwndPageTree->GetNextItem(hItem, TVGN_NEXT);
	}

	// we've found nothing, if we arrive here
	return hItem;
}

/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SelectPageTreeItem(int nPage)
{
  ASSERT( -1 != nPage );

  HTREEITEM	hItem = GetPageTreeItem(nPage);
	if (!hItem)
		return FALSE;

	return m_pwndPageTree->SelectItem(hItem);
}

/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::SelectCurrentPageTreeItem()
{
	CTabCtrl	*pTab = GetTabControl();
	if (!IsWindow(pTab->GetSafeHwnd()))
		return FALSE;

  // TreePropSheetEx: Fix problem when removing last page from control.
  int nPage = pTab->GetCurSel();
  if( nPage >= 0 && nPage < pTab->GetItemCount() )
    return SelectPageTreeItem( nPage );
  // TreePropSheetEx: End fix problem when removing last page from control.

  return FALSE;
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::UpdateCaption()
{
	HWND			hPage = PropSheet_GetCurrentPageHwnd(GetSafeHwnd());
	BOOL			bRealPage = IsWindow(hPage) && ::IsWindowVisible(hPage);
	HTREEITEM	hItem = m_pwndPageTree->GetSelectedItem();
	if (!hItem)
		return;
	CString		strCaption = m_pwndPageTree->GetItemText(hItem);

	// if empty page, then update empty page message
	if (!bRealPage)
		m_pFrame->SetMsgText(GenerateEmptyPageMessage(m_strEmptyPageMessage, strCaption));

	// if no captions are displayed, cancel here
	if (!m_pFrame->GetShowCaption())
		return;

	// get tab control, to the the images from
	CTabCtrl	*pTabCtrl = GetTabControl();
	if (!IsWindow(pTabCtrl->GetSafeHwnd()))
	{
		ASSERT(FALSE);
		return;
	}

	if (m_bTreeImages)
	{
		// get image from tree
		int	nImage;
		m_pwndPageTree->GetItemImage(hItem, nImage, nImage);
		HICON	hIcon = m_Images.ExtractIcon(nImage);
		m_pFrame->SetCaption(strCaption, hIcon);
		if (hIcon)
			DestroyIcon(hIcon);
	}
	else if (bRealPage)
	{
		// get image from hidden (original) tab provided by the original
		// implementation
		CImageList	*pImages = pTabCtrl->GetImageList();
		if (pImages)
		{
			TCITEM	ti;
			ZeroMemory(&ti, sizeof(ti));
			ti.mask = TCIF_IMAGE;

			HICON	hIcon = NULL;
			if (pTabCtrl->GetItem((int)m_pwndPageTree->GetItemData(hItem), &ti))
				hIcon = pImages->ExtractIcon(ti.iImage);

			m_pFrame->SetCaption(strCaption, hIcon);
			if (hIcon)
				DestroyIcon(hIcon);
		}
		else
			m_pFrame->SetCaption(strCaption);
	}
	else
		m_pFrame->SetCaption(strCaption);
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::ActivatePreviousPage()
{
	if( !IsWindow(m_hWnd) || 0 == GetPageCount() )
		return;

	if (!IsWindow(m_pwndPageTree->GetSafeHwnd()))
	{
		// normal tab property sheet. Simply use page index
		int	nPageIndex = GetActiveIndex();
		if (nPageIndex<0 || nPageIndex>=GetPageCount())
			return;

		int	nPrevIndex = (nPageIndex==0)? GetPageCount()-1 : nPageIndex-1;
		SetActivePage(nPrevIndex);
	}
	else
	{
		HTREEITEM	hItem = m_pwndPageTree->GetSelectedItem();
    ActivatePreviousPage( hItem );
	}
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::ActivatePreviousPage(HTREEITEM hItem)
{
		// property sheet with page tree.
		// we need a more sophisticated handling here, than simply using
		// the page index, because we won't skip empty pages.
		// so we have to walk the page tree
		ASSERT(hItem);
		if (!hItem)
			return;

		HTREEITEM	hPrevItem = m_pwndPageTree->GetPrevSiblingItem(hItem);
		if( hPrevItem )
		{
			while (m_pwndPageTree->ItemHasChildren(hPrevItem))
			{
				hPrevItem = m_pwndPageTree->GetChildItem(hPrevItem);
				while (m_pwndPageTree->GetNextSiblingItem(hPrevItem))
					hPrevItem = m_pwndPageTree->GetNextSiblingItem(hPrevItem);
			}
		}
		else 
			hPrevItem=m_pwndPageTree->GetParentItem(hItem);

		if (!hPrevItem)
		{
			// no prev item, so cycle to the last item
			hPrevItem = m_pwndPageTree->GetRootItem();

			while (TRUE)
			{
				while (m_pwndPageTree->GetNextSiblingItem(hPrevItem))
					hPrevItem = m_pwndPageTree->GetNextSiblingItem(hPrevItem);

				if (m_pwndPageTree->ItemHasChildren(hPrevItem))
					hPrevItem = m_pwndPageTree->GetChildItem(hPrevItem);
				else
					break;
			}
		}

    if (!hPrevItem)
      return;

    // If we skip the empty pages and the tree item does not point to 
    // a property page, call ActivateNextPage again. This is fine since
    // we know there is at least one valid property page in the sheet.
    if( IsTreeItemDisplayable( hPrevItem ) )
			m_pwndPageTree->SelectItem( hPrevItem );
    else
      ActivatePreviousPage( hPrevItem );
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::ActivateNextPage()
{
  // Window must be defined and a least one property page.
	if (!IsWindow(m_hWnd) || 0 == GetPageCount() )
		return;

	if (!IsWindow(m_pwndPageTree->GetSafeHwnd()))
	{
		// normal tab property sheet. Simply use page index
		int	nPageIndex = GetActiveIndex();
		if (nPageIndex<0 || nPageIndex>=GetPageCount())
			return;

		int	nNextIndex = (nPageIndex==GetPageCount()-1)? 0 : nPageIndex+1;
		SetActivePage(nNextIndex);
	}
	else
	{
		HTREEITEM	hItem = m_pwndPageTree->GetSelectedItem();
    ActivateNextPage( hItem );
	}
}

/////////////////////////////////////////////////////////////////////
//
#pragma warning(push)
#pragma warning(disable:4706)

void CTreePropSheetBase::ActivateNextPage(HTREEITEM hItem)
{
		// property sheet with page tree.
		// we need a more sophisticated handling here, than simply using
		// the page index, because we won't skip empty pages.
		// so we have to walk the page tree
		ASSERT(hItem);
		if (!hItem)
			return;

		HTREEITEM	hNextItem = m_pwndPageTree->GetChildItem(hItem);
		if ( hNextItem )
    {
      // Have a child, skip rest of search.
    } 
		else if (hNextItem=m_pwndPageTree->GetNextSiblingItem(hItem))
    {
      // Had not child but has a sibling, skip rest of search.
    }
		else if (m_pwndPageTree->GetParentItem(hItem))
		{
      // No child, no sibling, get the next sibling from the parent.
			while (!hNextItem) 
			{
				hItem = m_pwndPageTree->GetParentItem(hItem);
				if (!hItem)
					break;

				hNextItem	= m_pwndPageTree->GetNextSiblingItem(hItem);
			}
		}

		if (!hNextItem)
			// no next item -- so cycle to the first item
			hNextItem = m_pwndPageTree->GetRootItem();

		if (!hNextItem)
      return;

    // If we skip the empty pages and the tree item does not point to 
    // a property page, call ActivateNextPage again. This is fine since
    // we know there is at least one valid property page in the sheet.
    if( IsTreeItemDisplayable( hNextItem ) )
			m_pwndPageTree->SelectItem( hNextItem );
    else
      ActivateNextPage( hNextItem );
}

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsWizardMode() const
{
  return PSH_WIZARD & m_psh.dwFlags?true:false;
}

//////////////////////////////////////////////////////////////////////
//
int CTreePropSheetBase::GetNextValidPageIndex(HTREEITEM hTreeItem) const
{
	HTREEITEM hNextItem = GetNextValidPageTreeItem( hTreeItem );
  if( NULL == hNextItem )
    return 0;

  return m_pwndPageTree->GetItemData( hNextItem );
}

//////////////////////////////////////////////////////////////////////
//
HTREEITEM CTreePropSheetBase::GetNextItem(HTREEITEM hItem) const
{
  if( m_pwndPageTree->ItemHasChildren(hItem) )
	{
		return m_pwndPageTree->GetNextItem(hItem, TVGN_CHILD);
	}

	return m_pwndPageTree->GetNextItem(hItem, TVGN_NEXT);
}

//////////////////////////////////////////////////////////////////////
//
HTREEITEM CTreePropSheetBase::GetNextValidPageTreeItem(HTREEITEM hTreeItem) const
{
	if (hTreeItem == TVI_ROOT)
		hTreeItem = m_pwndPageTree->GetNextItem(NULL, TVGN_ROOT);

	if (hTreeItem == NULL)
	{
		return NULL;
	}

	HTREEITEM	hItem = GetNextItem( hTreeItem );
	while (hItem)
	{
		if( IsTreeItemDisplayable( hItem ) )
			return hItem;

    if (m_pwndPageTree->ItemHasChildren(hItem))
		{
			HTREEITEM	hResult = GetNextValidPageTreeItem( m_pwndPageTree->GetNextItem(hItem, TVGN_CHILD) );
			if( hResult )
				return hResult;
		}

		hItem = m_pwndPageTree->GetNextItem(hItem, TVGN_NEXT);
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::HasValidPropertyPageAssociated(HTREEITEM hItem) const
{
	if( NULL == m_pwndPageTree || NULL == hItem )
    return false;

  return kNoPageAssociated != (int)m_pwndPageTree->GetItemData( hItem );
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsAssociatedPropertyPageEnabled( HTREEITEM hItem ) const
{
	DWORD dwItemData = m_pwndPageTree->GetItemData( hItem );
  if( kNoPageAssociated == dwItemData )
  {
    return true;
  }

  CPropertyPage* pPage = GetPage( dwItemData );
  // ASSERT: No page associated with this item data.
  ASSERT( pPage );
  if( NULL == pPage )
    return true;

  return IsPageEnabled( pPage );
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsItemExpanded(HTREEITEM hItem) const
{
	if( NULL == m_pwndPageTree || NULL == hItem )
    return false;

  return ( TVIS_EXPANDED | TVIS_EXPANDPARTIAL) &
         m_pwndPageTree->GetItemState( hItem, TVIS_EXPANDED | TVIS_EXPANDPARTIAL )?true:false;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsTreeItemDisplayable(const HTREEITEM hItem) const
{
  // Determine if the provided hItem has a property page associated with it.
  bool bHasValidPropertyPageAssociated = HasValidPropertyPageAssociated( hItem );
	
  bool bDisplayEmpty = bHasValidPropertyPageAssociated || m_bSkipEmptyPages ;
  bool bDisplayDisabled = !bHasValidPropertyPageAssociated || IsAssociatedPropertyPageEnabled( hItem ) || !m_bSkipDisabledPages;
  return bDisplayEmpty && bDisplayDisabled;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::Init()
{
	m_bTreeViewMode = FALSE;
	m_bPageCaption = FALSE;
	m_bTreeImages = FALSE;
	m_nPageTreeWidth = 150;
  m_nSeparatorWidth = 5;
	m_pwndPageTree = NULL;
	m_pFrame = NULL;
  m_nRefillingPageTreeContent = 0;
  m_bAutoExpandTree = false;
  m_bSkipDisabledPages = true;
  m_bSkipEmptyPages = false;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::SetSkipEmptyPages(const bool bSkipEmptyPages)
{
	m_bSkipEmptyPages = bSkipEmptyPages;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetBase::IsSkippingEmptyPages() const
{
	return m_bSkipEmptyPages;
}

/////////////////////////////////////////////////////////////////////
// Overridings
/////////////////////////////////////////////////////////////////////

BOOL CTreePropSheetBase::OnInitDialog() 
{
	if (m_bTreeViewMode && !IsWizardMode() )
	{
	  // Fix suggested by Przemek Miszczuk 
	  // http://www.codeproject.com/property/TreePropSheetEx.asp?msg=1024928#xx1024928xx
    TreePropSheet::CIncrementScope RefillingPageTreeContentGuard(m_nRefillingPageTreeContent );

		// be sure, there are no stacked tabs, because otherwise the
		// page caption will be to large in tree view mode
		EnableStackedTabs(FALSE);

		// Initialize image list.
		if (m_DefaultImages.GetSafeHandle())
		{
			IMAGEINFO	ii;
			m_DefaultImages.GetImageInfo(0, &ii);
			if (ii.hbmImage) DeleteObject(ii.hbmImage);
			if (ii.hbmMask) DeleteObject(ii.hbmMask);
			m_Images.Create(ii.rcImage.right-ii.rcImage.left, ii.rcImage.bottom-ii.rcImage.top, ILC_COLOR32|ILC_MASK, 0, 1);
		}
		else
			m_Images.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 1);
	}

	// perform default implementation
	BOOL bResult = CPropertySheet::OnInitDialog();

  // If in wizard mode, stop here.
  if( IsWizardMode() )
    return bResult;

	// Get tab control...
	CTabCtrl	*pTab = GetTabControl();
	if (!IsWindow(pTab->GetSafeHwnd()))
	{
		ASSERT(FALSE);
		return bResult;
	}

  // HighColorTab::UpdateImageList to change the internal image list to 24 bits colors)
  HighColorTab::UpdateImageList( *this );

	// If not in tree mode, stop here.
  if (!m_bTreeViewMode)
		// stop here, if we would like to use tabs
		return bResult;

	// ... and hide it
	pTab->ShowWindow(SW_HIDE);
	pTab->EnableWindow(FALSE);

	// Place another (empty) tab ctrl, to get a frame instead
	CRect	rectFrame;
	pTab->GetWindowRect(rectFrame);
	ScreenToClient(rectFrame);

	m_pFrame = CreatePageFrame();
	if (!m_pFrame)
	{
		ASSERT(FALSE);
		AfxThrowMemoryException();
	}
	m_pFrame->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rectFrame, this, 0xFFFF);
	m_pFrame->ShowCaption(m_bPageCaption);

	// Lets make place for the tree ctrl
	const int	nTreeWidth = m_nPageTreeWidth;

	CRect	rectSheet;
	GetWindowRect(rectSheet);
	rectSheet.right+= nTreeWidth;
	SetWindowPos(NULL, -1, -1, rectSheet.Width(), rectSheet.Height(), SWP_NOZORDER|SWP_NOMOVE);
	CenterWindow();

	MoveChildWindows(nTreeWidth, 0);

	// Lets calculate the rectangle for the tree ctrl
	CRect	rectTree(rectFrame);
	rectTree.right = rectTree.left + nTreeWidth - m_nSeparatorWidth;

	// calculate caption height
	CTabCtrl	wndTabCtrl;
	wndTabCtrl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rectFrame, this, 0x1234);
	wndTabCtrl.InsertItem(0, _T(""));
	CRect	rectFrameCaption;
	wndTabCtrl.GetItemRect(0, rectFrameCaption);
	wndTabCtrl.DestroyWindow();
	m_pFrame->SetCaptionHeight(rectFrameCaption.Height());

	// if no caption should be displayed, make the window smaller in
	// height
	if (!m_bPageCaption)
	{
		// make frame smaller
		m_pFrame->GetWnd()->GetWindowRect(rectFrame);
		ScreenToClient(rectFrame);
		rectFrame.top+= rectFrameCaption.Height();
		m_pFrame->GetWnd()->MoveWindow(rectFrame);

		// move all child windows up
		MoveChildWindows(0, -rectFrameCaption.Height());

		// modify rectangle for the tree ctrl
		rectTree.bottom-= rectFrameCaption.Height();

		// make us smaller
		CRect	rect;
		GetWindowRect(rect);
		rect.top+= rectFrameCaption.Height()/2;
		rect.bottom-= rectFrameCaption.Height()-rectFrameCaption.Height()/2;
		if (GetParent())
			GetParent()->ScreenToClient(rect);
		MoveWindow(rect);
    CenterWindow();
	}

	// finally create the tree control
	const DWORD	dwTreeStyle = TVS_SHOWSELALWAYS|TVS_TRACKSELECT|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS;
	m_pwndPageTree = CreatePageTreeObject();
	if (!m_pwndPageTree)
	{
		ASSERT(FALSE);
		AfxThrowMemoryException();
	}
	
	// MFC7-support here (Thanks to Rainer Wollgarten)
  // YT: Cast tree control to CWnd and calls CWnd::CreateEx in all cases (VC 6 and7).
  ((CWnd*)m_pwndPageTree)->CreateEx(
    WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY, 
    _T("SysTreeView32"), _T("PageTree"), 
    WS_TABSTOP|WS_CHILD|WS_VISIBLE|dwTreeStyle, 
    rectTree, this, s_unPageTreeId);
	
	if (m_bTreeImages)
	{
		m_pwndPageTree->SetImageList(&m_Images, TVSIL_NORMAL);
		m_pwndPageTree->SetImageList(&m_Images, TVSIL_STATE);
	}

  // TreePropSheetEx: Fix refresh problem.
	// Fill the tree ctrl
  {
    TreePropSheet::CWindowRedrawScope WindowRedrawScope( m_pwndPageTree, true );
    // Populate the tree control.
    RefillPageTree();
    // Expand the tree if necessary.
    if( IsAutoExpandTree() )
    {
      ExpandTreeItem( m_pwndPageTree, m_pwndPageTree->GetRootItem(), TVE_EXPAND );
    }
    // Select item for the current page
	  if (pTab->GetCurSel() > -1)
		  SelectPageTreeItem(pTab->GetCurSel());
  }
	return bResult;
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::OnDestroy() 
{
	CPropertySheet::OnDestroy();
	
	// Fix for TN017
  if( m_pwndPageTree && m_pwndPageTree->m_hWnd )
    m_pwndPageTree->DestroyWindow();
  if( m_pFrame && m_pFrame->GetWnd()->m_hWnd )
    m_pFrame->GetWnd()->DestroyWindow();	

  if (m_Images.GetSafeHandle())
		m_Images.DeleteImageList();

	delete m_pwndPageTree;
	m_pwndPageTree = NULL;

	delete m_pFrame;
	m_pFrame = NULL;
}

/////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetBase::OnAddPage(WPARAM wParam, LPARAM lParam)
{
	LRESULT	lResult = DefWindowProc(PSM_ADDPAGE, wParam, lParam);
	if (!m_bTreeViewMode)
		return lResult;

  // TreePropSheetEx: Fix refresh problem.
  {
    TreePropSheet::CWindowRedrawScope WindowRedrawScope( m_pwndPageTree, true );
    RefillPageTree();
	  SelectCurrentPageTreeItem();
  }

	return lResult;
}

/////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetBase::OnRemovePage(WPARAM wParam, LPARAM lParam)
{
	LRESULT	lResult = DefWindowProc(PSM_REMOVEPAGE, wParam, lParam);
	if (!m_bTreeViewMode)
		return lResult;

  // TreePropSheetEx: Fix refresh problem.
  {
    TreePropSheet::CWindowRedrawScope WindowRedrawScope( m_pwndPageTree, true );
    RefillPageTree();
	  SelectCurrentPageTreeItem();
  }

	return lResult;
}

/////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetBase::OnSetCurSel(WPARAM wParam, LPARAM lParam)
{
	LRESULT	lResult = DefWindowProc(PSM_SETCURSEL, wParam, lParam);
	if (!m_bTreeViewMode)
		return lResult;

	SelectCurrentPageTreeItem();
	UpdateCaption();
	return lResult;
}

/////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetBase::OnSetCurSelId(WPARAM wParam, LPARAM lParam)
{
	LRESULT	lResult = DefWindowProc(PSM_SETCURSEL, wParam, lParam);
	if (!m_bTreeViewMode)
		return lResult;

	SelectCurrentPageTreeItem();
	UpdateCaption();
	return lResult;
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::OnPageTreeSelChanging(NMHDR *pNotifyStruct, LRESULT *plResult)
{
  if( m_nRefillingPageTreeContent ) 
    return;

	*plResult = 0;

	NMTREEVIEW	*pTvn = reinterpret_cast<NMTREEVIEW*>(pNotifyStruct);
	int					nPage = m_pwndPageTree->GetItemData(pTvn->itemNew.hItem);
	BOOL				bResult;
  // Is the data item pointing to a valid page?
	if (nPage<0 || (unsigned)nPage>=m_pwndPageTree->GetCount())
	{
    // No, see if we are skipping empty page.
    if( IsTreeItemDisplayable( pTvn->itemNew.hItem ) ) 
    {
      // Skipping empty page, get the next valid page.
      HTREEITEM hNewItem = GetNextValidPageTreeItem( pTvn->itemNew.hItem );
      if( hNewItem )
        m_pwndPageTree->SelectItem( hNewItem );
      bResult = false;
    }
    else
    {
      // Not skipping empty pages, kill the current page so that we can display 
      // the generic message.
      bResult = KillActiveCurrentPage();
    }
  }
	else
  {
    if( IsTreeItemDisplayable( pTvn->itemNew.hItem ) )
    {
      // OK, set the new active page.
      TreePropSheet::CIncrementScope RefillingPageTreeContentGuard( m_nRefillingPageTreeContent );
      bResult = SetActivePage(nPage);
    }
    else
    {
      HTREEITEM hNewItem = GetNextValidPageTreeItem( pTvn->itemNew.hItem );
      if( hNewItem )
        m_pwndPageTree->SelectItem( hNewItem );
      bResult = false;
    }
  }

	if (!bResult)
		// prevent selection to change
		*plResult = TRUE;

	// Set focus to tree control (I guess that's what the user expects)
	m_pwndPageTree->SetFocus();

	return;
}

/////////////////////////////////////////////////////////////////////
//
void CTreePropSheetBase::OnPageTreeSelChanged(NMHDR *pNotifyStruct, LRESULT *plResult)
{
  UNREFERENCED_PARAMETER(pNotifyStruct);

  // Refilling the tree control, ignore selection change.
  if( m_nRefillingPageTreeContent ) 
    return;

  *plResult = 0;

	UpdateCaption();

	return;
}

/////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetBase::OnEnablePage(WPARAM wParam, LPARAM lParam)
{
  // Convert parameters
  CPropertyPage* pPage = reinterpret_cast<CPropertyPage*>( wParam );
  bool bEnable = lParam?true:false;

  // Make sure that this page is inside this sheet.

  // Enable/Disable the page.
  EnablePage( pPage, bEnable );
  return 1L;
}

/////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetBase::OnIsDialogMessage(WPARAM wParam, LPARAM lParam)
{
  MSG *pMsg = reinterpret_cast<MSG*>(lParam);
  ASSERT( pMsg );

  if (pMsg->message==WM_KEYDOWN && GetKeyState(VK_CONTROL)&0x8000)
  {
    if( pMsg->wParam==VK_TAB )
    {
      if (GetKeyState(VK_SHIFT)&0x8000)
      {
        ActivatePreviousPage(); 
      }
      else    
      {
        ActivateNextPage(); 
      }
    }
    else
    {
      if( pMsg->wParam==VK_PRIOR )      /*PageUp*/
      {
        ActivatePreviousPage();
      }
      else
      {
        if( pMsg->wParam==VK_NEXT )     /*PageDown*/
        {
          ActivateNextPage();
        }
      }
    }
    return TRUE; 
  }

	return CPropertySheet::DefWindowProc(PSM_ISDIALOGMESSAGE, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetBase::PreTranslateMessage(MSG* pMsg) 
{
  if( pMsg->hwnd == GetPageTreeControl()->GetSafeHwnd() )
  {
    // If not in skipping empty page mode, keep the standard behavior.
    if( !m_bSkipEmptyPages )
      return CPropertySheet::PreTranslateMessage(pMsg);

    if( pMsg->message == WM_KEYDOWN )
    {
      // Get the current tree item.
      HTREEITEM hCurrentItem = m_pwndPageTree->GetSelectedItem();
      if( NULL == hCurrentItem )
        return TRUE;

      if( pMsg->wParam == VK_UP )
      {
        // Active the previous page according to tree ordering - 
        // skipping empty pages on the way.
        ActivatePreviousPage( hCurrentItem );
   	    return TRUE;
      }
      else if ( pMsg->wParam == VK_DOWN )
      {
        // Active the next page according to tree ordering -
        // skipping empty pages on the way.
        ActivateNextPage( hCurrentItem );
  	    return TRUE;
      }
      else if( pMsg->wParam == VK_LEFT )
      {
        /* Here, we try to mimic the tree keyboard handling by doing 
           one of the two things:the following
           - If the tree item is expanded, collapse it
           - If the tree item is collapse, find a parent item that has a page
             associated with it and select it, collapsing all items along the 
             way. */
        if( IsItemExpanded( hCurrentItem ) )
        {
          // Collapse the item since it is expanded.
          m_pwndPageTree->Expand( hCurrentItem, TVE_COLLAPSE );
        }
        else
        {
          // Already collapsed, search for a candidate for selection.
          HTREEITEM hItem = m_pwndPageTree->GetParentItem( hCurrentItem );
          while( NULL != hItem && !HasValidPropertyPageAssociated( hItem ) )
          {
            // Add item to the stack.
            hItem = m_pwndPageTree->GetParentItem( hItem );
          }
          // If the item points to a valid page, select it and collapse 
          if( NULL != hItem && HasValidPropertyPageAssociated( hItem ) )
          {
            m_pwndPageTree->SelectItem( hItem );
          }
        }
        return TRUE;
      }
      else if( pMsg->wParam == VK_RIGHT )
      {
        /* Here, we try to mimic the tree keyboard handling by doing 
           one of the two things:the following
           - If the tree item is collapsed, expand it
           - If the tree item is expanded, find a child item that has a page
             associated with it and select it, expanding all items along the 
             way. The child has to be a first child in the hierarchy. */
        if( IsItemExpanded( hCurrentItem ) )
        {
          // Already expanded, search for a candidate for selection.
          HTREEITEM hItem = m_pwndPageTree->GetChildItem( hCurrentItem );
          while( NULL != hItem && !HasValidPropertyPageAssociated( hItem ) )
          {
            // Add item to the stack.
            hItem = m_pwndPageTree->GetChildItem( hItem );
          }
          // If the item points to a valid page, select it and collapse 
          if( NULL != hItem && HasValidPropertyPageAssociated( hItem ) )
          {
            m_pwndPageTree->SelectItem( hItem );
          }
        }
        else
        {
          // Expand the item since it is collapsed.
          m_pwndPageTree->Expand( hCurrentItem, TVE_EXPAND );
        }
        return TRUE;
      }
    }
  }
  else if( WM_ENABLE == pMsg->message)
  {
    // Handle WM_ENABLE messages for property pages: Update the property page
    // information map.
    TRACE("");

  }

	return CPropertySheet::PreTranslateMessage(pMsg);
}

} //namespace TreePropSheet

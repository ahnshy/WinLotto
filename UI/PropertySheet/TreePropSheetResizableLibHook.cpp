// TreePropSheetResizableLibHook.cpp: implementation of the CTreePropSheetResizableLibHook class.
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

#include "stdafx.h"
#include "TreePropSheetResizableLibHook.h"
#include "TreePropSheetEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TreePropSheet
{

//********************************************************************
// CResizableSheetHook
//********************************************************************

// maps an index to a button ID and vice-versa
static UINT _propButtons[] =
{
	IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP,
	ID_WIZBACK, ID_WIZNEXT, ID_WIZFINISH
};
const int _propButtonsCount = sizeof(_propButtons)/sizeof(UINT);

// horizontal line in wizard mode
#define ID_WIZLINE	ID_WIZFINISH+1

// used to save/restore active page
// either in the registry or a private .INI file
// depending on your application settings

#define ACTIVEPAGE 	_T("ActivePage")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResizableSheetHook::CResizableSheetHook(CPropertySheet* const pSheet)
 : m_pSheet(pSheet)
{
  ASSERT( m_pSheet );

	PrivateConstruct();

}

CResizableSheetHook::~CResizableSheetHook()
{
}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////

void CResizableSheetHook::Initialize()
{
	ASSERT( m_pSheet->GetSafeHwnd() );

  Hook( m_pSheet );

  // Perform remaining initialization.

  // From OnCreate
	// Create and initialize the size-grip. We only do this for child windows.
	if( m_pSheet->GetStyle() & WS_POPUP )
  {
    CreateSizeGrip();
  }
  
  // From OnInitDialog
  // set the initial size as the min track size
	CRect rc;
	m_pSheet->GetWindowRect(&rc);
	SetMinTrackSize(rc.Size());

	// initialize layout
	PresetLayout();

	// prevent flickering
	m_pSheet->GetTabControl()->ModifyStyle(0, WS_CLIPSIBLINGS);
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CResizableSheetHook::SetMinSize(const CSize& size)
{
	SetMinTrackSize( size );
}

//////////////////////////////////////////////////////////////////////
// 
void CResizableSheetHook::SetMaxSize(const CSize& size)
{
	SetMaxTrackSize( size );
}

//////////////////////////////////////////////////////////////////////
// CHookWnd overrides
//////////////////////////////////////////////////////////////////////

LRESULT CResizableSheetHook::WindowProc(UINT nMsg,WPARAM wParam,LPARAM lParam)
{
  if( WM_SIZE == nMsg )
  {
    LRESULT lResult = Default();

    UINT nType = (UINT)wParam;

	  if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		  return lResult;		// arrangement not needed

	  if (nType == SIZE_MAXIMIZED)
		  HideSizeGrip(&m_dwGripTempState);
	  else
		  ShowSizeGrip(&m_dwGripTempState);

	  // update grip and layout
	  if( IsSizeGripVisible() )
      UpdateSizeGrip();
	  ArrangeLayout();

    return lResult;
  }
  else if( WM_ERASEBKGND == nMsg )
  {
    // Windows XP doesn't like clipping regions ...try this!
    EraseBackground( (HDC)wParam );
	  return 0L;
  }
  else if( WM_GETMINMAXINFO == nMsg )
  {
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
    MinMaxInfo(lpMMI);
  }
  else if( PSN_SETACTIVE == nMsg )
  {
    ASSERT( FALSE );
  }
  else if( WM_CREATE == nMsg )
  {
    return Default();
  }
  else if( WM_DESTROY == nMsg )
  {
    if (m_bEnableSaveRestore)
    {
		  SaveWindowRect(m_sSection, m_bRectOnly);
		  SavePage();
	  }
	  RemoveAllAnchors();
  }
 

  return Default();
}

BOOL CResizableSheetHook::ArrangeLayoutCallback(LayoutInfo &layout)
{
	if (layout.nCallbackID != 1)	// we only added 1 callback
		return CResizableLayout::ArrangeLayoutCallback(layout);

	// set layout info for active page
	layout.hWnd = (HWND)::SendMessage( m_pSheet->GetSafeHwnd(), PSM_GETCURRENTPAGEHWND, 0, 0);
	if (!::IsWindow(layout.hWnd))
		return FALSE;

	// set margins
	if (IsWizard())	// wizard mode
	{
		// use pre-calculated margins
		layout.sizeMarginTL = m_sizePageTL;
		layout.sizeMarginBR = m_sizePageBR;
	}
	else	// tab mode
	{
		CTabCtrl* pTab = m_pSheet->GetTabControl();
		ASSERT(pTab != NULL);

		// get tab position after resizing and calc page rect
		CRect rectPage, rectSheet;
		GetTotalClientRect(&rectSheet);

		VERIFY(GetAnchorPosition(pTab->m_hWnd, rectSheet, rectPage));
		pTab->AdjustRect(FALSE, &rectPage);

		// set margins
		layout.sizeMarginTL = rectPage.TopLeft() - rectSheet.TopLeft();
		layout.sizeMarginBR = rectPage.BottomRight() - rectSheet.BottomRight();
	}

	// set anchor types
	layout.sizeTypeTL = TOP_LEFT;
	layout.sizeTypeBR = BOTTOM_RIGHT;

	// use this layout info
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CResizableLayout implementation
//////////////////////////////////////////////////////////////////////

CWnd* CResizableSheetHook::GetResizableWnd()
{
	return m_pSheet;
}

//////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////

void CResizableSheetHook::PresetLayout()
{
  if (IsWizard())	// wizard mode
	{
		// hide tab control
		m_pSheet->GetTabControl()->ShowWindow(SW_HIDE);

		AddAnchor(ID_WIZLINE, BOTTOM_LEFT, BOTTOM_RIGHT);
	}
	else	// tab mode
	{
		AddAnchor(AFX_IDC_TAB_CONTROL, TOP_LEFT, BOTTOM_RIGHT);
	}

	// add a callback for active page (which can change at run-time)
	AddAnchorCallback(1);

	// use *total* parent size to have correct margins
	CRect rectPage, rectSheet;
	GetTotalClientRect(&rectSheet);

	m_pSheet->GetActivePage()->GetWindowRect(&rectPage);
	::MapWindowPoints(NULL, m_pSheet->GetSafeHwnd(), (LPPOINT)&rectPage, 2);

	// pre-calculate margins
	m_sizePageTL = rectPage.TopLeft() - rectSheet.TopLeft();
	m_sizePageBR = rectPage.BottomRight() - rectSheet.BottomRight();

	// add all possible buttons, if they exist
	for (int i = 0; i < _propButtonsCount; i++)
	{
		if (NULL != m_pSheet->GetDlgItem(_propButtons[i]))
			AddAnchor(_propButtons[i], BOTTOM_RIGHT);
	}
}

inline void CResizableSheetHook::PrivateConstruct()
{
	m_bEnableSaveRestore = FALSE;
	m_bSavePage = FALSE;
	m_dwGripTempState = 1;
}

void CResizableSheetHook::SavePage()
{
	if (!m_bSavePage)
		return;

	// saves active page index, zero (the first) if problems
	// cannot use GetActivePage, because it always fails

	CTabCtrl *pTab = m_pSheet->GetTabControl();
	int page = 0;

	if (pTab != NULL) 
		page = pTab->GetCurSel();
	if (page < 0)
		page = 0;

	AfxGetApp()->WriteProfileInt(m_sSection, ACTIVEPAGE, page);
}

void CResizableSheetHook::LoadPage()
{
	// restore active page, zero (the first) if not found
	int page = AfxGetApp()->GetProfileInt(m_sSection, ACTIVEPAGE, 0);
	
	if (m_bSavePage)
	{
		m_pSheet->SetActivePage(page);
		ArrangeLayout();	// needs refresh
	}
}

BOOL CResizableSheetHook::IsWizard()
{
  return (m_pSheet->m_psh.dwFlags & PSH_WIZARD); 
}

void CResizableSheetHook::EnableSaveRestore(LPCTSTR pszSection,BOOL bRectOnly /*=FALSE*/,BOOL bWithPage /*=FALSE*/)
{
	m_sSection = pszSection;
	m_bSavePage = bWithPage;

	m_bEnableSaveRestore = TRUE;
	m_bRectOnly = bRectOnly;

	// restore immediately
	LoadWindowRect(pszSection, bRectOnly);
	LoadPage();
}

int CResizableSheetHook::GetMinWidth()
{
	CWnd* pWnd = NULL;
	CRect rectWnd, rectSheet;
	GetTotalClientRect(&rectSheet);

	int max = 0, min = rectSheet.Width();
	// search for leftmost and rightmost button margins
	for (int i = 0; i < 7; i++)
	{
		pWnd = m_pSheet->GetDlgItem(_propButtons[i]);
		// exclude not present or hidden buttons
		if (pWnd == NULL || !(pWnd->GetStyle() & WS_VISIBLE))
			continue;

		// left position is relative to the right border
		// of the parent window (negative value)
		pWnd->GetWindowRect(&rectWnd);
		::MapWindowPoints(NULL, m_pSheet->GetSafeHwnd(), (LPPOINT)&rectWnd, 2);
		int left = rectSheet.right - rectWnd.left;
		int right = rectSheet.right - rectWnd.right;

		if (left > max)
			max = left;
		if (right < min)
			min = right;
	}

	// sizing border width
	int border = GetSystemMetrics(SM_CXSIZEFRAME);
	
	// compute total width
	return max + min + 2*border;
}

//********************************************************************
// CTreePropSheetResizableLibHook
//********************************************************************

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreePropSheetResizableLibHook::CTreePropSheetResizableLibHook(TreePropSheet::CTreePropSheetEx* pTreePropSheet)
 : CResizableSheetHook(pTreePropSheet),
   m_pTreePropSheet(pTreePropSheet)
{
}

CTreePropSheetResizableLibHook::~CTreePropSheetResizableLibHook()
{
}

//////////////////////////////////////////////////////////////////////
// Overridable
//////////////////////////////////////////////////////////////////////

void CTreePropSheetResizableLibHook::PresetLayout()
{
	/* If in tree mode, register the tree and the frame with the layout manager. */
  if( m_pTreePropSheet->IsTreeViewMode() )      // Tree mode
  {
  	m_pTreePropSheet->GetPageTreeControl()->ModifyStyle(0, WS_CLIPSIBLINGS);
	  m_pTreePropSheet->GetFrameControl()->GetWnd()->ModifyStyle(0, WS_CLIPSIBLINGS);
    AddAnchor( m_pTreePropSheet->GetSplitterWnd()->GetSafeHwnd(), TOP_LEFT, BOTTOM_RIGHT );
    // Save the margin between the frame and the current page. This rect should NOT be normalized.
    CRect rectFrame, rectPage;
    ::GetWindowRect( m_pTreePropSheet->GetFrameControl()->GetWnd()->GetSafeHwnd(), &rectFrame );
    ::GetWindowRect( m_pTreePropSheet->GetActivePage()->GetSafeHwnd(), &rectPage );
    m_rectFramePageMargins.SetRect( rectPage.left - rectFrame.left,
                                    rectPage.top - rectFrame.top,
                                    rectFrame.right - rectPage.right,
                                    rectFrame.bottom - rectPage.bottom );
  	AddAnchorCallback(1);   // Callback for frame
	  AddAnchorCallback(2);   // Callback for page
  }
  else
  {
    if (IsWizard())	                        // wizard mode
	  {
		  // hide tab control
		  m_pSheet->GetTabControl()->ShowWindow(SW_HIDE);

		  AddAnchor(ID_WIZLINE, BOTTOM_LEFT, BOTTOM_RIGHT);
	  }
	  else	                                        // tab mode
	  {
		  AddAnchor(AFX_IDC_TAB_CONTROL, TOP_LEFT, BOTTOM_RIGHT);
	  }

	  // add a callback for active page (which can change at run-time)
	  AddAnchorCallback(1);
  }

	// use *total* parent size to have correct margins
	CRect rectPage, rectSheet;
	GetTotalClientRect(&rectSheet);

	m_pSheet->GetActivePage()->GetWindowRect(&rectPage);
	::MapWindowPoints(NULL, m_pSheet->GetSafeHwnd(), (LPPOINT)&rectPage, 2);

	// pre-calculate margins
	m_sizePageTL = rectPage.TopLeft() - rectSheet.TopLeft();
	m_sizePageBR = rectPage.BottomRight() - rectSheet.BottomRight();

	// add all possible buttons, if they exist
  for (int i = 0; i < _propButtonsCount; i++)
	{
    if (NULL != m_pSheet->GetDlgItem(_propButtons[i]))
      AddAnchor(_propButtons[i], BOTTOM_RIGHT);
	}
}

//////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetResizableLibHook::ArrangeLayoutCallback(LayoutInfo &layout)
{
	// Handle the tree callbacks in another methods.
  if( m_pTreePropSheet->IsTreeViewMode() )    // Tree mode
  {
    return TreeModeCallbacks( layout );
  }

	if (layout.nCallbackID != 1)	// we only added 1 callback
		return CResizableSheetHook::ArrangeLayoutCallback(layout);

	// set layout info for active page
	layout.hWnd = (HWND)::SendMessage( m_pSheet->GetSafeHwnd(), PSM_GETCURRENTPAGEHWND, 0, 0);
	if (!::IsWindow(layout.hWnd))
		return FALSE;

	if( m_pTreePropSheet->IsWizard() )	   // wizard mode
	{
		// use pre-calculated margins
		layout.sizeMarginTL = m_sizePageTL;
		layout.sizeMarginBR = m_sizePageBR;
	}
	else	                                 // tab mode
	{
		CTabCtrl* pTab = m_pSheet->GetTabControl();
		ASSERT(pTab != NULL);

		// get tab position after resizing and calc page rect
		CRect rectPage, rectSheet;
		GetTotalClientRect(&rectSheet);

		VERIFY(GetAnchorPosition(pTab->m_hWnd, rectSheet, rectPage));
		pTab->AdjustRect(FALSE, &rectPage);

		// set margins
		layout.sizeMarginTL = rectPage.TopLeft() - rectSheet.TopLeft();
		layout.sizeMarginBR = rectPage.BottomRight() - rectSheet.BottomRight();
	}

	// set anchor types
	layout.sizeTypeTL = TOP_LEFT;
	layout.sizeTypeBR = BOTTOM_RIGHT;

	// use this layout info
	return TRUE;
}

BOOL CTreePropSheetResizableLibHook::TreeModeCallbacks(LayoutInfo &layout)
{
	ASSERT( m_pTreePropSheet->IsTreeViewMode() );

  if (layout.nCallbackID != 1 && layout.nCallbackID != 2)	// we only added 1 callback
		return CResizableSheetHook::ArrangeLayoutCallback(layout);

  /* Callbacks:
      1: Tab
      2: Page */

  CWnd* pFrame = m_pTreePropSheet->GetFrameControl()->GetWnd();
  ASSERT( pFrame );
  CRect rectSheet, rectFrame;
  m_pTreePropSheet->GetClientRect( &rectSheet );
	pFrame->GetWindowRect(&rectFrame);
  m_pTreePropSheet->ScreenToClient(&rectFrame);

  if( layout.nCallbackID == 1 )
  { // Tab control
    // Need to set the tab and page according to the frame. Since this 
    // is resized by 'anchors', it is guaranteed to be already resized 
    // properly.
    layout.hWnd = m_pTreePropSheet->GetTabControl()->GetSafeHwnd();
    
	  layout.sizeTypeTL = TOP_LEFT;
	  layout.sizeTypeBR = BOTTOM_RIGHT;

		// set margins
		layout.sizeMarginTL = rectFrame.TopLeft() - rectSheet.TopLeft();
		layout.sizeMarginBR = rectFrame.BottomRight() - rectSheet.BottomRight();

    ::MoveWindow( layout.hWnd, rectFrame.left, rectFrame.top, rectFrame.Width(), rectFrame.Height(), FALSE );
  }
  else
  { // Page
	  // set layout info for active page
	  layout.hWnd = (HWND)::SendMessage( m_pSheet->GetSafeHwnd(), PSM_GETCURRENTPAGEHWND, 0, 0);
	  if (!::IsWindow(layout.hWnd))
		  return FALSE;

	  layout.sizeTypeTL = TOP_LEFT;
	  layout.sizeTypeBR = BOTTOM_RIGHT;
    rectFrame.DeflateRect( m_rectFramePageMargins );

		// set margins
		layout.sizeMarginTL = rectFrame.TopLeft() - rectSheet.TopLeft();
		layout.sizeMarginBR = rectFrame.BottomRight() - rectSheet.BottomRight();

    ::MoveWindow( layout.hWnd, rectFrame.left, rectFrame.top, rectFrame.Width(), rectFrame.Height(), FALSE );
  }
  return TRUE;
}

};
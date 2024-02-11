// TreePropSheetEx.cpp
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
#include "TreePropSheetEx.h"
#include "TreePropSheetSplitter.h"
#include "../PropertyFrame/MemDC.h"
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
  /*! Initialized size. */
  const CSize kInitializeSize( -1, -1 );
};
namespace TreePropSheet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CTreePropSheetEx, CTreePropSheetBase )

#pragma warning(push)
#pragma warning(4: 4355)

CTreePropSheetEx::CTreePropSheetEx()
 : CTreePropSheetBase()
{
  Init();
}

CTreePropSheetEx::CTreePropSheetEx(UINT nIDCaption,CWnd* pParentWnd /*=NULL*/,UINT iSelectPage /*=0*/)
 : CTreePropSheetBase( nIDCaption, pParentWnd, iSelectPage )
{
  Init();
}

CTreePropSheetEx::CTreePropSheetEx(LPCTSTR pszCaption,CWnd* pParentWnd /*=NULL*/,UINT iSelectPage /*=0*/)
 : CTreePropSheetBase( pszCaption, pParentWnd, iSelectPage )
{
  Init();
}

#pragma warning(pop)

CTreePropSheetEx::~CTreePropSheetEx()
{
}

//////////////////////////////////////////////////////////////////////
// Properties
//////////////////////////////////////////////////////////////////////

bool CTreePropSheetEx::SetIsResizable(const bool bIsResizable)
{
  if( ::IsWindow( m_hWnd ) )
    return false;

	m_bIsResizable = bIsResizable;
  return true;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::IsResizable() const
{
	return m_bIsResizable;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::SetMinSize(const CSize& sizeMin)
{
  ASSERT( m_bIsResizable );
	m_sizeSheetMinimumSize = sizeMin;

  // If the window exists, update it.
  UpdateSheetMinimumSize();
}

//////////////////////////////////////////////////////////////////////
//
CSize CTreePropSheetEx::GetMinSize() const
{
	return m_sizeSheetMinimumSize;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::SetMaxSize(const CSize& sizeMax)
{
  ASSERT( m_bIsResizable );
	m_sizeSheetMaximumSize = sizeMax;

  // If the window exists, update it.
  UpdateSheetMaximumSize();
}

//////////////////////////////////////////////////////////////////////
//
CSize CTreePropSheetEx::GetMaxSize() const
{
	return m_sizeSheetMaximumSize;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::SetPaneMinimumSize(const int nPaneMinimumSize)
{
  ASSERT( nPaneMinimumSize >= 0 );

  if( ::IsWindow( m_hWnd ) )
    return false;

  m_nTreeMinimumSize = nPaneMinimumSize;
  m_nFrameMinimumSize = nPaneMinimumSize;

  return true;
}

//////////////////////////////////////////////////////////////////////
//
int CTreePropSheetEx::GetPaneMinimumSize() const
{
  if( m_nTreeMinimumSize != m_nFrameMinimumSize )
    return -1;

	return m_nFrameMinimumSize;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::SetPaneMinimumSizes(const int nTreeMinimumSize,const int nFrameMinimumSize)
{
  ASSERT( nTreeMinimumSize >= 0 );
  ASSERT( nFrameMinimumSize >= 0 );

  if( ::IsWindow( m_hWnd ) )
    return false;

  m_nTreeMinimumSize = nTreeMinimumSize;
  m_nFrameMinimumSize = nFrameMinimumSize;
  return true;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::GetPaneMinimumSizes(int& nTreeMinimumSize,int& nFrameMinimumSize) const
{
  nTreeMinimumSize = m_nTreeMinimumSize;
  nFrameMinimumSize = m_nFrameMinimumSize;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::SetTreeIsResizable(const bool bIsTreeResizable)
{
	m_bIsTreeResizable = bIsTreeResizable;

  // If the splitter exists, update it.
  if( ::IsWindow( m_hWnd ) && NULL != m_pSplitter.get() )
  {
    m_pSplitter->SetAllowUserResizing( m_bIsTreeResizable );
  }
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::IsTreeResizable() const
{
	return m_bIsTreeResizable;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::SetSplitterWidth(const int nSplitterWidth)
{
  ASSERT( m_nSplitterWidth >= 1 );
	if( ::IsWindow( m_hWnd ) ) 
    return false;

  m_nSplitterWidth = nSplitterWidth;
  return true;
}

//////////////////////////////////////////////////////////////////////
//
int CTreePropSheetEx::GetSplitterWidth() const
{
	return m_nSplitterWidth;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::SetTreeResizingMode(const enTreeSizingMode eSizingMode)
{
  ASSERT( eSizingMode == TSM_Fixed || eSizingMode == TSM_Proportional );

	// Store the tree sizing mode.
  m_eSizingMode = eSizingMode;
  
  // Update the splitter if it exists.
  if( m_pSplitter.get() )
  {
    const bool vert_splitter_frozen[2] = { m_eSizingMode==TSM_Fixed?true:false, false };
    m_pSplitter->SetFrozenPanes( vert_splitter_frozen );
  }
}

//////////////////////////////////////////////////////////////////////
//
enTreeSizingMode CTreePropSheetEx::GetTreeResizingMode() const
{
	return m_eSizingMode;
}

//////////////////////////////////////////////////////////////////////
//
CTreePropSheetEx::tSplitter* CTreePropSheetEx::GetSplitterObject() const
{
	return m_pSplitter.get();
}

//////////////////////////////////////////////////////////////////////
//
CWnd* CTreePropSheetEx::GetSplitterWnd() const
{
	return static_cast<CWnd*>( m_pSplitter.get() );
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::SetContextMenuMode(const TreePropSheet::enContextMenuMode eContextMenuMode)
{
  m_eContextMenuMode = eContextMenuMode;
}

//////////////////////////////////////////////////////////////////////
//
TreePropSheet::enContextMenuMode CTreePropSheetEx::GetContextMenuMode() const
{
	return m_eContextMenuMode; 
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::SetRealTimeSplitter(const bool bRealtime)
{
	m_bIsRealtimeSplitterMode = bRealtime;

  // Update the splitter if it exists.
  if( m_pSplitter.get() )
  {
    m_pSplitter->SetRealtimeUpdate( m_bIsRealtimeSplitterMode );
  }
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::IsRealTimeSplitter() const
{
	return m_bIsRealtimeSplitterMode;
}

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

BOOL CTreePropSheetEx::SetTreeWidth(int nWidth)
{
  // If the sheet is not resizable and already dispalyed, we cannot set the width. 
  if( !IsSettingWidthValid() )
  {
    ASSERT( FALSE );
    return false;
  }

  if( nWidth < 0 )
  {
    // ASSERT: Tree width cannot be negative. Maximum range is not checked. 
    ASSERT( FALSE );
    return FALSE;
  }

  m_nPageTreeWidth = nWidth;

  return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Overridables
//////////////////////////////////////////////////////////////////////

CTreePropSheetEx::tSplitterPtr CTreePropSheetEx::CreateSplitterObject() const
{
  const int zMinSizes[2] = { m_nTreeMinimumSize, m_nFrameMinimumSize };
  return tSplitterPtr(new tSplitter( 2, SSP_HORZ, zMinSizes, m_nSplitterWidth ) );
}

//////////////////////////////////////////////////////////////////////
//
CTreePropSheetEx::tLayoutManagerPtr CTreePropSheetEx::CreateLayoutManagerObject()
{
  ASSERT( m_bIsResizable );
  return tLayoutManagerPtr( new tLayoutManager( this ) );
}

//////////////////////////////////////////////////////////////////////
//
CTreePropSheetEx::tLayoutManager* CTreePropSheetEx::GetLayoutManagerObject() const
{
  return m_pResizeHook.get();
}

//////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////

void CTreePropSheetEx::Init()
{
  m_psh.dwFlags &= ~( PSH_HASHELP );

  m_bIsResizable = true;
  m_bIsTreeResizable = true;
  m_bIsRealtimeSplitterMode = true;
  m_bSkipEmptyPages = false;
  m_eSizingMode = TSM_Fixed;
  m_eContextMenuMode = TPS_PropertySheetControls;
  m_sizeSheetMinimumSize = kInitializeSize;
  m_sizeSheetMaximumSize = kInitializeSize;
  m_nSplitterWidth = 5;
  m_nTreeMinimumSize = 148;
  m_nFrameMinimumSize = 148;
}

//////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetEx::IsSettingWidthValid() const
{
	return !IsWindow( m_hWnd ) || IsResizable();
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::UpdateSheetMinimumSize()
{
  // If the window exists, update it.
  if( ::IsWindow( m_hWnd ) &&
      IsResizable() &&
      kInitializeSize != m_sizeSheetMinimumSize &&
      NULL != m_pResizeHook.get() )
  {
    // Just make sure that the provided minimum width is
    // wide enough according to minimum pane width. 
    ASSERT( m_sizeSheetMinimumSize.cx >= m_nTreeMinimumSize + m_nFrameMinimumSize + m_nSplitterWidth );
    m_pResizeHook->SetMinSize( CSize( __max( m_sizeSheetMinimumSize.cx, m_nTreeMinimumSize + m_nFrameMinimumSize + m_nSplitterWidth ),
                                      m_sizeSheetMinimumSize.cy ) );
  }
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::UpdateSheetMaximumSize()
{
  // If the window exists, update it.
  if( ::IsWindow( m_hWnd ) &&
      IsResizable() &&
      kInitializeSize != m_sizeSheetMaximumSize &&
      NULL != m_pResizeHook.get() )
  {
    // Just make sure that the provided maximum size is
    // larger than the minimum size.
    ASSERT( m_sizeSheetMinimumSize.cx <= m_sizeSheetMaximumSize.cx );
    ASSERT( m_sizeSheetMinimumSize.cy <= m_sizeSheetMaximumSize.cy );
    m_pResizeHook->SetMaxSize( CSize( __max( m_sizeSheetMinimumSize.cx, m_sizeSheetMaximumSize.cx ),
                                      __max( m_sizeSheetMinimumSize.cy, m_sizeSheetMaximumSize.cy ) ) );
  }
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTreePropSheetEx, CTreePropSheetBase)
	//{{AFX_MSG_MAP(CTreePropSheetEx)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreePropSheetEx message handlers
//////////////////////////////////////////////////////////////////////

int CTreePropSheetEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreePropSheetBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  // If the sheet is resizable, update the sheet window style to allow resizing.
	if( IsResizable() )
  {
    // Prepare the property sheet to be resizable.
	  // keep client area
	  CRect rect;
	  GetClientRect(&rect);
    // Do not change the window style if it is a child window.
    if( lpCreateStruct->style & WS_POPUP )
    {
      // set resizable style
	    ModifyStyle(DS_MODALFRAME, WS_POPUP | WS_THICKFRAME );
    }
	  // adjust size to reflect new style
	  ::AdjustWindowRectEx(&rect, GetStyle(),::IsMenu(GetMenu()->GetSafeHmenu()), GetExStyle());
	  SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
                 SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREPOSITION);
  }

	return 0;
}

//////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetEx::OnInitDialog() 
{
	CTreePropSheetBase::OnInitDialog();
	
  // Add the panes to the splitter.
  if( IsTreeViewMode() )
  {
    if( IsResizable() )
    {
      // Create the splitter object.
      m_pSplitter = CreateSplitterObject();
      ASSERT( m_pSplitter.get() );

      // Compute the proper position for the splitter control, that is union of tree and frame.
      CRect rectTree, rectTab, rectSplit;
      GetPageTreeControl()->GetWindowRect( &rectTree );
      GetFrameControl()->GetWnd()->GetWindowRect( &rectTab );
      rectSplit.UnionRect( &rectTree, &rectTab );
      ScreenToClient( &rectSplit );

      // Create the splitter window. 
      m_pSplitter->Create( this, rectSplit );

      // Set the pane
      m_pSplitter->SetPane( 0, GetPageTreeControl() );                // Pane 0 is the tree control
      m_pSplitter->SetPane( 1, GetFrameControl()->GetWnd() );         // Pane 1 is the frame
	    const int vert_splitter_sizes[2] = { rectTree.Width(), rectTab.Width() };
      m_pSplitter->SetPaneSizes( vert_splitter_sizes );
	    // Set frozen panes.
      SetTreeResizingMode( m_eSizingMode );
      // Set realtime mode.
      SetRealTimeSplitter( m_bIsRealtimeSplitterMode );
      // Enable/disable resizing in splitter.
      m_pSplitter->SetAllowUserResizing( m_bIsTreeResizable );
    }
  }

	// Initialize the layout library.
  if( IsResizable() )
  {
    m_pResizeHook = CreateLayoutManagerObject();
    ASSERT( m_pResizeHook.get() );
    m_pResizeHook->Initialize();
    
    // Set the minimum size.
    UpdateSheetMinimumSize();

    // Set the maximum size.
    UpdateSheetMaximumSize();
  }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
  /*! \internal Based on the context menu mode and the window, the 
                message is either trapped or passed to the parent.*/
  
  // TPS_None: Eat all messages
  if( TPS_None == m_eContextMenuMode )
    return;

  // TPS_All: Delegate everything to the parent (TreePropSheet mode).
  if( TPS_All == m_eContextMenuMode )
    CTreePropSheetBase::OnContextMenu( pWnd, point );

  // TPS_PropertySheetControls: Eat the messages for the tree and the 
  // frame control. The other windows are own by the property sheets 
  // and therefore should be notified.
  ASSERT( TPS_PropertySheetControls == m_eContextMenuMode );
  if( pWnd->GetSafeHwnd() != GetPageTreeControl()->GetSafeHwnd() &&
      GetFrameControl() && pWnd->GetSafeHwnd() != GetFrameControl()->GetWnd()->GetSafeHwnd() )
    CTreePropSheetBase::OnContextMenu( pWnd, point );
}

//////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetEx::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	/* Forward help notification to property page in order to keep behavior
     consistent with CpropertyPage. 
     Thanks to  pablo_mag for the suggestion
     http://www.codeproject.com/property/TreePropSheetEx.asp?msg=973632#xx973632xx */
	
  CPropertyPage* page = GetActivePage();
  if( page != NULL )
  {
    page->SendMessage(WM_HELP, 0, reinterpret_cast<LPARAM>(pHelpInfo) );
    return TRUE;
  }

	return CTreePropSheetBase::OnHelpInfo(pHelpInfo);
}

} // namespace TreePropSheet

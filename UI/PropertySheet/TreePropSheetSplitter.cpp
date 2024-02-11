// RealTimeSplitter.cpp
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
//  * This code is an update of SimpleSplitter written by Robert A. T. Kaldy and 
//  * published on code project at http://www.codeproject.com/splitter/kaldysimplesplitter.asp.
//  *
//  *********************************************************************/
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreePropSheetSplitter.h"

namespace TreePropSheet
{
#define FULL_SIZE 32768

inline int MulDivRound(int x, int mul, int div)
{
	return (x * mul + div / 2) / div;
}

BEGIN_MESSAGE_MAP(CTreePropSheetSplitter, CWnd)
	//{{AFX_MSG_MAP(CTreePropSheetSplitter)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_NCCREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CTreePropSheetSplitter::CTreePropSheetSplitter(const int nPanes, const UINT nOrientation, const int nMinSize, const int nBarThickness):
  m_nPanes(nPanes),
  m_nOrientation(nOrientation),
  m_nBarThickness(nBarThickness),
  m_nFrozenPaneCount(0),
  m_bRealtimeUpdate(false),
  m_bAllowUserResizing(true)
{
	ASSERT(nMinSize >= 0);

  // Common initialization
  CommonInit();
  // Set minimum pane sizes
 	for (int iPaneCnt = 0; iPaneCnt < m_nPanes; iPaneCnt++)
  {
    // Initialize minimum pane sizes.
    m_pzMinSize[iPaneCnt] = nMinSize;
  }
}

CTreePropSheetSplitter::CTreePropSheetSplitter(const int nPanes, const UINT nOrientation, const int* pzMinSize, const int nBarThickness):
  m_nPanes(nPanes),
  m_nOrientation(nOrientation),
  m_nBarThickness(nBarThickness),
  m_nFrozenPaneCount(0),
  m_bRealtimeUpdate(false),
  m_bAllowUserResizing(true)
{
  // Common initialization
  CommonInit();
  // Set minimum pane sizes
 	for (int iPaneCnt = 0; iPaneCnt < m_nPanes; iPaneCnt++)
  {
    // Initialize minimum pane sizes.
    m_pzMinSize[iPaneCnt] = pzMinSize[iPaneCnt];
  }

}

CTreePropSheetSplitter::~CTreePropSheetSplitter()
{
	delete[] m_pane;
	delete[] m_size;
	delete[] m_orig;
  delete[] m_frozen;
  delete[] m_pzMinSize;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetSplitter::Create(CWnd* pParent, const CRect& rect, UINT nID)
{
	ASSERT(pParent);

	BOOL bRet = CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, GetCursorHandle(), NULL, NULL), NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID, NULL);
  return bRet;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetSplitter::Create(CWnd* pParent, UINT nID)
{
	CRect rcOuter;

	ASSERT(pParent);
	pParent->GetClientRect(&rcOuter);

	return Create( pParent, rcOuter, nID );
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetSplitter::CreatePane(int nIndex, CWnd* pPaneWnd, DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszClassName)
{
	CRect rcPane;

	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	m_pane[nIndex] = pPaneWnd;
	dwStyle |= WS_CHILD | WS_VISIBLE;
	GetPaneRect(nIndex, rcPane);
	return pPaneWnd->CreateEx(dwExStyle, lpszClassName, NULL, dwStyle, rcPane, this, AFX_IDW_PANE_FIRST + nIndex);
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::SetPane(int nIndex, CWnd* pPaneWnd)
{
	CRect rcPane;

	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	ASSERT(pPaneWnd);
	ASSERT(pPaneWnd->m_hWnd);

	m_pane[nIndex] = pPaneWnd;
	GetPaneRect(nIndex, rcPane);
	pPaneWnd->MoveWindow(rcPane, false);
}

///////////////////////////////////////////////////////////////////////////////
//
CWnd* CTreePropSheetSplitter::GetPane(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	return m_pane[nIndex];
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::SetActivePane(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	m_pane[nIndex]->SetFocus();
}

///////////////////////////////////////////////////////////////////////////////
//
CWnd* CTreePropSheetSplitter::GetActivePane(int& nIndex) const
{
	for (int i = 0; i < m_nPanes; i++)
		if (m_pane[i]->GetFocus())
		{
			nIndex = i;
			return m_pane[i];
		}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::SetPaneSizes(const int* sizes)
{
	int i, total = 0, total_in = 0;
	
	for (i = 0; i < m_nPanes; i++)
	{
		ASSERT(sizes[i] >= 0);
		total += sizes[i];
	}
	for (i = 0; i < m_nPanes - 1; i++)
	{
		m_size[i] = MulDivRound(sizes[i], FULL_SIZE, total);
		total_in += m_size[i];
	}
	m_size[m_nPanes - 1] = FULL_SIZE - total_in;
	RecalcLayout();
	ResizePanes();
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::GetPaneRect(int nIndex, CRect& rcPane) const
{
	ASSERT(nIndex >= 0 && nIndex < m_nPanes);
	GetAdjustedClientRect(&rcPane);
	if (m_nOrientation == SSP_HORZ)
	{
		rcPane.left = m_orig[nIndex];
		rcPane.right = m_orig[nIndex + 1] - m_nBarThickness;
	}
	else
	{
		rcPane.top = m_orig[nIndex];
		rcPane.bottom = m_orig[nIndex + 1] - m_nBarThickness;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::GetBarRect(int nIndex, CRect& rcBar) const
{
	ASSERT(nIndex > 0 && nIndex < m_nPanes - 1);
	GetAdjustedClientRect(&rcBar);
	if (m_nOrientation == SSP_HORZ)
	{
		rcBar.left = m_orig[nIndex];
		rcBar.right = m_orig[nIndex + 1] - m_nBarThickness;
	}
	else
	{
		rcBar.top = m_orig[nIndex];
		rcBar.bottom = m_orig[nIndex + 1] - m_nBarThickness;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::SetFrozenPanes(const bool* frozenPanes)
{
  m_nFrozenPaneCount = 0;
	for( int i = 0; i < m_nPanes; ++i )
  {
    m_frozen[i] = frozenPanes[i];
    if( frozenPanes[i] )
      ++m_nFrozenPaneCount;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::ResetFrozenPanes()
{
	for( int i = 0; i < m_nPanes; ++i )
  {
    m_frozen[i] = false;
  }

  m_nFrozenPaneCount = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::SetRealtimeUpdate(const bool bRealtime)
{
	m_bRealtimeUpdate = bRealtime; 
}

///////////////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetSplitter::IsRealtimeUpdate() const
{
	return m_bRealtimeUpdate;
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::SetAllowUserResizing(const bool bAllowUserResizing)
{
	m_bAllowUserResizing  = bAllowUserResizing;

  // If the window exists, reset the cursor.
  if( ::IsWindow( GetSafeHwnd() ) )
  {
    ::SetClassLong( GetSafeHwnd(), GCL_HCURSOR, (LONG)GetCursorHandle() );
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool CTreePropSheetSplitter::IsAllowUserResizing() const
{
	return m_bAllowUserResizing; 
}

///////////////////////////////////////////////////////////////////////////////
// Overridables
///////////////////////////////////////////////////////////////////////////////

void CTreePropSheetSplitter::UpdateParentRect(LPCRECT lpRectUpdate)
{
  GetParent()->RedrawWindow( lpRectUpdate, NULL, RDW_UPDATENOW | RDW_INVALIDATE );
}

///////////////////////////////////////////////////////////////////////////////
// Helpers
///////////////////////////////////////////////////////////////////////////////

HCURSOR CTreePropSheetSplitter::GetCursorHandle() const
{
	HCURSOR hCursor;

  LPCTSTR lpszCursor = IDC_ARROW;

  if( m_bAllowUserResizing )
    lpszCursor = m_nOrientation == SSP_HORZ ? IDC_SIZEWE : IDC_SIZENS;

	hCursor = (HCURSOR)::LoadImage(0, lpszCursor, IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_SHARED );
  return hCursor;
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::CommonInit()
{
	ASSERT(m_nPanes > 0);
	ASSERT(m_nOrientation == SSP_HORZ || m_nOrientation == SSP_VERT);
	ASSERT(m_nBarThickness >= 0);

	int total = 0;

  m_pane = new CWnd*[m_nPanes];
	m_size = new int[m_nPanes];
	m_orig = new int[m_nPanes + 1];
  m_pzMinSize = new int[m_nPanes];
  m_frozen = new bool[m_nPanes];
	::ZeroMemory(m_pane, m_nPanes * sizeof(CWnd*));
	for (int i = 0; i < m_nPanes - 1; i++)
	{
		// Initialize sizes. Default, set equal size to all panes
    m_size[i] = (FULL_SIZE + m_nPanes / 2) / m_nPanes;
		total += m_size[i];
	}

  m_size[m_nPanes - 1] = FULL_SIZE - total;
  ResetFrozenPanes();
}

///////////////////////////////////////////////////////////////////////////////
// CTreePropSheetSplitter protected
///////////////////////////////////////////////////////////////////////////////

void CTreePropSheetSplitter::RecalcLayout()
{
	int i, size_sum, remain, remain_new = 0;
	bool bGrow = true;
	CRect rcOuter;

	GetAdjustedClientRect(&rcOuter);


	size_sum = m_nOrientation == SSP_HORZ ? rcOuter.Width() : rcOuter.Height();
	size_sum -= (m_nPanes - 1) * m_nBarThickness;

  // Frozen panes
  /* Get the previous size and adjust the size of the frozen pane. We need to  
     do this as the size array contains ratio to FULL_SIZE. */
  
  // If we have some frozen columns use this algorithm otherwise adjust for minimum sizes.
  // If all the panes are frozen, do not use frozen pane algorithm.
  bool bFrozenOnly = m_nFrozenPaneCount && m_nPanes - m_nFrozenPaneCount;
  if( bFrozenOnly )
  {
    int prev_size_sum = m_orig[m_nPanes] - m_orig[0] - m_nBarThickness * m_nPanes;
    int non_frozen_count = m_nPanes - m_nFrozenPaneCount;
    int nTotalPanes = 0;
	  for( int i = 0; i < m_nPanes; ++i )
    {
      if( i + 1 == m_nPanes )
      {
        // Assigned all remaining value to the last pane without computation.
        // This is done in order to prevent propagation of computation errors. 
        m_size[i] = FULL_SIZE - nTotalPanes;
      }
      else
      {
        if( m_frozen[i] )
        { // Frozen pane
          m_size[i] = MulDivRound( m_size[i], prev_size_sum, size_sum);
        }
        else
        { // Non frozen pane
          m_size[i] = MulDivRound( m_size[i],  prev_size_sum, size_sum ) +
                      MulDivRound( size_sum - prev_size_sum, FULL_SIZE, size_sum * non_frozen_count );
        }
      }
      // If a pane become to small, it will be adjusted with the proportional algorithm.
      bFrozenOnly &= ( MulDivRound(m_size[i], size_sum, FULL_SIZE) <= m_pzMinSize[i] );
      // Compute running sumof pane sizes.
      nTotalPanes += m_size[i];
    }
  }

  /* The previous section might set the flag bFrozenOnly in case a pane became too small.
     In this case, we also want to execute this algorithm in order to respect the minimum
     pane size constraint. */
  if( !bFrozenOnly )
  {
	  while (bGrow)                           // adjust sizes on the beginning
	  {                                       // and while we have growed something
		  bGrow = false;
		  remain = remain_new = FULL_SIZE;
		  for (i = 0; i < m_nPanes; i++)        // grow small panes to minimal size
			  if ( MulDivRound(m_size[i], size_sum, FULL_SIZE) <= m_pzMinSize[i])
			  {
				  remain -= m_size[i];
				  if (MulDivRound(m_size[i], size_sum, FULL_SIZE) < m_pzMinSize[i])
				  {
					  if (m_pzMinSize[i] > size_sum)
						  m_size[i] = FULL_SIZE;
					  else
						  m_size[i] = MulDivRound(m_pzMinSize[i], FULL_SIZE, size_sum);
					  bGrow = true;
				  }
				  remain_new -= m_size[i];
			  }
		  if (remain_new <= 0)                  // if there isn't place to all panes
		  {                                     // set the minimal size to the leftmost/topmost
			  remain = FULL_SIZE;                 // and set zero size to the remainimg
			  for (i = 0; i < m_nPanes; i++)
			  {
				  if (size_sum == 0)
					  m_size[i] = 0;
				  else
					  m_size[i] = MulDivRound(m_pzMinSize[i], FULL_SIZE, size_sum);
				  if (m_size[i] > remain)
					  m_size[i] = remain;
				  remain -= m_size[i];
			  }
			  break;
		  }
		  if (remain_new != FULL_SIZE)          // adjust other pane sizes, if we have growed some
			  for (i = 0; i < m_nPanes; i++)
				  if ( MulDivRound(m_size[i], size_sum, FULL_SIZE) != m_pzMinSize[i])
					  m_size[i] = MulDivRound(m_size[i], remain_new, remain);
	  }
  }

  // calculate positions (in pixels) from relative sizes
  m_orig[0] = ( m_nOrientation == SSP_HORZ ? rcOuter.left:rcOuter.top );
	for (i = 0; i < m_nPanes - 1; i++)
		m_orig[i + 1] = m_orig[i] + MulDivRound(m_size[i], size_sum, FULL_SIZE) + m_nBarThickness;
	m_orig[m_nPanes] = m_orig[0] + size_sum + m_nBarThickness * m_nPanes;
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::ResizePanes()
{
	int i;
	CRect rcOuter;

	GetAdjustedClientRect(&rcOuter);
	if (m_nOrientation == SSP_HORZ)	
	  for (i = 0; i < m_nPanes; i++)
		{
			if (m_pane[i])
				m_pane[i]->MoveWindow(m_orig[i], rcOuter.top, m_orig[i + 1] - m_orig[i] - m_nBarThickness, rcOuter.Height(), FALSE);
		}
	else
		for (i = 0; i < m_nPanes; i++)
		{
			if (m_pane[i])
				m_pane[i]->MoveWindow(rcOuter.left, m_orig[i], rcOuter.Width(), m_orig[i + 1] - m_orig[i] - m_nBarThickness, FALSE);
		}
  UpdateParentRect( &rcOuter );
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::InvertTracker()
{
	CDC* pDC = GetDC();
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush;

  CRect rcWnd;
	GetAdjustedClientRect(&rcWnd);
  
	hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	if (m_nOrientation == SSP_HORZ)
		pDC->PatBlt(m_nTracker - m_nBarThickness - rcWnd.left, 0, m_nBarThickness, m_nTrackerLength, PATINVERT);
	else
		pDC->PatBlt(0, m_nTracker - m_nBarThickness - rcWnd.top, m_nTrackerLength, m_nBarThickness, PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::GetAdjustedClientRect(CRect* pRect) const
{
	GetWindowRect(pRect);
	GetParent()->ScreenToClient(pRect);
}

///////////////////////////////////////////////////////////////////////////////
// CTreePropSheetSplitter messages
///////////////////////////////////////////////////////////////////////////////

void CTreePropSheetSplitter::OnPaint() 
{
	CPaintDC dc(this);
	CRect rcClip, rcClipPane;
	int i;

	// retrieve the background brush
  HWND hWnd = GetParent()->GetSafeHwnd();
	// send a message to the dialog box
	HBRUSH  hBrush = (HBRUSH)::SendMessage(hWnd, WM_CTLCOLORDLG, (WPARAM)dc.m_ps.hdc, (LPARAM)hWnd);
  ASSERT( hBrush );

  CRect rcWnd;
	GetAdjustedClientRect(&rcWnd);
  
	dc.GetClipBox(&rcClip);
  CRect rect;
  int top, left;
	if (m_nOrientation == SSP_HORZ)
  {
    for (i = 1; i < m_nPanes; i++)
    {
      left = m_orig[i] - m_nBarThickness - rcWnd.left;
      top = rcClip.top;
      rect.SetRect( left, top, left + m_nBarThickness, top + rcClip.Height() );
      dc.FillRect( &rect, CBrush::FromHandle(hBrush) );
    }
  }
	else
  {
    for (i = 1; i < m_nPanes; i++)
    {
      left = rcClip.left;
      top = m_orig[i] - m_nBarThickness - rcWnd.top;
      rect.SetRect( left, top, left + rcClip.Width(), top + m_nBarThickness );
      dc.FillRect( &rect, CBrush::FromHandle(hBrush) );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	RecalcLayout();
	ResizePanes();
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
  UNREFERENCED_PARAMETER(nFlags);

  // Must be in AllowUserResizing mode.
  if( !m_bAllowUserResizing )
    return;

	CRect rcClient;
	int mouse_pos = m_nOrientation == SSP_HORZ ? point.x : point.y;

	SetCapture();
	for (m_nTrackIndex = 1; (m_nTrackIndex < m_nPanes && m_orig[m_nTrackIndex] < mouse_pos); m_nTrackIndex++);
	m_nTracker = m_orig[m_nTrackIndex];
	m_nTrackerMouseOffset = mouse_pos - m_nTracker;
	GetWindowRect(&rcClient);
	GetParent()->ScreenToClient(&rcClient);
	m_nTrackerLength = m_nOrientation == SSP_HORZ ? rcClient.Height() : rcClient.Width();
	InvertTracker();
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
  UNREFERENCED_PARAMETER(nFlags);
  UNREFERENCED_PARAMETER(point);

  if (GetCapture() != this) 
		return;

	CRect rcOuter;
	int size_sum;

	GetAdjustedClientRect(&rcOuter);
	size_sum = m_nOrientation == SSP_HORZ ? rcOuter.Width() : rcOuter.Height();
	size_sum -= (m_nPanes - 1) * m_nBarThickness;

  InvertTracker();
	ReleaseCapture();
	m_orig[m_nTrackIndex] = m_nTracker;
	m_size[m_nTrackIndex - 1] = MulDivRound(m_orig[m_nTrackIndex] - m_orig[m_nTrackIndex - 1] - m_nBarThickness, FULL_SIZE, size_sum);
	m_size[m_nTrackIndex]     = MulDivRound(m_orig[m_nTrackIndex + 1] - m_orig[m_nTrackIndex] - m_nBarThickness, FULL_SIZE, size_sum);
	ResizePanes();
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
  UNREFERENCED_PARAMETER(nFlags);

  if (GetCapture() != this) 
		return;
	InvertTracker();

  m_nTracker = (m_nOrientation == SSP_HORZ ? point.x : point.y) - m_nTrackerMouseOffset;
  ASSERT( m_nTrackIndex > 0 );
	if (m_nTracker > m_orig[m_nTrackIndex + 1] - m_nBarThickness - m_pzMinSize[m_nTrackIndex])
  {
    m_nTracker = m_orig[m_nTrackIndex + 1] - m_nBarThickness - m_pzMinSize[m_nTrackIndex];
  }
  else if (m_nTracker < m_orig[m_nTrackIndex - 1] + m_nBarThickness + m_pzMinSize[m_nTrackIndex-1])
  {
    m_nTracker = m_orig[m_nTrackIndex - 1] + m_nBarThickness + m_pzMinSize[m_nTrackIndex-1];
  }
  
  InvertTracker();

	CRect rcOuter;
	GetAdjustedClientRect(&rcOuter);
	// Real-time update.
  if( m_bRealtimeUpdate )
  {
    m_orig[m_nTrackIndex] = m_nTracker;
    ResizePanes();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetSplitter::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!CWnd::OnNcCreate(lpCreateStruct))
		return FALSE;

	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);
	pParent->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
void CTreePropSheetSplitter::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags |= SWP_NOCOPYBITS;
	CWnd::OnWindowPosChanging(lpwndpos);
}

}; // namespace TreePropSheet

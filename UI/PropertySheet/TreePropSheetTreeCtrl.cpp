// TreePropSheetTreeCtrl.cpp
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

#include "stdafx.h"
#include "TreePropSheetTreeCtrl.h"
#include "TreePropSheetBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TreePropSheet
{
/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTreePropSheetTreeCtrl, CTreeCtrl)

CTreePropSheetTreeCtrl::CTreePropSheetTreeCtrl()
{
}

CTreePropSheetTreeCtrl::~CTreePropSheetTreeCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// Properties
/////////////////////////////////////////////////////////////////////////////

void CTreePropSheetTreeCtrl::SetItemBold(const HTREEITEM hItem, const bool bBold)
{
	SetItemState(hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD);
}

//////////////////////////////////////////////////////////////////////
//
BOOL CTreePropSheetTreeCtrl::GetItemBold(const HTREEITEM hItem) const
{
  return (GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD)?true:false;
}

//////////////////////////////////////////////////////////////////////
//
void CTreePropSheetTreeCtrl::SetItemColor(const HTREEITEM hItem, const COLORREF color)
{
  // If the color is the text color of the tree, remove the related entry 
  // from the tree.
  if( color == GetTextColor() )
  {
    m_mapTreeItemProperties.erase( hItem );
  }
  else
  {
    sTreeItemProperties treeItemProperty = m_mapTreeItemProperties[hItem];
    treeItemProperty.color = color;
    m_mapTreeItemProperties[hItem] = treeItemProperty;
  }
}

//////////////////////////////////////////////////////////////////////
//
COLORREF CTreePropSheetTreeCtrl::GetItemColor(const HTREEITEM hItem) const
{
  tTreeItemProperties::const_iterator cit = m_mapTreeItemProperties.find( hItem );
  if( m_mapTreeItemProperties.end() == cit )
  {
    return GetTextColor();
  }
  else
  {
    return cit->second.color;
  }
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTreePropSheetTreeCtrl, CTreeCtrl)
  //{{AFX_MSG_MAP(CTreePropSheetTreeCtrl)
  ON_WM_PAINT()
  ON_REGISTERED_MESSAGE(WMU_ENABLETREEITEM, OnSetItemColor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void CTreePropSheetTreeCtrl::OnPaint() 
{
	CPaintDC dc(this);

	// Create a memory DC compatible with the paint DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );
	
	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );
	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();
	
	// First let the control do its default drawing.
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	HTREEITEM hItem = GetFirstVisibleItem();

	int iItemCount = GetVisibleCount() + 1;
	while(hItem && iItemCount--)
	{		
		CRect rect;

		// Do not update selected or drop highlighted items
		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
	
		if( 0 == (GetItemState(hItem, selflag) & selflag) )
		{
      tTreeItemProperties::const_iterator cit = m_mapTreeItemProperties.find( hItem );
      if( cit != m_mapTreeItemProperties.end() )
      {
			  CFont *pFontDC;
			  CFont fontDC;
			  LOGFONT logfont;

  		  // Create a new font.
        CFont *pFont = GetFont();
			  pFont->GetLogFont( &logfont );

        // Update if bold.
			  if(GetItemBold(hItem))
        {
          logfont.lfWeight = 700;
        }

			  fontDC.CreateFontIndirect(&logfont);
			  pFontDC = memDC.SelectObject(&fontDC );

			  // Set the color.
        memDC.SetTextColor( cit->second.color);

			  CString sItem = GetItemText(hItem);

			  GetItemRect(hItem, &rect, TRUE);
//			  memDC.SetBkColor( GetSysColor(COLOR_WINDOW) );
			  memDC.TextOut(rect.left + 2, rect.top + 1, sItem);
			  
			  memDC.SelectObject(pFontDC);
      }
		}
		hItem = GetNextVisibleItem(hItem);
	}

	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
				rcClip.left, rcClip.top, SRCCOPY);

	memDC.DeleteDC();
}

//////////////////////////////////////////////////////////////////////
//
LRESULT CTreePropSheetTreeCtrl::OnSetItemColor(WPARAM wParam, LPARAM lParam)
{
  // Convert parameters
  HTREEITEM hItem = reinterpret_cast<HTREEITEM>( wParam );
  COLORREF color = (COLORREF)lParam;

  SetItemColor( hItem, color );
  return 1L;
}

}  //namespace TreePropSheet

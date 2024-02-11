// PropPageFrameOffice2003.cpp: implementation of the CPropPageFrameOffice2003 class.
//
//////////////////////////////////////////////////////////////////////
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
#include "PropPageFrameOffice2003.h"
#include "ThemeLibEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Define constants that are in the Platform SDK, but not in the default VC6
// installation.
// Thank to Don M for suggestion
// (http://www.codeproject.com/property/TreePropSheetEx.asp?msg=939854#xx939854xx)
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION 27
#endif

#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION 28
#endif

#ifndef WP_FRAMELEFT
#define WP_FRAMELEFT 7
#endif

#ifndef FS_ACTIVE
#define FS_ACTIVE 1
#endif

#ifndef TMT_BORDERCOLOR
#define TMT_BORDERCOLOR 3801
#endif

namespace TreePropSheet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropPageFrameOffice2003::CPropPageFrameOffice2003()
{
}

CPropPageFrameOffice2003::~CPropPageFrameOffice2003()
{
	if (m_fontCaption.GetSafeHandle())
		m_fontCaption.DeleteObject();
}

/////////////////////////////////////////////////////////////////////
// Overridings
/////////////////////////////////////////////////////////////////////

BOOL CPropPageFrameOffice2003::Create(DWORD dwWindowStyle, const RECT &rect, CWnd *pwndParent, UINT nID)
{
	if (!m_fontCaption.GetSafeHandle())
		m_fontCaption.CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, DEFAULT_FONT);

  if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
	  return CWnd::CreateEx(
      0,
      AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW), GetSysColorBrush(COLOR_3DFACE)),
		  _T("PropPageFrameBordered"),
		  dwWindowStyle, rect, pwndParent, nID);
  }
	else
	{
	  return CWnd::CreateEx(
      WS_EX_CLIENTEDGE/*|WS_EX_TRANSPARENT*/,
      AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW), GetSysColorBrush(COLOR_3DFACE)),
		  _T("PropPageFrameBordered"),
		  dwWindowStyle, rect, pwndParent, nID);
  }
}

/////////////////////////////////////////////////////////////////////
// CPropPageFrame overriding
/////////////////////////////////////////////////////////////////////

void CPropPageFrameOffice2003::DrawCaption(CDC *pDc,CRect rect,LPCTSTR lpszCaption,HICON hIcon)
{
	rect.left+= 4;
  int nLineLeft = rect.left;

  // draw icon
	if (hIcon && m_Images.GetSafeHandle() && m_Images.GetImageCount() == 1)
	{
		IMAGEINFO	ii;
		m_Images.GetImageInfo(0, &ii);
		CPoint		pt(3, rect.CenterPoint().y - (ii.rcImage.bottom-ii.rcImage.top)/2);
		m_Images.Draw(pDc, 0, pt, ILD_TRANSPARENT);
		rect.left+= (ii.rcImage.right-ii.rcImage.left) + 3;
	}

	// draw text
	rect.left+= 2;

	COLORREF	clrPrev = pDc->SetTextColor( GetSysColor( COLOR_WINDOWTEXT ) );
	int				nBkStyle = pDc->SetBkMode(TRANSPARENT );
	//CFont			*pFont = (CFont*)pDc->SelectStockObject( SYSTEM_FONT );
	CFont			*pFont= pDc->SelectObject(&m_fontCaption);
	pDc->DrawText( lpszCaption, rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

	pDc->SetTextColor(clrPrev);
	pDc->SetBkMode(nBkStyle);
	pDc->SelectObject(pFont);

  // draw line
  CPen pen( PS_SOLID, 1, ::GetSysColor( COLOR_GRADIENTACTIVECAPTION ) );
  CPen* pPrevPen = pDc->SelectObject( &pen );
  pDc->MoveTo( nLineLeft, rect.bottom - 1);
  pDc->LineTo( rect.right - 4, rect.bottom - 1);
  pDc->SelectObject( pPrevPen );
}

/////////////////////////////////////////////////////////////////////
// CPropPageFrameEx overrides
/////////////////////////////////////////////////////////////////////

void CPropPageFrameOffice2003::DrawBackground(CDC* pDc)
{
  // Draw a frame in themed mode.
  if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
    COLORREF color = ::GetSysColor( COLOR_GRADIENTINACTIVECAPTION );
		HTHEME	hTheme = GetThemeLib().OpenThemeData(m_hWnd, L"TREEVIEW");
		if (hTheme)
		{
      GetThemeLib().GetThemeColor( hTheme, WP_FRAMELEFT, FS_ACTIVE, TMT_BORDERCOLOR, &color );
		}

    CWnd::OnEraseBkgnd( pDc );
 	  CRect	rect;
	  GetClientRect(rect);
   
    CPen pen( PS_SOLID, 1, color );
    CBrush brush( ::GetSysColor( COLOR_WINDOW ) );

    CPen* pPrevPen = pDc->SelectObject( &pen );
    CBrush* pPrevBrush =  pDc->SelectObject( &brush );

    pDc->Rectangle( &rect );

    pDc->SelectObject( pPrevPen );
    pDc->SelectObject( pPrevBrush );
  }
  else
  {
    // Draw our own background: always COLOR_WINDOW system color.
	  CWnd::OnEraseBkgnd( pDc );
	  CRect	rect;
	  GetClientRect(rect);
    ::FillRect(pDc->m_hDC, &rect, ::GetSysColorBrush( COLOR_WINDOW ) );
  }
}

};  // namespace TreePropSheet
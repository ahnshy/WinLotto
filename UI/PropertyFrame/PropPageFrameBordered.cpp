// PropPageFrameBordered.cpp: implementation of the CPropPageFrameBordered class.
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
#include "PropPageFrameBordered.h"
#include "ThemeLibEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TreePropSheet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropPageFrameBordered::CPropPageFrameBordered()
{
}

CPropPageFrameBordered::~CPropPageFrameBordered()
{
}

/////////////////////////////////////////////////////////////////////
// Overridings
/////////////////////////////////////////////////////////////////////

BOOL CPropPageFrameBordered::Create(DWORD dwWindowStyle, const RECT &rect, CWnd *pwndParent, UINT nID)
{
  if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
    // Delegate to base class.
    return CPropPageFrameEx::Create(dwWindowStyle, rect, pwndParent, nID);
  }
	else
	{
	  return CWnd::CreateEx(
      WS_EX_CLIENTEDGE|WS_EX_TRANSPARENT,
      AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW), GetSysColorBrush(COLOR_3DFACE)),
		  _T("PropPageFrameBordered"),
		  dwWindowStyle, rect, pwndParent, nID);
  }
}

/////////////////////////////////////////////////////////////////////
// CPropPageFrameEx overrides
/////////////////////////////////////////////////////////////////////

void CPropPageFrameBordered::DrawBackground(CDC* pDC)
{
  if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
    // Delegate to base class.
    CPropPageFrameEx::DrawBackground( pDC );
  }
	else
	{
    // Draw our own background.
		CWnd::OnEraseBkgnd( pDC );
		CRect	rect;
		GetClientRect(rect);
    ::FillRect(pDC->m_hDC, &rect, ::GetSysColorBrush( COLOR_BTNFACE ) );
	}
}

};  // namespace TreePropSheet
// PropPageFrameEx.cpp: implementation of the CPropPageFrameEx class.
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
#include "PropPageFrameEx.h"
#include "MemDC.h"
#include "ThemeLibEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TreePropSheet
{
// Uncomment the following line if you want to use the GDI FillGradient
// rather than the owner draw gradient. 
//#define USE_OWNER_DRAW_GRADIANT

#ifndef USE_OWNER_DRAW_GRADIANT
#include "Wingdi.h"
#pragma comment(lib, "Msimg32")
#else
void GradientFill(...) { }
#endif // USE_OWNER_DRAW_GRADIANT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropPageFrameEx::CPropPageFrameEx()
 : m_pGradientFn(NULL)
{
  InitialGradientDrawingFunction();
}

CPropPageFrameEx::~CPropPageFrameEx()
{
	if (m_Images.GetSafeHandle())
		m_Images.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CPropPageFrameEx, CWnd)
	//{{AFX_MSG_MAP(CPropPageFrameEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////
// Overridings
/////////////////////////////////////////////////////////////////////

BOOL CPropPageFrameEx::Create(DWORD dwWindowStyle, const RECT &rect, CWnd *pwndParent, UINT nID)
{
	return CWnd::Create(
		AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW), GetSysColorBrush(COLOR_3DFACE)),
		_T("PropPageFrameEx"),
		dwWindowStyle, rect, pwndParent, nID);
}

/////////////////////////////////////////////////////////////////////
//
CWnd* CPropPageFrameEx::GetWnd()
{
	return static_cast<CWnd*>(this);
}

/////////////////////////////////////////////////////////////////////
//
void CPropPageFrameEx::SetCaption(LPCTSTR lpszCaption, HICON hIcon /*= NULL*/)
{
	CPropPageFrame::SetCaption(lpszCaption, hIcon);

	// build image list
	if (m_Images.GetSafeHandle())
		m_Images.DeleteImageList();
	if (hIcon)
	{
		ICONINFO	ii;
		if (!GetIconInfo(hIcon, &ii))
			return;

		CBitmap	bmMask;
		bmMask.Attach(ii.hbmMask);
		if (ii.hbmColor) DeleteObject(ii.hbmColor);

		BITMAP	bm;
		bmMask.GetBitmap(&bm);

		if (!m_Images.Create(bm.bmWidth, bm.bmHeight, ILC_COLOR32|ILC_MASK, 0, 1))
			return;

		if (m_Images.Add(hIcon) == -1)
			m_Images.DeleteImageList();
	}
}

/////////////////////////////////////////////////////////////////////
//
CRect CPropPageFrameEx::CalcMsgArea()
{
	CRect	rect;
	GetClientRect(rect);
	if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
		HTHEME	hTheme = GetThemeLib().OpenThemeData(m_hWnd, L"Tab");
		if (hTheme)
		{
			CRect	rectContent;
			CDC		*pDc = GetDC();
			GetThemeLib().GetThemeBackgroundContentRect(hTheme, pDc->m_hDC, TABP_PANE, 0, rect, rectContent);
			ReleaseDC(pDc);
			GetThemeLib().CloseThemeData(hTheme);
			
			if (GetShowCaption())
				rectContent.top = rect.top+GetCaptionHeight()+1;
			rect = rectContent;
		}
	}
	else if (GetShowCaption())
  {
    rect.top+= GetCaptionHeight()+1;
  }

	return rect;
}

/////////////////////////////////////////////////////////////////////
//
CRect CPropPageFrameEx::CalcCaptionArea()
{
	CRect	rect;
	GetClientRect(rect);
	if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
		HTHEME	hTheme = GetThemeLib().OpenThemeData(m_hWnd, L"Tab");
		if (hTheme)
		{
			CRect	rectContent;
			CDC		*pDc = GetDC();
			GetThemeLib().GetThemeBackgroundContentRect(hTheme, pDc->m_hDC, TABP_PANE, 0, rect, rectContent);
			ReleaseDC(pDc);
			GetThemeLib().CloseThemeData(hTheme);
			
			if (GetShowCaption())
				rectContent.bottom = rect.top+GetCaptionHeight();
			else
				rectContent.bottom = rectContent.top;

			rect = rectContent;
		}
	}
	else
	{
		if (GetShowCaption())
			rect.bottom = rect.top+GetCaptionHeight();
		else
			rect.bottom = rect.top;
	}

	return rect;
}

/////////////////////////////////////////////////////////////////////
//
void CPropPageFrameEx::DrawCaption(CDC *pDc, CRect rect, LPCTSTR lpszCaption, HICON hIcon)
{
	COLORREF	clrLeft = GetSysColor(COLOR_INACTIVECAPTION);
	COLORREF	clrRight = pDc->GetPixel(rect.right-1, rect.top);
	m_pGradientFn(pDc, rect, clrLeft, clrRight);

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

	COLORREF	clrPrev = pDc->SetTextColor(GetSysColor(COLOR_CAPTIONTEXT));
	int				nBkStyle = pDc->SetBkMode(TRANSPARENT);
	CFont			*pFont = (CFont*)pDc->SelectStockObject(SYSTEM_FONT);

	pDc->DrawText(lpszCaption, rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	pDc->SetTextColor(clrPrev);
	pDc->SetBkMode(nBkStyle);
	pDc->SelectObject(pFont);
}

/////////////////////////////////////////////////////////////////////
// Methods
/////////////////////////////////////////////////////////////////////

const CThemeLibEx& CPropPageFrameEx::GetThemeLib() const
{
	static CThemeLibEx themeLib;
  return themeLib;
}

/////////////////////////////////////////////////////////////////////
// Overridables
/////////////////////////////////////////////////////////////////////

void CPropPageFrameEx::DrawBackground(CDC* pDC)
{
	if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive() && GetThemeLib().IsAppThemed() )
	{
		HTHEME	hTheme = GetThemeLib().OpenThemeData(m_hWnd, L"Tab");
		if (hTheme)
		{
			CRect	rect;
			GetClientRect(rect);
			GetThemeLib().DrawThemeBackground(hTheme, pDC->m_hDC, TABP_PANE, 0, rect, NULL);
			GetThemeLib().CloseThemeData(hTheme);
		}
  }
	else
	{
		CWnd::OnEraseBkgnd(pDC);
		CRect	rect;
		GetClientRect(rect);
    ::FillRect(pDC->m_hDC, &rect, ::GetSysColorBrush( COLOR_BTNFACE ) );
	}
}

/////////////////////////////////////////////////////////////////////
// Implementation helpers
/////////////////////////////////////////////////////////////////////

void CPropPageFrameEx::FillOwnerDrawnGradientRectH(CDC *pDc, const RECT &rect, COLORREF clrLeft, COLORREF clrRight)
{
	// pre calculation
	int	nSteps = rect.right-rect.left;
	int	nRRange = GetRValue(clrRight)-GetRValue(clrLeft);
	int	nGRange = GetGValue(clrRight)-GetGValue(clrLeft);
	int	nBRange = GetBValue(clrRight)-GetBValue(clrLeft);

	double	dRStep = (double)nRRange/(double)nSteps;
	double	dGStep = (double)nGRange/(double)nSteps;
	double	dBStep = (double)nBRange/(double)nSteps;

	double	dR = (double)GetRValue(clrLeft);
	double	dG = (double)GetGValue(clrLeft);
	double	dB = (double)GetBValue(clrLeft);

	CPen	*pPrevPen = NULL;
	for (int x = rect.left; x <= rect.right; ++x)
	{
		CPen	Pen(PS_SOLID, 1, RGB((BYTE)dR, (BYTE)dG, (BYTE)dB));
		pPrevPen = pDc->SelectObject(&Pen);
		pDc->MoveTo(x, rect.top);
		pDc->LineTo(x, rect.bottom);
		pDc->SelectObject(pPrevPen);
		
		dR+= dRStep;
		dG+= dGStep;
		dB+= dBStep;
	}
}

/////////////////////////////////////////////////////////////////////
//
void CPropPageFrameEx::FillGDIGradientRectH(CDC *pDc, const RECT &rect, COLORREF clrLeft, COLORREF clrRight)
{
  TRIVERTEX        vert[2] ;
  GRADIENT_RECT    gRect;
  vert [0] .x      = rect.left;
  vert [0] .y      = rect.top;
  vert [0] .Red    = (short)(GetRValue( clrLeft ) << 8);
  vert [0] .Green  = (short)(GetGValue( clrLeft ) << 8);
  vert [0] .Blue   = (short)(GetBValue( clrLeft ) << 8);
  vert [0] .Alpha  = 0x0000;

  vert [1] .x      = rect.right;
  vert [1] .y      = rect.bottom; 
  vert [1] .Red    = (short)(GetRValue( clrRight ) << 8);
  vert [1] .Green  = (short)(GetGValue( clrRight ) << 8);
  vert [1] .Blue   = (short)(GetBValue( clrRight ) << 8);
  vert [1] .Alpha  = 0x0000;

  gRect.UpperLeft  = 0;
  gRect.LowerRight = 1;

  GradientFill( pDc->m_hDC,vert,2,&gRect,1, GRADIENT_FILL_RECT_H ); 
}

void CPropPageFrameEx::InitialGradientDrawingFunction()
{
  /*! \internal Only the #define USE_OWNER_DRAW_GRADIANT
                is used for now. This could be extended
                to check at run-time if Msimg32.dll is
                available and use it. This was my initial
                intent but it did not make it in this
                release. */
#ifdef USE_OWNER_DRAW_GRADIANT
  m_pGradientFn = &CPropPageFrameEx::FillOwnerDrawnGradientRectH;
#else
  m_pGradientFn = &CPropPageFrameEx::FillGDIGradientRectH;
#endif
}

/////////////////////////////////////////////////////////////////////
// message handlers

void CPropPageFrameEx::OnPaint() 
{
	CPaintDC dc(this);
  CRect rect;
  GetClientRect(&rect);
  CMemDC memDC( &dc,&rect );

  // Draw the background.
  DrawBackground( &memDC );

  // Draw the title pane.
  Draw( &memDC );	
}


BOOL CPropPageFrameEx::OnEraseBkgnd(CDC* pDC) 
{
  UNREFERENCED_PARAMETER( pDC );

  return TRUE;
}

} //namespace TreePropSheet

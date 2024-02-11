// ThemeLibEx.cpp: implementation of the CThemeLibEx class.
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

#include "stdafx.h"
#include "ThemeLibEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TreePropSheet
{

#ifdef XPSUPPORT
	#define THEMECALL(f)						return (*m_p##f)
	#define GETTHEMECALL(f)					m_p##f = (_##f)GetProcAddress(m_hThemeLib, #f)
#else
	void ThemeDummyEx(...) {ASSERT(FALSE);}
	#define THEMECALL(f)						return 0; ThemeDummyEx
	#define GETTHEMECALL(f)					m_p##f = NULL
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThemeLibEx::CThemeLibEx()
:	m_hThemeLib(NULL)
{
#ifdef XPSUPPORT
	m_hThemeLib = LoadLibrary(_T("uxtheme.dll"));
	if (!m_hThemeLib)
		return;

	GETTHEMECALL(IsAppThemed);
	GETTHEMECALL(IsThemeActive);
	GETTHEMECALL(OpenThemeData);
	GETTHEMECALL(CloseThemeData);
	GETTHEMECALL(GetThemeBackgroundContentRect);
	GETTHEMECALL(DrawThemeBackground);
	GETTHEMECALL(GetThemeColor);
#endif
}

CThemeLibEx::~CThemeLibEx()
{
	if (m_hThemeLib)
		FreeLibrary(m_hThemeLib);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CThemeLibEx::IsAvailable() const
{
	return m_hThemeLib!=NULL;
}

BOOL CThemeLibEx::IsAppThemed() const
{
  THEMECALL(IsAppThemed)();
}

BOOL CThemeLibEx::IsThemeActive()  const
{
  THEMECALL(IsThemeActive)();
}

HTHEME CThemeLibEx::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)  const
{
  THEMECALL(OpenThemeData)(hwnd, pszClassList);
}

HRESULT CThemeLibEx::CloseThemeData(HTHEME hTheme)  const
{
  THEMECALL(CloseThemeData)(hTheme);
}

HRESULT CThemeLibEx::GetThemeBackgroundContentRect(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, int iStateId,  const RECT *pBoundingRect, OUT RECT *pContentRect) const
{
  THEMECALL(GetThemeBackgroundContentRect)(hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect);
}

HRESULT CThemeLibEx::DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect) const
{
  THEMECALL(DrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT CThemeLibEx::GetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor) const
{
  THEMECALL(GetThemeColor)(hTheme, iPartId, iStateId, iPropId, pColor);
}

};
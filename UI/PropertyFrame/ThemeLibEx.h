// ThemeLibEx.h: interface for the CThemeLibEx class.
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


#ifndef _TREEPROPSHEET_THEMELIBEX_H__INCLUDED_
#define _TREEPROPSHEET_THEMELIBEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TreePropSheet
{

//comment the following line, if you don't have installed the
//new platform SDK
#define XPSUPPORT

#ifdef XPSUPPORT
#include <UXTheme.h>
#include <TMSchema.h>
#pragma comment(lib, "UXTheme")
#endif

//-------------------------------------------------------------------
// class CThemeLibEx
//-------------------------------------------------------------------

#define THEMEAPITYPE(f)					typedef HRESULT (__stdcall *_##f)
#define THEMEAPITYPE_(t, f)			typedef t (__stdcall *_##f)
#define THEMEAPIPTR(f)					_##f m_p##f

#ifndef XPSUPPORT
	#define HTHEME									void*
	#define TABP_PANE								0
#endif

/*! \brief Helper class for XP Theme

Helper class for loading the uxtheme DLL and providing their 
functions.

This class is a copy of CThemeLib that is defined in 
TreePropSheet::CPropPageFrame. It has been extracted and 
renamed for reuse purpose. Constness have been changed for 
all calls.

\version 0.1
\author Sven Wiegand (original CThemeLib)
\author Yves Tkaczyk <yves@tkaczyk.net>*/
class CThemeLibEx  
{
// Construction/Destruction
public:
	CThemeLibEx();
	virtual ~CThemeLibEx();

// operations
public:
	/**
	Returns TRUE if the call wrappers are available, FALSE otherwise.
	*/
	BOOL IsAvailable() const;

// call wrappers
public:
  BOOL IsAppThemed() const;

	BOOL IsThemeActive() const;

	HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList) const;

	HRESULT CloseThemeData(HTHEME hTheme) const;

	HRESULT GetThemeBackgroundContentRect(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, int iStateId,  const RECT *pBoundingRect, OUT RECT *pContentRect) const;

	HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect) const;

	HRESULT GetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor) const;

// function pointers
private:
#ifdef XPSUPPORT
	THEMEAPITYPE_(BOOL, IsAppThemed)();
	THEMEAPIPTR(IsAppThemed);

  THEMEAPITYPE_(BOOL, IsThemeActive)();
	THEMEAPIPTR(IsThemeActive);

	THEMEAPITYPE_(HTHEME, OpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
	THEMEAPIPTR(OpenThemeData);

	THEMEAPITYPE(CloseThemeData)(HTHEME hTheme);
	THEMEAPIPTR(CloseThemeData);

	THEMEAPITYPE(GetThemeBackgroundContentRect)(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, int iStateId,  const RECT *pBoundingRect, OUT RECT *pContentRect);
	THEMEAPIPTR(GetThemeBackgroundContentRect);

  THEMEAPITYPE(DrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect);
	THEMEAPIPTR(DrawThemeBackground);

  THEMEAPITYPE(GetThemeColor)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor);
	THEMEAPIPTR(GetThemeColor);
#endif

// properties
private:
	/** instance handle to the library or NULL. */
	HINSTANCE m_hThemeLib;
};

};
#endif // !defined(_TREEPROPSHEET_THEMELIBEX_H__INCLUDED_)

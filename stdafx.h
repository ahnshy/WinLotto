// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

#define _AFXDLL
#define WINVER     0x0901

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include <afxglobals.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxmt.h>

#include "resource.h"

#define		DEFAULT_FONT _T("¸¼Àº °íµñ")

#pragma warning(disable: 4786)
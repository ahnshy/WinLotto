// TreePropSheetBordered.cpp: implementation of the CTreePropSheetBordered class.
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
#include "TreePropSheetBordered.h"
#include "../PropertyFrame/PropPageFrameBordered.h"

namespace TreePropSheet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTreePropSheetBordered, CTreePropSheetEx)

CTreePropSheetBordered::CTreePropSheetBordered()
{
}

CTreePropSheetBordered::CTreePropSheetBordered(UINT nIDCaption,CWnd* pParentWnd /*=NULL*/,UINT iSelectPage /*=0*/)
 : CTreePropSheetEx(nIDCaption, pParentWnd, iSelectPage)
{
}

CTreePropSheetBordered::CTreePropSheetBordered(LPCTSTR pszCaption,CWnd* pParentWnd /*=NULL*/,UINT iSelectPage /*=0*/)
 : CTreePropSheetEx(pszCaption, pParentWnd, iSelectPage)
{
}

CTreePropSheetBordered::~CTreePropSheetBordered()
{
}

//////////////////////////////////////////////////////////////////////
// Overrided implementation helpers
//////////////////////////////////////////////////////////////////////

CPropPageFrame* CTreePropSheetBordered::CreatePageFrame()
{
  return new CPropPageFrameBordered;
}

}; // namespace TreePropSheet
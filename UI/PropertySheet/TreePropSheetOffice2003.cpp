// TreePropSheetOffice2003.cpp: implementation of the CTreePropSheetOffice2003 class.
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
#include "TreePropSheetOffice2003.h"
#include "../PropertyFrame/PropPageFrameOffice2003.h"

namespace TreePropSheet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTreePropSheetOffice2003, CTreePropSheetEx)

CTreePropSheetOffice2003::CTreePropSheetOffice2003()
{
}

CTreePropSheetOffice2003::CTreePropSheetOffice2003(UINT nIDCaption,CWnd* pParentWnd /*=NULL*/,UINT iSelectPage /*=0*/)
 : CTreePropSheetEx(nIDCaption, pParentWnd, iSelectPage)
{
}

CTreePropSheetOffice2003::CTreePropSheetOffice2003(LPCTSTR pszCaption,CWnd* pParentWnd /*=NULL*/,UINT iSelectPage /*=0*/)
 : CTreePropSheetEx(pszCaption, pParentWnd, iSelectPage)
{
}

CTreePropSheetOffice2003::~CTreePropSheetOffice2003()
{
}

//////////////////////////////////////////////////////////////////////
// Overrided implementation helpers
//////////////////////////////////////////////////////////////////////

CPropPageFrame* CTreePropSheetOffice2003::CreatePageFrame()
{
  return new CPropPageFrameOffice2003;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTreePropSheetOffice2003, CTreePropSheetEx)
	//{{AFX_MSG_MAP(CTreePropSheetOffice2003)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////
// Overridings
/////////////////////////////////////////////////////////////////////

BOOL CTreePropSheetOffice2003::OnInitDialog() 
{
  BOOL bRet = CTreePropSheetEx::OnInitDialog();

  // If in tree mode, update the tree style.
	if( IsTreeViewMode() && GetPageTreeControl() )
  {
    GetPageTreeControl()->ModifyStyle( TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_TRACKSELECT, TVS_FULLROWSELECT|TVS_SHOWSELALWAYS, 0 );
  }

	HICON hIconLarge = AfxGetApp()->LoadIcon(IDI_MAINFRAME);
	HICON hIconSmall = (HICON) :: LoadImage (AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAINFRAME), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	SetIcon(hIconLarge, TRUE);
	SetIcon(hIconSmall, FALSE);

  return bRet;
}


};
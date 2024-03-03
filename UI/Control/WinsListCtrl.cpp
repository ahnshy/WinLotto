// CWinsListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WinsListCtrl.h"

#include "../../Data/WinsNumberManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CWinsListCtrl::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	CWinsItem *pParam1 = (CWinsItem*)lParam1;
	CWinsItem *pParam2 = (CWinsItem*)lParam2;

	if (!pParam1 || !pParam2)
		return 0;

	if (iColumn == 0)
	{
		return(pParam1->GetRound() < pParam2->GetRound() ? -1 : 1);
		//return pParam1->m_strFileName.CompareNoCase(pParam2->m_strFileName);
	}
	else if (iColumn == 1)
	{
		return pParam1->GetDate().CompareNoCase(pParam2->GetDate());
	}
	//else if (iColumn == 2)
	//{
	//	//return(pParam1->m_dwSize < pParam2->m_dwSize ? -1 : 1);
	//}
	//else if (iColumn == 3)
	//{	
	//	//return(pParam1->m_dwLines < pParam2->m_dwLines ? -1 : 1);
	//}
	else
		return 0;
}

COLORREF CWinsListCtrl::OnGetCellBkColor(int nRow, int nColum)
{
	//if (nColum == m_iSortedColumn)
	//{
	//	//return(nRow % 2) == 0 ? RGB(233, 221, 229) : RGB(176, 218, 234);
	//}

	return(nRow % 2) == 0 ? RGB(253, 241, 249) : RGB(196, 238, 254);
	//return(nRow % 2) == 0 ? RGB(241, 244, 247) : RGB(250, 250, 250);
	//return(nRow % 2) == 0 ? RGB(250, 250, 250) : RGB(223, 230, 238);
}
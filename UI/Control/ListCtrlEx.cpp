#include "stdafx.h"
#include <assert.h>
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT( )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx construction/destruction

CListCtrlEx::CListCtrlEx()
{
	m_colorText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_colorBk = ::GetSysColor(COLOR_WINDOW);
	m_colorFocusText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrHBkgnd = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrPercent = ::GetSysColor(COLOR_HOTLIGHT);
	m_clrHPercent = ::GetSysColor(COLOR_BTNFACE);
}

CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int iSavedDC = pDC->SaveDC();             // Save DC state
        
	int iItem = lpDrawItemStruct->itemID;

	CRect rcBound;
	GetItemRect(iItem, rcBound, LVIR_BOUNDS);


	//return(nRow % 2) == 0 ? RGB(253, 241, 249) : RGB(196, 238, 254);

	LVITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE | LVIF_INDENT | LVIF_PARAM;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	(lvi.iItem % 2) == 0 ? m_colorBk = RGB(253, 241, 249) : m_colorBk = RGB(196, 238, 254);

	bool bHighlight = (
		(lvi.state & LVIS_DROPHILITED) ||
		((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS)))
		);

	pDC->FillSolidRect(&rcBound, (bHighlight) ? m_clrHBkgnd : m_colorBk);

	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	for(int nColumn = 0; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcBound.right = rcBound.left + lvc.cx;
		CRect rcItem(rcBound);
		CString sItem = GetItemText(iItem, nColumn);

		rcItem.DeflateRect(2, 1);
		if(sItem.Right(1) == _T('%'))
		{
			pDC->Rectangle(rcItem);
			rcItem.DeflateRect(1, 1);
			CRect rcLeft, rcRight;
			rcLeft = rcRight = rcItem;
			rcLeft.right = rcLeft.left + MulDiv(_tstoi(sItem), rcItem.Width(), 100);
			rcRight.left = rcLeft.right;
			if(bHighlight)
			{
				pDC->FillSolidRect(rcLeft, m_colorBk);
				pDC->FillSolidRect(rcRight, m_colorBk);
				pDC->SetTextColor(m_colorText);
				pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
			}
			else
			{
				pDC->FillSolidRect(rcLeft, m_clrPercent);
				pDC->FillSolidRect(rcRight, m_colorBk);
				CRgn rgn;
				rgn.CreateRectRgnIndirect(rcLeft);
				pDC->SelectClipRgn(&rgn);
				pDC->SetTextColor(m_colorBk);
				pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

				rgn.SetRectRgn(rcRight);
				pDC->SelectClipRgn(&rgn);
				pDC->SetTextColor(m_colorText);
				pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
				pDC->SelectClipRgn(NULL);
			}

		}
		else
		{
			pDC->SetTextColor((bHighlight) ? m_colorFocusText : m_colorText);
			pDC->DrawText(sItem, rcItem, DT_NOCLIP | DT_LEFT | DT_VCENTER);
		}
		rcBound.left = rcBound.right;
	}

	pDC->RestoreDC(iSavedDC);                 // Restore DC.
}

void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 16;
}

void CListCtrlEx::Init()
{
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}

//return(nRow % 2) == 0 ? RGB(253, 241, 249) : RGB(196, 238, 254);

int CListCtrlEx::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	//CWinsItem *pParam1 = (CWinsItem*)lParam1;
	//CWinsItem *pParam2 = (CWinsItem*)lParam2;

	//if (!pParam1 || !pParam2)
		//return 0;

	if (iColumn == 0)
	{
		//return(pParam1->GetRound() < pParam2->GetRound() ? -1 : 1);
		//return pParam1->m_strFileName.CompareNoCase(pParam2->m_strFileName);
	}
	else if (iColumn == 1)
	{
		//return pParam1->GetDate().CompareNoCase(pParam2->GetDate());
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

	return 0;
}
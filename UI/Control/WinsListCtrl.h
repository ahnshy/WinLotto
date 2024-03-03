#pragma once

#include <afxcmn.h>
#include <afxlistctrl.h>

class CWinsListCtrl : public CMFCListCtrl
{
	virtual int OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn);
	virtual COLORREF OnGetCellBkColor(int nRow, int nColum);
};
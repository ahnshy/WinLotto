// PageWinsRoundList.cpp : implementation file
//

#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "PageWinsRoundList.h"
#include "../../Helper/PaserUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageNote property page

IMPLEMENT_DYNCREATE(CPageWinsRoundList, CDialogEx)

	CPageWinsRoundList::CPageWinsRoundList()
	: CDialogEx(CPageWinsRoundList::IDD)
{
	m_pNofityWnd		= NULL;
	m_rtTab.SetRectEmpty();
}

CPageWinsRoundList::~CPageWinsRoundList()
{
}

void CPageWinsRoundList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINS, m_wndList);
	//DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
}

BEGIN_MESSAGE_MAP(CPageWinsRoundList, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WINS, OnItemchangedListRecord)
	ON_WM_CTLCOLOR()
	ON_WM_ENABLE()
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageWinsRoundList message handlers
BOOL CPageWinsRoundList::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	//AddAnchor(IDC_LIST_WINS, TOP_LEFT, BOTTOM_RIGHT);

	Initialize();

	SetBackgroundColor(RGB(255,255, 255));

	return TRUE;
}

void CPageWinsRoundList::Initialize()
{
	CRect rt;
	GetClientRect(&rt);

	m_wndList.ShowWindow(SW_HIDE);

	//SetWindowPos(&wndTop, rt.left, rt.top, m_rtTab.right, m_rtTab.bottom, SWP_SHOWWINDOW);
	//m_wndList.SetWindowPos(&wndTop, rt.left, rt.top, m_rtTab.right, m_rtTab.bottom, SWP_SHOWWINDOW);

	LVCOLUMN item;
	item.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;


	item.fmt=LVCFMT_LEFT;

	CString strBuffer;
	int nIndex = 0;
	double const fWidthRatio = 0.09;
	item.cx=(int)(rt.Width() * fWidthRatio);
	item.pszText = (_T("No."));
	m_wndList.InsertColumn(nIndex++,&item);

	item.cx=(int)(rt.Width() * fWidthRatio*2);
	item.pszText = (_T("Date"));
	m_wndList.InsertColumn(nIndex++,&item);

	for (; nIndex < 8 ; nIndex++)
	{
		item.cx=int(rt.Width() * fWidthRatio);
		strBuffer.Format(_T("%02d"), nIndex-1);
		item.pszText = ((LPTSTR)(LPCTSTR)strBuffer);
		item.iSubItem = nIndex;
		m_wndList.InsertColumn(nIndex,&item);
	}

	item.cx=(int)(rt.Width() * fWidthRatio);
	item.pszText = (_T("Bonus"));
	m_wndList.InsertColumn(++nIndex,&item);

	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	m_pNofityWnd = GetParent();

	//m_strPath = _T("C:\\Temp");
	//AddFiles();

	SetList();
}

void CPageWinsRoundList::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RGB(255,255,255));

	m_wndList.InvalidateRect(NULL);
	m_wndList.RedrawWindow();
}

BOOL CPageWinsRoundList::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) ||
		(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

CMFCListCtrl* CPageWinsRoundList::GetListCtrl()
{
	return &m_wndList;
}

void CPageWinsRoundList::SetList()
{
	CString strBuffer;
	LVITEM item;

	try
	{
		CWinsNumberManager* pManager = CWinsNumberManager::GetInstance();
		if (!pManager)
			return;

		MapRounds& m = pManager->GetRoundMap();
		for (MapRounds::iterator itor = m.begin() ; itor != m.end() ; ++itor)
		{
			int nColumn = 0, nCnt = m_wndList.GetItemCount();

			item.iItem		= nCnt;
			item.iSubItem	= nColumn;
			strBuffer.Format(_T("%d"), itor->first);
			item.pszText	= (LPTSTR)(LPCTSTR)strBuffer;
			item.mask		= LVIF_TEXT;
			if (m_wndList.GetSafeHwnd())
				m_wndList.InsertItem(&item);

			//if (m_bTaskFinish)
			//return;

			item.iSubItem	= ++nColumn;
			item.pszText	= (LPTSTR)(LPCTSTR)itor->second->GetDate();

			if (m_wndList.GetSafeHwnd())
				m_wndList.SetItem(&item);

			//if (m_bTaskFinish)
			//return;

			for (INT32 nIndex = 0 ; nIndex < itor->second->GetNumberCount() ; nIndex++)
			{
				item.iSubItem	= ++nColumn;
				strBuffer.Format(_T("%d"), itor->second->GetWinNumbers(nIndex));
			
				item.pszText	= (LPTSTR)(LPCTSTR)strBuffer;
				if (m_wndList.GetSafeHwnd())
					m_wndList.SetItem(&item);
			}
			
			if (m_wndList.GetSafeHwnd())
				m_wndList.SetItemData(nCnt, (DWORD)itor->second);

			//nCnt++;
		}
	}
	catch (...)
	{
	}
}

void CPageWinsRoundList::OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uChanged == LVIF_STATE && pNMListView->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
	{
		if (!m_pNofityWnd)
			return;

		//m_pNofityWnd->SendMessage(WM_CHANGE_PROJECTLIST, (WPARAM)NULL, (LPARAM)m_wndList.GetItemData(pNMListView->iItem));
	}

	*pResult = 0;
}

HBRUSH CPageWinsRoundList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//pDC->SetBkMode(TRANSPARENT);
	return  CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPageWinsRoundList::MoveAdjustWnd(CRect &rt)
{
	if (!GetSafeHwnd())
		return;

	//MoveTabWindow();
	MoveTabWindow();

	//SetWindowPos(&wndTop, rt.left, rt.top, rt.right, rt.bottom, SWP_SHOWWINDOW);
}

void CPageWinsRoundList::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{	
	MoveTabWindow();
	CDialogEx::OnWindowPosChanged(lpwndpos);
}

void CPageWinsRoundList::MoveTabWindow()
{
	
	//GetClientRect(&rc);

	//rc.CopyRect(m_rtTab);
	//this->ClientToScreen(rc);
	SetWindowPos(&wndTop, m_rtTab.left, m_rtTab.top, m_rtTab.right, m_rtTab.bottom, SWP_SHOWWINDOW);
	//MoveWindow(m_rtTab);

	//m_wndList.SetWindowPos(&wndTop, m_rtTab.left, m_rtTab.top, m_rtTab.right, m_rtTab.bottom, SWP_SHOWWINDOW);
	
	CRect rcList = m_rtTab;
	rcList.left		= rcList.left	- GetSystemMetrics(SM_CXEDGE);
	rcList.top		= rcList.top	- (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXEDGE)/2);
	rcList.bottom	= rcList.bottom	- (GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXEDGE));

	m_wndList.SetWindowPos(&wndTop, rcList.left, rcList.top, rcList.right, rcList.bottom, SWP_SHOWWINDOW);
	m_wndList.RedrawWindow();

	//CString strDebug;
	//strDebug.Format(_T("Tab Parent Windows Pos : %d %d %d %d \n"), m_rtTab.left, m_rtTab.top, m_rtTab.right, m_rtTab.bottom);
	//OutputDebugString(strDebug);
}


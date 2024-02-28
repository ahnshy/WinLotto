// PageWins.cpp : implementation file
//

#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "PageWins.h"
#include "../../Helper/PaserUtil.h"

#include "PageWinsRoundList.h"
#include "PageWinsFrequencyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CFileInfo
{
public:
	CString				m_strPath;
	CString				m_strFileName;
	CString				m_strModifiedTime;
	DWORD				m_dwLines;

	CFileInfo::CFileInfo()
	{
		SetEmpty();
	}

	void CFileInfo::SetEmpty()
	{
		m_strPath.Empty();
		m_strFileName.Empty();
		m_strModifiedTime.Empty();
		m_dwLines = 0;
	}
};

UINT TaskCountLineFunc(LPVOID pParam)
{
	CPageWins* pDlg = NULL;
	pDlg = (CPageWins*)pParam;

	if (pDlg == NULL)
		return -1;

	//pDlg->UpdateStaticText(IDS_MESSAGE_INITIALIZE);
//	pDlg->AddFiles();

//	DWORD dwCount = pDlg->GetTargetFileCount();

//	pDlg->GetProgressCtrl()->SetRange(0, dwCount);
//	pDlg->GetProgressCtrl()->SetStep(1);
//	for (DWORD dwIndex=0 ; dwIndex < dwCount; dwIndex++)
//	{
//		pDlg->GetProgressCtrl()->StepIt();
//		pDlg->UpdateStatus();
//	}

//	pDlg->TaskFinish();
//	pDlg->UpdateStatus();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPageNote property page

IMPLEMENT_DYNCREATE(CPageWins, CResizablePage)

CPageWins::CPageWins()
	: CResizablePage(CPageWins::IDD)
{
	//m_pWins				= NULL;
	m_dwTotalFiles		= 0;
	m_bIncludeSubPath	= TRUE;
	m_pNofityWnd		= NULL;
	m_pThread			= NULL;
	m_strPath.Empty();
	m_strFileExts.Empty();
	m_strStaticMessage.Empty();
	m_arFiles.RemoveAll();

	m_psp.dwFlags &= (~PSP_HASHELP);
	m_psp.dwFlags |= ( PSP_USEHICON );
	HICON hIconTab = AfxGetApp()->LoadIcon( IDI_TELEPHONE );
	m_psp.hIcon = hIconTab;

	m_pCurrent = NULL;
	m_rcTab.SetRectEmpty();
}

CPageWins::~CPageWins()
{
	CleanUp();

	if (m_pThread)
		m_pThread = NULL;

	m_arFileExt.RemoveAll();
	m_arFiles.RemoveAll();
}

void CPageWins::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TAB_FRAME, m_wndTab);
	DDX_Control(pDX, IDC_TAB_WINS, m_wndTab);

	//DDX_Control(pDX, IDC_LIST_WINS, m_wndList);
	//DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
}

BEGIN_MESSAGE_MAP(CPageWins, CResizablePage)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_TAB_WINS, OnItemchangedListRecord)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_WINS, &CPageWins::OnTcnSelchangeTabResult)
	ON_WM_CTLCOLOR()
	ON_WM_ENABLE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageWins message handlers
BOOL CPageWins::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	AddAnchor(IDC_TAB_WINS, TOP_LEFT, BOTTOM_RIGHT);

	Initialize();
	CalcContols();
	InitCtrl();

	//ShowTabWindow(0);

	return TRUE;
}

void CPageWins::Initialize()
{
}

void CPageWins::InitCtrl()
{
	
	int nTotal = (int)m_wndTab.GetItemCount();

	CPageWinsRoundList *pDlg = new CPageWinsRoundList();
	if (!pDlg)
		return;

	//pDlg->SetHasWhiteBackground(true);
	m_arWnd.Add(pDlg);

	pDlg->Create(IDD_PAGE_WINS_LIST, this);
	pDlg->ShowWindow(SW_HIDE);
	pDlg->SetTabRect(m_rcTab);

	m_wndTab.InsertItem(nTotal, _T("Round Win"));

	nTotal = (int)m_wndTab.GetItemCount();
	CPageWinsFrequencyList *pDlg2 = new CPageWinsFrequencyList();
	if (!pDlg2)
		return;

	//pDlg->SetHasWhiteBackground(true);
	m_arWnd.Add(pDlg2);

	pDlg2->Create(IDD_PAGE_WINS_LIST, this);
	pDlg2->ShowWindow(SW_HIDE);
	pDlg2->SetTabRect(m_rcTab);

	m_wndTab.InsertItem(nTotal, _T("Frequency"));
	ShowTabWindow(0);

	//ChangeTabStyle();
	// VS2010
	//if (m_wndTab.GetSafeHwnd())
	//{
	//m_wndTab.EnableActiveTabCloseButton();
	//m_wndTab.EnableInPlaceEdit(true);
	//m_wndTab.EnableTabDocumentsMenu();
	//m_wndTab.SetActiveTab(1);
	//m_wndTab.SetDrawFrame();
	//m_wndTab.SetFlatFrame();
	//}

	//CMFCVisualManagerWindows *pManager = (CMFCVisualManagerWindows*)CMFCVisualManager::GetInstance();
}

void CPageWins::CalcContols()
{
	CRect rc;
	GetClientRect(&rc);

	m_rcTab = rc;

	m_rcTab.left	= m_rcTab.left	+ GetSystemMetrics(SM_CXEDGE);
	m_rcTab.top		= m_rcTab.top	+ (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXEDGE)/2);
	m_rcTab.right	= m_rcTab.right - (GetSystemMetrics(SM_CXEDGE) *2);
	m_rcTab.bottom	= m_rcTab.bottom- (GetSystemMetrics(SM_CXEDGE)+ GetSystemMetrics(SM_CXEDGE)/2);
}

void CPageWins::ShowTabWindow(int nTabIndex)
{
	CDialogEx *pDlg = NULL;
	for (int nIndex = 0; nIndex < m_arWnd.GetCount(); nIndex++)
	{
		pDlg = (CDialogEx*)m_arWnd.GetAt(nIndex);
		if (!pDlg)
			continue;

		if (nIndex == nTabIndex)
		{
			m_pCurrent = pDlg;
			//UpdateResult();
			//m_pCurrent->SetWindowPos(&wndTop, m_rcTab.left, m_rcTab.top, m_rcTab.right, m_rcTab.bottom, SWP_SHOWWINDOW);
			//m_pCurrent->ShowWindowControls(TRUE);
			m_wndTab.SetCurSel(nIndex);
			m_pCurrent->ShowWindow(SW_SHOW);
			//MoveTabWindow();
		}
		else
			pDlg->ShowWindow(FALSE);
	}
}

void CPageWins::ChangeTabStyle() 
{
	/*
	CMFCTabCtrl::Style style;
	int nTab = 0;

	//m_wndTabIcons.EnableWindow ();

	//for (nTab = 0; nTab < m_wndTab.GetTabsNum (); nTab++)
	//{
	//	m_wndTab.SetTabIcon (nTab, nTab);
	//}

	style = CMFCTabCtrl::STYLE_3D_ONENOTE;
	//	style = CMFCTabCtrl::STYLE_3D;

	m_wndTab.ModifyTabStyle (style);

	CMFCTabCtrl::Location location;

	//	location = CMFCTabCtrl::LOCATION_BOTTOM;
	location = CMFCTabCtrl::LOCATION_TOP;
	m_wndTab.SetLocation (location);

	CArray<COLORREF, COLORREF> arColors;
	arColors.Add (RGB (190, 218, 153));
	arColors.Add (RGB (121, 210, 231));
	arColors.Add (RGB (170, 255, 100));
	arColors.Add (RGB (255, 170, 100));
	m_wndTab.EnableAutoColor (TRUE);

	m_wndTab.SetAutoColors (arColors);
	m_wndTab.EnableTabSwap (FALSE);

	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	//m_wndTab.ShowWindow(SW_SHOW);

	//theApp.WriteInt (_T("TabStyle"), m_nTabStyle);
	*/
}


void CPageWins::AddFiles()
{
	FindFiles(m_strPath);
}

DWORD CPageWins::GetTargetFileCount()
{
	return m_dwTotalFiles;
}

void CPageWins::FindFiles(CString strPath)
{
	if (strPath.IsEmpty())
		return;

	PathAddBackslash(strPath.GetBuffer(BUFSIZ));
	strPath.ReleaseBuffer();
	strPath.Append(_T("*.*"));

	CTime ct;
	CFileFind ff;
	CFileInfo *pItem = NULL;

	BOOL bContinue = ff.FindFile(strPath);

	CString strExt, strBuffer;
	while (bContinue)
	{
		bContinue = ff.FindNextFile();

		if (ff.IsDots())
			continue;

		if (m_bIncludeSubPath && ff.IsDirectory())
			FindFiles(ff.GetFilePath());

		pItem = new CFileInfo();
		if (!pItem)
			continue;

		strExt = PathFindExtension(ff.GetFileName());
		//for (int nCnt = 0 ; nCnt < m_arFileExt.GetCount() ; nCnt++)
		{
			//if (m_arFileExt.ElementAt(nCnt).CompareNoCase(strExt))
				//continue;

			pItem->m_strPath		= ff.GetFilePath();
			pItem->m_strFileName	= ff.GetFileName();

			ff.GetLastAccessTime(ct);
			pItem->m_strModifiedTime = ct.Format(_T("%Y-%m-%d"));

			m_wndList.SetItemData(m_dwTotalFiles++, (DWORD_PTR)pItem);
		}
	}
}

void CPageWins::UpdateStatus()
{
	if (m_pThread)
	{
		// to do...
	}
	else
	{
		// to do...
	}
}

void CPageWins::UpdateStaticText(CString strText)
{
	// to do...
}

void CPageWins::UpdateStaticText(UINT uID)
{
	// to do...
}

BOOL CPageWins::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) ||
		(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN))
		return TRUE;

	return CResizablePage::PreTranslateMessage(pMsg);
}

CMFCListCtrl* CPageWins::GetListCtrl()
{
	return &m_wndList;
}

CProgressCtrl*	CPageWins::GetProgressCtrl()
{
	return &m_wndProgress;
}

BOOL CPageWins::TaskCountLine(CString strPath)
{
	if (!strPath.IsEmpty())
		m_strPath = strPath;

	if (m_strPath.IsEmpty())
		return FALSE;

	if (m_pThread)
		m_pThread = NULL;

	m_pThread = (CWinThread*)AfxBeginThread(TaskCountLineFunc, (LPVOID)this);
	return TRUE;
}

void CPageWins::ShowResult()
{
	// to do...
}

BOOL CPageWins::ParseFilter()
{
	if (m_strFileExts.IsEmpty())
		return FALSE;

	CPaserUtil::ParseString(m_strFileExts, _T(";"), m_arFileExt);

	for (int nCnt = 0; nCnt < (int)m_arFileExt.GetSize() ; nCnt++)
		m_arFileExt.ElementAt(nCnt).Insert(0, _T("."));

	return TRUE;
}

void CPageWins::SetList()
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

//void CPageWins::OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	if (pNMListView->uChanged == LVIF_STATE && pNMListView->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
//	{
//		ShowTabWindow(pNMListView->iItem);
//		if (!m_pNofityWnd)
//			return;
//		
//		//m_pNofityWnd->SendMessage(WM_CHANGE_PROJECTLIST, (WPARAM)NULL, (LPARAM)m_wndList.GetItemData(pNMListView->iItem));
//	}
//
//	*pResult = 0;
//}

HBRUSH CPageWins::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//if (GetDlgItem(IDC_TAB_WINS)->GetSafeHwnd())
		//return (HBRUSH)GetStockObject(NULL_BRUSH);

	if( HasWhiteBackground() )
	{
		pDC->SetBkMode(TRANSPARENT);
		return ::GetSysColorBrush( COLOR_WINDOW );
	}

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPageWins::OnEnable(BOOL bEnable) 
{
	CResizablePage::OnEnable(bEnable);
}

void CPageWins::CleanUp()
{
	//m_pCurrent = NULL;

	CPageWinsRoundList *pDlg = NULL;
	for (int nCnt = 0 ; nCnt < (int)m_arWnd.GetCount() ; nCnt++)
	{
		pDlg = (CPageWinsRoundList*)m_arWnd.GetAt(nCnt);
		if (!pDlg)
			continue;

		delete pDlg;
		pDlg = NULL;
	}

	m_arWnd.RemoveAll();

	//CConfigManager* pConfig = CConfigManager::GetInstance();
	//if (!pConfig)
		//return;

	//pConfig->DestroyInstance();
}

void CPageWins::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	GetClientRect(m_rcTab);

	CResizablePage::OnSize(nType, cx, cy);
}

void CPageWins::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{	
	MoveTabWindow();
	CResizablePage::OnWindowPosChanged(lpwndpos);
}

void CPageWins::MoveTabWindow()
{
	CDialogEx* pDlg = NULL;
	for (int nIndex = 0; nIndex < m_arWnd.GetCount(); nIndex++)
	{
		pDlg = (CDialogEx*)m_arWnd.GetAt(nIndex);
		if (!pDlg)
			continue;

		if (m_pCurrent == pDlg)
		{
			CalcContols();
			//CRect rc, rcBuffer;
			//m_wndTab.GetClientRect(&rc);
			//m_wndTab.GetWindowRect(&rcBuffer);


			//rc.left		= rcBuffer.left;
			//rc.top		= rcBuffer.top + 21;
			//rc.right	= rc.right;
			//rc.bottom	= rc.bottom	 - 21;
		
			if (nIndex == 0)
			{
				((CPageWinsRoundList*)m_pCurrent)->SetTabRect(m_rcTab);
				((CPageWinsRoundList*)m_pCurrent)->MoveAdjustWnd(m_rcTab);
			}
			else if (nIndex == 1)
			{
				((CPageWinsFrequencyList*)m_pCurrent)->SetTabRect(m_rcTab);
				((CPageWinsFrequencyList*)m_pCurrent)->MoveAdjustWnd(m_rcTab);
			}
		}
	}
}

void CPageWins::OnTcnSelchangeTabResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_wndTab.GetCurSel();
	ShowTabWindow(nIndex);
	*pResult = 0;
}
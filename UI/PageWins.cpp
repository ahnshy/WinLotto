// PageWins.cpp : implementation file
//

#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "PageWins.h"
#include "../../Helper/PaserUtil.h"

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
	pDlg->AddFiles();

	DWORD dwCount = pDlg->GetTargetFileCount();

	pDlg->GetProgressCtrl()->SetRange(0, dwCount);
	pDlg->GetProgressCtrl()->SetStep(1);
	for (DWORD dwIndex=0 ; dwIndex < dwCount; dwIndex++)
	{
		pDlg->GetProgressCtrl()->StepIt();
		pDlg->UpdateStatus();
	}

	pDlg->TaskFinish();
	pDlg->UpdateStatus();
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
}

CPageWins::~CPageWins()
{
	if (m_pThread)
		m_pThread = NULL;

	m_arFileExt.RemoveAll();
	m_arFiles.RemoveAll();
}

void CPageWins::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINS, m_wndList);
	DDX_Control(pDX, IDC_TAB_WINS, m_wndTab);
	//DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
}

BEGIN_MESSAGE_MAP(CPageWins, CResizablePage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WINS, OnItemchangedListRecord)
	ON_WM_CTLCOLOR()
	ON_WM_ENABLE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageWins message handlers
BOOL CPageWins::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	AddAnchor(IDC_LIST_WINS, TOP_LEFT, BOTTOM_RIGHT);

	Initialize();

	// VS2010
	if (m_wndTab.GetSafeHwnd())
	{
		m_wndTab.EnableActiveTabCloseButton();
		//m_wndTab.EnableInPlaceEdit(true);
		m_wndTab.EnableTabDocumentsMenu();
		m_wndTab.SetActiveTab(1);
		m_wndTab.SetDrawFrame();
		m_wndTab.SetFlatFrame();
	}

	return TRUE;
}

void CPageWins::Initialize()
{
	CRect rt;
	GetClientRect(&rt);

	//m_wndList.MoveWindow(&rt);
	//m_wndList.SetWindowPos(&wndTop, rt.left, rt.top, rt.right, rt.bottom, SWP_SHOWWINDOW);

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

	m_arFiles.RemoveAll();

	m_pNofityWnd = GetParent();

	//m_strPath = _T("C:\\Temp");
	//AddFiles();

	SetList();
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

void CPageWins::OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult)
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
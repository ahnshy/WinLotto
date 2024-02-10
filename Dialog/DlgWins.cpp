// DlgWins.cpp : implementation file
//

#include "stdafx.h"

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "DlgWins.h"
#include "../Helper/PaserUtil.h"


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
	CDlgWins* pDlg = NULL;
	pDlg = (CDlgWins*)pParam;

	if (pDlg == NULL)
		return -1;

	//pDlg->UpdateStaticText(IDS_MESSAGE_INITIALIZE);
	pDlg->AddFiles();

	DWORD dwCount = pDlg->GetTargetFileCount();

	pDlg->GetProgressCtrl()->SetRange(0, dwCount);
	pDlg->GetProgressCtrl()->SetStep(1);
	for (int nIndex=0 ; nIndex < dwCount; nIndex++)
	{
		pDlg->GetProgressCtrl()->StepIt();
		pDlg->UpdateStatus();
	}

	pDlg->TaskFinish();
	pDlg->UpdateStatus();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgWins dialog
CDlgWins::CDlgWins(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWins::IDD, pParent)
{
	m_dwTotalFiles		= 0;
	m_bIncludeSubPath	= TRUE;
	m_pNofityWnd		= NULL;
	m_pThread			= NULL;
	m_strPath.Empty();
	m_strFileExts.Empty();
	m_strStaticMessage.Empty();
	m_arFiles.RemoveAll();
}

CDlgWins::~CDlgWins()
{
	if (m_pThread)
		m_pThread = NULL;

	m_arFileExt.RemoveAll();
	m_arFiles.RemoveAll();
}

void CDlgWins::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINS, m_wndList);
	//DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
}

BEGIN_MESSAGE_MAP(CDlgWins, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WINS, OnItemchangedListRecord)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWins message handlers
BOOL CDlgWins::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//SetBackgroundColor(RGB(255,255,255));

	Initialize();
	return TRUE;
}

void CDlgWins::Initialize()
{
	CRect rt;
	GetParent()->GetClientRect(&rt);

	m_wndList.MoveWindow(&rt);
	//m_wndList.SetWindowPos(&wndTop, rt.left, rt.top, rt.right, rt.bottom, SWP_SHOWWINDOW);

	LVCOLUMN item;
	item.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;


	item.fmt=LVCFMT_LEFT;
	//item.cx=300;
	item.cx=rt.Width() * 0.49;
	item.pszText=_T("Column 1");
	item.iSubItem=0;
	m_wndList.InsertColumn(0,&item);

	//item.fmt=LVCFMT_RIGHT;
	//item.cx=120;
	item.cx=rt.Width() * 0.23;
	item.pszText=_T("Column 2");
	item.iSubItem=1;
	m_wndList.InsertColumn(1,&item);

	//item.fmt=LVCFMT_RIGHT;
	//item.cx=80;
	item.cx=rt.Width() * 0.14;
	item.pszText=_T("Column 3");
	item.iSubItem=2;
	m_wndList.InsertColumn(2,&item);

	//item.fmt=LVCFMT_RIGHT;
	//item.cx=60;
	item.cx=rt.Width() * 0.12;
	item.pszText=_T("Column 4");
	item.iSubItem=3;
	m_wndList.InsertColumn(3,&item);

	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	m_arFiles.RemoveAll();

	m_pNofityWnd = GetParent();
}

void CDlgWins::AddFiles()
{
	FindFiles(m_strPath);
}

DWORD CDlgWins::GetTargetFileCount()
{
	return m_dwTotalFiles;
}

void CDlgWins::FindFiles(CString strPath)
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
		for (int nCnt = 0 ; nCnt < m_arFileExt.GetCount() ; nCnt++)
		{
			if (m_arFileExt.ElementAt(nCnt).CompareNoCase(strExt))
				continue;

			pItem->m_strPath		= ff.GetFilePath();
			pItem->m_strFileName	= ff.GetFileName();

			ff.GetLastAccessTime(ct);
			pItem->m_strModifiedTime = ct.Format(_T("%Y-%m-%d"));

			m_wndList.SetItemData(m_dwTotalFiles++, (DWORD_PTR)pItem);
		}
	}
}

void CDlgWins::UpdateStatus()
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

void CDlgWins::UpdateStaticText(CString strText)
{
	// to do...
}

void CDlgWins::UpdateStaticText(UINT uID)
{
	// to do...
}

BOOL CDlgWins::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) ||
		(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

CListCtrl* CDlgWins::GetListCtrl()
{
	return &m_wndList;
}

CProgressCtrl*	CDlgWins::GetProgressCtrl()
{
	return &m_wndProgress;
}

BOOL CDlgWins::TaskCountLine(CString strPath)
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

void CDlgWins::ShowResult()
{
	// to do...
}

BOOL CDlgWins::ParseFilter()
{
	if (m_strFileExts.IsEmpty())
		return FALSE;

	CPaserUtil::ParseString(m_strFileExts, _T(";"), m_arFileExt);

	for (int nCnt = 0; nCnt < (int)m_arFileExt.GetSize() ; nCnt++)
		m_arFileExt.ElementAt(nCnt).Insert(0, _T("."));

	return TRUE;
}

void CDlgWins::OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult)
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
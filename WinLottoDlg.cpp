
// WinLottoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinLotto.h"
#include "WinLottoDlg.h"
#include "afxdialogex.h"

#include <new>

#include "Data/WinsNumberManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWinLottoDlg dialog
CWinLottoDlg::CWinLottoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinLottoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinLottoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinLottoDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWinLottoDlg message handlers
int CWinLottoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyle(0, WS_CLIPCHILDREN); // to avoid flicks of child controls.
	
	VERIFY(m_MPCC.Create(this, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), 3001));
	VERIFY(m_wndOutlookTabCtrl.Create(this, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), 100));
	
	VERIFY(m_wndRoundWins.Create(WS_CHILD | LVS_REPORT, CRect(0, 0, 0, 0), this, 3004));
	VERIFY(m_wndListFrequency.Create(WS_CHILD | LVS_REPORT, CRect(0, 0, 0, 0), this, 3005));


	for (INT32 nIndex = 0; nIndex <= 3; nIndex++)
	{
		VERIFY(m_btnMenus[nIndex].Create(NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndOutlookTabCtrl, 100 + nIndex));
		m_btnMenus[nIndex].m_nFlatStyle = (CMFCButton::FlatStyle)(CMFCButton::BUTTONSTYLE_FLAT | CMFCButton::BUTTONSTYLE_NOBORDERS);
		m_btnMenus[nIndex].SetFaceColor(DEFAULT_BACKGROUND_COLOR, TRUE);
		m_btnMenus[nIndex].SetTextColor(DEFAULT_BACKGROUND_COLOR);
		m_btnMenus[nIndex].SetTextHotColor(DEFAULT_BACKGROUND_COLOR);
		m_btnMenus[nIndex].m_bTransparent = FALSE;
		m_btnMenus[nIndex].EnableWindowsTheming(FALSE);
		m_btnMenus[nIndex].m_bDrawFocus = FALSE;
		m_btnMenus[nIndex].m_nAlignStyle = CMFCButton::ALIGN_CENTER;;
	}

	try
	{
		
		//m_wndRoundWins
		MultiPaneCtrl::Tabs tabs;
		tabs.Add(m_wndOutlookTabCtrl, _T("Menus"), 0);
		tabs.Add(m_wndRoundWins, _T("Wins Round Numbers"), 1);
		tabs.Add(m_wndListFrequency, _T("Frequency Numbers"), 2);
		
		if (!m_MPCC.LoadState(AfxGetApp(), _T("WinLottoLayout"), _T("State"), &tabs, false))
			SetDefaultLayout(tabs);   // create default state.
	}
	catch (std::bad_alloc &)
	{
		return -1;
	}
	// 
	return 0;
}


BOOL CWinLottoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(DEFAULT_BACKGROUND_COLOR);
	// 
	SetTabsPosition();

	CFont font;
	font.CreatePointFont(90, DEFAULT_FONT);


	// OutlookBar Setting
	m_OutlookTabStyle4.Install(&m_wndOutlookTabCtrl);
	m_wndOutlookTabCtrl.SetAbilityManager(this);
	m_wndOutlookTabCtrl.SetNotifyManager(this);

	m_wndOutlookTabCtrl.ShowCaption(false);
	m_wndOutlookTabCtrl.ActivateSplitter(false);
	m_wndOutlookTabCtrl.HideEmptyButtonsArea(true);
	m_wndOutlookTabCtrl.ShowMenuButton(false);
	m_wndOutlookTabCtrl.ShowBorder(false);
	m_wndOutlookTabCtrl.SetLayout(OutlookTabCtrl::Layout4);

	m_wndOutlookTabCtrl.CreateStripesImages(NULL, IDB_STRIPE_NORMAL, IDB_STRIPE_DISABLE, true, 24);
	m_wndOutlookTabCtrl.CreateButtonsImages(NULL, IDB_IMAGES_TAB_NORMAL, IDB_IMAGES_TAB_DISABLE, true, 16);
	m_wndOutlookTabCtrl.SetCaptionFont(&font);
	m_wndOutlookTabCtrl.SetStripesFont(&font);
	m_wndOutlookTabCtrl.SetButtonsFont(&font);

	INT32 nCount = 0;
	INT32 nArrIcon[] = { 3, 1, 2, 3, 4 };
	LPCTSTR lpszMenuText[] = { _T("Round Wins"), _T("Simulation"), _T("Statistics"), _T("Prediction"), };
	for (nCount = 0; nCount < _countof(m_btnMenus); nCount++)
	{
		if (!m_btnMenus[nCount].GetSafeHwnd())
			continue;

		m_wndOutlookTabCtrl.AddItem(m_btnMenus[nCount], lpszMenuText[nCount], nArrIcon[nCount], nArrIcon[nCount]);
		if (nCount == 3 || nCount == 2)
		{
			HANDLE hItemDisable = m_wndOutlookTabCtrl.GetItemHandleByIndex(nCount);   // 'Contacts' item.
			m_wndOutlookTabCtrl.DisableItem(hItemDisable, true);   // just for demonstration the disable item.
		}
	}

	m_wndOutlookTabCtrl.Update();

	// MPCC Settings
	m_wndListFrequency.InsertColumn(0, _T("Frequency Numbers"), LVCFMT_LEFT, 100);
	//m_List1.InsertItem(0, _T("Item 1"));
	//m_List1.InsertItem(1, _T("Item 2"));
	//m_List2.InsertColumn(0, _T("CListCtrl 2"), LVCFMT_LEFT, 100);
	//m_List2.InsertItem(0, _T("Item 1"));
	//m_List2.InsertItem(1, _T("Item 2"));

	m_MPCC.CreateSystemImages(nullptr, IDB_IMAGES_SYSTEM, true, 14);
	m_MPCC.CreateImages(nullptr, IDB_IMAGES_TAB_NORMAL, IDB_IMAGES_TAB_DISABLE, true, 16);

	m_MPCC.SetFontNormal(&font);
	m_MPCC.SetFontSelect(&font);
	m_MPCC.InstallStyle(&m_MPCC.styleVS2010_bars);
	m_MPCC.SetDockingMarkers(MarkersLayoutB(), 21);
	m_MPCC.SetAbilityManager(this);
	m_MPCC.SetNotifyManager(this);
	//m_MPCC.SetCursors(IDC_CURSOR_TAB, IDC_CURSOR_SPLITTER_HORZ, IDC_CURSOR_SPLITTER_VERT, IDC_CURSOR_DRAGOUT_ENABLE, IDC_CURSOR_DRAGOUT_DISABLE);
	m_MPCC.SetCursors(static_cast<UINT>(0), 0, 0, 0, 0);
	m_MPCC.EnableTabRemove(true);
	m_MPCC.EnableTabDrag(true);
	m_MPCC.SetPanesMinSize(CSize(35, 35));

	// Setting
	//m_MPCC.Show
	m_MPCC.ShowButtonsMenu(true);
	m_MPCC.ShowButtonsClose(false);
	m_MPCC.ShowButtonsScroll(true);
	m_MPCC.HideSingleTab(true);
	m_MPCC.Update();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	MoveWindow(CRect(0, 0, 800, 600));
	RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	SetListControl();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinLottoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinLottoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CWinLottoDlg::OnDestroy()
{
	//ahnshy
	return CDialogEx::OnDestroy();
	m_MPCC.SaveState(AfxGetApp(), _T("WinLottoLayout"), _T("State"));
	// 
	//CDialogEx::OnDestroy();
}

void CWinLottoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	SetTabsPosition();
}

void CWinLottoDlg::SetTabsPosition()
{
	CWnd *pBaseWnd = GetDlgItem(IDC_STATIC_CONTENTS);
	if (pBaseWnd)
	{
		CRect rcBase;
		pBaseWnd->GetWindowRect(&rcBase/*out*/);
		ScreenToClient(&rcBase);

		CRect rc;
		GetClientRect(&rc/*out*/);
		rc.DeflateRect(rcBase.left, rcBase.top, rcBase.top, rcBase.top);
		m_MPCC.MoveWindow(&rc);
	}
}

void CWinLottoDlg::SetDefaultLayout(MultiPaneCtrl::Tabs const &tabs)
{
	HPANE h1 = m_MPCC.ConvertPaneToLine(nullptr, true);
	HPANE h2 = m_MPCC.ConvertPaneToLine(h1, false);
	HPANE h3 = m_MPCC.ConvertPaneToLine(h2, true);
	HPANE h4 = m_MPCC.AddPane(h2);
	m_MPCC.AddTab(h3, tabs[0]);
	m_MPCC.AddTab(h4, tabs[1]);
	m_MPCC.AddTab(h4, tabs[2]);
}

/////////////////////////////////////////////////////////////////////////////
//
void CWinLottoDlg::OnSelectionChanged(OutlookTabCtrl *ctrl)
{
	CString strText = ctrl->GetItemText(ctrl->GetSelectedItem());
	if (strText.CompareNoCase(_T("Tasks")) || strText.CompareNoCase(_T("Notes")))
	{
		//text = _T("You selected item: \"") + text + _T('\"');
		//::MessageBox(m_hWnd, text, _T("OutlookTabCtrl::Notify"), MB_OK);
	}
}
/////////////////////////////////////////////////////////////////////////////
//
void CWinLottoDlg::OnRightButtonReleased(OutlookTabCtrl *ctrl, CPoint pt)
{
	//CMenu menu;
	//menu.LoadMenu(IDR_CONTEXT_MENU);
	//CMenu *popup = menu.GetSubMenu(0);
	//// 
	//if (!ctrl->CanVisibleItemPop())
	//	popup->EnableMenuItem(ID_CONTEXT_MENU_POP, MF_BYCOMMAND | MF_GRAYED);
	//if (!ctrl->CanVisibleItemPush())
	//	popup->EnableMenuItem(ID_CONTEXT_MENU_PUSH, MF_BYCOMMAND | MF_GRAYED);
	//// 
	//ctrl->ClientToScreen(&pt);
	//popup->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
}
/////////////////////////////////////////////////////////////////////////////
//
void CWinLottoDlg::OnMenuButtonClicked(OutlookTabCtrl *ctrl, CRect const *rect)
{
	OnRightButtonReleased(ctrl, CPoint(rect->right, rect->top));
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void CWinLottoDlg::SetListControl()
{
	// Initial Header
	CRect rt;
	GetClientRect(&rt);

	//m_wndRoundWins.MoveWindow(&rt);
	//m_wndList.SetWindowPos(&wndTop, rt.left, rt.top, rt.right, rt.bottom, SWP_SHOWWINDOW);

	LVCOLUMN itemColumn;
	itemColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;


	itemColumn.fmt = LVCFMT_LEFT;

	CString strBuffer;
	int nIndex = 0;
	//float const fWidthRatio = 0.11;
	//itemColumn.cx = rt.Width() * fWidthRatio;
	itemColumn.pszText = (_T("No."));
	m_wndRoundWins.InsertColumn(nIndex++, &itemColumn);
	//m_wndRoundWins.SetColumnWidth(nIndex++, LVSCW_AUTOSIZE_USEHEADER);

	for (; nIndex < 8; nIndex++)
	{
		//itemColumn.cx = rt.Width() * fWidthRatio;
		strBuffer.Format(_T("%02d"), nIndex);
		itemColumn.pszText = ((LPTSTR)(LPCTSTR)strBuffer);
		itemColumn.iSubItem = nIndex;
		m_wndRoundWins.InsertColumn(nIndex, &itemColumn);
	}

	m_wndRoundWins.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);


	// Initial item
	LVITEM item;

	try
	{
		CWinsNumberManager* pManager = CWinsNumberManager::GetInstance();
		if (!pManager)
			return;

		MapRounds& m = pManager->GetRoundMap();
		for (MapRounds::const_reverse_iterator itor = m.rbegin(); itor != m.rend(); ++itor)
		{
			int nColumn = 0, nCnt = m_wndRoundWins.GetItemCount();

			item.iItem = nCnt;
			item.iSubItem = nColumn;
			strBuffer.Format(_T("%d"), itor->first);
			item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
			item.mask = LVIF_TEXT;
			if (m_wndRoundWins.GetSafeHwnd())
				m_wndRoundWins.InsertItem(&item);

			//if (m_bTaskFinish)
			//return;

			item.iSubItem = ++nColumn;
			item.pszText = (LPTSTR)(LPCTSTR)itor->second->GetDate();

			if (m_wndRoundWins.GetSafeHwnd())
				m_wndRoundWins.SetItem(&item);

			//if (m_bTaskFinish)
			//return;

			for (INT32 nIndex = 0; nIndex < itor->second->GetNumberCount(); nIndex++)
			{
				item.iSubItem = ++nColumn;
				strBuffer.Format(_T("%d"), itor->second->GetWinNumbers(nIndex));

				item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
				if (m_wndRoundWins.GetSafeHwnd())
					m_wndRoundWins.SetItem(&item);
			}

			if (m_wndRoundWins.GetSafeHwnd())
				m_wndRoundWins.SetItemData(nCnt, (DWORD)itor->second);

			//nCnt++;
		}

		for (int i = 0; i < m_wndRoundWins.GetHeaderCtrl().GetItemCount(); i++)
			m_wndRoundWins.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
	catch (...)
	{
	}
}
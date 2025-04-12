#include "stdafx.h"
#include <CommCtrl.h>
#include <vector>
#include <algorithm>
#include "LottoNumberDlg.h"
#include "../../resource.h"

#include "../../Data/WinsNumberManager.h"

BEGIN_MESSAGE_MAP(CLottoNumberDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(1003, &CLottoNumberDlg::OnBnClickedRemoveAll)
	ON_BN_CLICKED(1004, &CLottoNumberDlg::OnBnClickedExtract)
	ON_NOTIFY(LVN_ITEMCHANGED, 1001, &CLottoNumberDlg::OnSelChangeRandomList)
END_MESSAGE_MAP()

CLottoNumberDlg::CLottoNumberDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_EMPTY, pParent)
	, m_pExtractCtrl(nullptr)
	, m_pResultCtrl(nullptr)
	, m_bDragging(FALSE)
	, m_nSplitPos(187)
{
	m_rcSplitBar.SetRectEmpty();
}

CLottoNumberDlg::~CLottoNumberDlg()
{
	if (m_fontButton.GetSafeHandle())
		m_fontButton.DeleteObject();
}

BOOL CLottoNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitExtractList();
	InitResultList();

	VERIFY(m_fontButton.CreateFont(-12, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, DEFAULT_FONT));

	CString strText;
	// https://msdn.microsoft.com/ko-kr/library/bb983153.aspx
	// https://technet.microsoft.com/pl-pl/library/y0k9f0a4(v=vs.90).aspx

	m_btnRemoveAll.Create(_T("Clear"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1003);
	m_btnRemoveAll.m_nFlatStyle = (CMFCButton::FlatStyle)(CMFCButton::BUTTONSTYLE_FLAT | CMFCButton::BUTTONSTYLE_NOBORDERS);
	m_btnRemoveAll.SetFaceColor(RGB(46, 64, 94), TRUE);

	m_btnRemoveAll.SetFont(&m_fontButton);
	m_btnRemoveAll.SetTextColor(RGB(255, 255, 255));
	m_btnRemoveAll.SetTextHotColor(RGB(255, 114, 114));

	m_btnRemoveAll.m_bTransparent = FALSE;
	m_btnRemoveAll.EnableWindowsTheming(FALSE);
	m_btnRemoveAll.m_bDrawFocus = FALSE;
	m_btnRemoveAll.m_nAlignStyle = CMFCButton::ALIGN_CENTER;
	m_btnRemoveAll.SetMouseCursorHand();
	
	m_btnExtract.Create(_T("Extract"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1004);
	m_btnExtract.m_nFlatStyle = (CMFCButton::FlatStyle)(CMFCButton::BUTTONSTYLE_FLAT | CMFCButton::BUTTONSTYLE_NOBORDERS);
	m_btnExtract.SetFaceColor(RGB(46, 64, 94), TRUE);

	m_btnExtract.SetFont(&m_fontButton);
	m_btnExtract.SetTextColor(RGB(255, 255, 255));
	m_btnExtract.SetTextHotColor(RGB(105, 200, 242));

	m_btnExtract.m_bTransparent = FALSE;
	m_btnExtract.EnableWindowsTheming(FALSE);
	m_btnExtract.m_bDrawFocus = FALSE;
	m_btnExtract.m_nAlignStyle = CMFCButton::ALIGN_CENTER;
	m_btnExtract.SetMouseCursorHand();

	return TRUE;
}

void CLottoNumberDlg::InitExtractList()
{
	m_pExtractCtrl = new CLottoListCtrl();
	CRect rcDummy(0, 0, 100, 100);

	m_pExtractCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1001);
	m_pExtractCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_pExtractCtrl->InitializeColumns();

	m_pExtractCtrl->InsertColumn(0, _T("Numbers"), LVCFMT_CENTER, 150);
	m_pExtractCtrl->InsertColumn(1, _T("Bonus"), LVCFMT_CENTER, 30);
}

void CLottoNumberDlg::InitResultList()
{
	m_pResultCtrl = new CLottoListCtrl();
	CRect rcDummy(0, 0, 100, 100);

	m_pResultCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1002);
	m_pResultCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_pResultCtrl->InitializeColumns();
	m_pResultCtrl->SetDrawSelected(FALSE);

	m_pResultCtrl->InsertColumn(0, _T("Rank"), LVCFMT_CENTER, 30);
	m_pResultCtrl->InsertColumn(1, _T("Round"), LVCFMT_RIGHT, 40);
	m_pResultCtrl->InsertColumn(2, _T("Date"), LVCFMT_CENTER, 75);
	m_pResultCtrl->InsertColumn(3, _T("Numbers"), LVCFMT_CENTER, 150);
	m_pResultCtrl->InsertColumn(4, _T("Bonus"), LVCFMT_CENTER, 30);
}

void CLottoNumberDlg::OnPaint()
{
	CPaintDC dc(this);

	COLORREF bgBarColor = RGB(46, 64, 94);
	CBrush bgBrush(bgBarColor);
	dc.FillRect(m_rcSplitBar, &bgBrush);

	int midX = (m_rcSplitBar.left + m_rcSplitBar.right) / 2;
	dc.FillSolidRect(midX - 1, m_rcSplitBar.top, 2, m_rcSplitBar.Height(), bgBarColor);

	//dc.DrawEdge(&m_rcSplitBar, EDGE_ETCHED, BF_RECT);
}


void CLottoNumberDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (::IsWindow(m_btnRemoveAll.GetSafeHwnd()))
		m_btnRemoveAll.MoveWindow(10, 10, 80, 25);

	if (::IsWindow(m_btnExtract.GetSafeHwnd()))
		m_btnExtract.MoveWindow(cx - 90, 10, 80, 25);

	int topMargin = 45;
	int splitBarWidth = 4;

	if (m_nSplitPos < 100) m_nSplitPos = 100;
	if (m_nSplitPos > cx - 100) m_nSplitPos = cx - 100;

	m_rcSplitBar.SetRect(m_nSplitPos - splitBarWidth / 2, topMargin, m_nSplitPos + splitBarWidth / 2, cy - 10);

	int leftMargin = 3;
	int rightMargin = 3;
	int gapBetween = 2;

	if (m_pExtractCtrl && ::IsWindow(m_pExtractCtrl->GetSafeHwnd()))
		m_pExtractCtrl->MoveWindow(leftMargin, topMargin, m_nSplitPos - gapBetween - leftMargin, cy - topMargin - 10);

	if (m_pResultCtrl && ::IsWindow(m_pResultCtrl->GetSafeHwnd()))
		m_pResultCtrl->MoveWindow(m_nSplitPos + gapBetween, topMargin, cx - m_nSplitPos - rightMargin - gapBetween, cy - topMargin - 10);

	CRect rt;
	GetClientRect(&rt);
	rt.top -= topMargin;
	InvalidateRect(rt, FALSE);
}

void CLottoNumberDlg::OnDestroy()
{
	if (m_pExtractCtrl)
	{
		if (::IsWindow(m_pExtractCtrl->GetSafeHwnd()))
			m_pExtractCtrl->DestroyWindow();
		
		delete m_pExtractCtrl;
		m_pExtractCtrl = nullptr;
	}

	if (m_pResultCtrl)
	{
		if (::IsWindow(m_pResultCtrl->GetSafeHwnd()))
			m_pResultCtrl->DestroyWindow();

		delete m_pResultCtrl;
		m_pResultCtrl = nullptr;
	}

	CDialogEx::OnDestroy();
}

void CLottoNumberDlg::OnBnClickedRemoveAll()
{
	if (m_pExtractCtrl)
		m_pExtractCtrl->DeleteAllItems();

	if (m_pResultCtrl)
		m_pResultCtrl->DeleteAllItems();
}

void CLottoNumberDlg::OnBnClickedExtract()
{
	std::vector<int> numbers;
	while (numbers.size() < 7)
	{
		int num = rand() % 45 + 1;
		if (std::find(numbers.begin(), numbers.end(), num) == numbers.end())
			numbers.push_back(num);
	}

	std::sort(numbers.begin(), numbers.begin() + 6);

	CString mainNums, bonusNum;
	for (int i = 0; i < 6; ++i)
	{
		CString numStr;
		numStr.Format(_T("%d"), numbers[i]);
		mainNums += numStr;
		if (i != 5) mainNums += _T(",");
	}

	bonusNum.Format(_T("%d"), numbers[6]);

	int nItem = m_pExtractCtrl->GetItemCount();
	m_pExtractCtrl->InsertLottoRow(nItem, mainNums, bonusNum);
}

void CLottoNumberDlg::OnSelChangeRandomList(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelected = m_pExtractCtrl->GetNextItem(-1, LVNI_SELECTED);
	if (nSelected != -1)
		UpdateResultList(nSelected);

	*pResult = 0;
}

BOOL CLottoNumberDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (m_rcSplitBar.PtInRect(pt))
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		return TRUE;
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CLottoNumberDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcSplitBar.PtInRect(point))
	{
		m_bDragging = TRUE;
		SetCapture();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CLottoNumberDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		CRect rc;
		GetClientRect(&rc);
		int cx = rc.Width();
		m_nSplitPos = max(100, min(point.x, cx - 100));

		Invalidate(FALSE);
		OnSize(0, cx, rc.Height());
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CLottoNumberDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		m_bDragging = FALSE;
		ReleaseCapture();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CLottoNumberDlg::UpdateResultList(int nSelectIndex)
{
	if (!m_pResultCtrl || !m_pExtractCtrl) return;

	m_pResultCtrl->DeleteAllItems();

	CString strNumbers = m_pExtractCtrl->GetItemText(nSelectIndex, 0);
	CString strBonus = m_pExtractCtrl->GetItemText(nSelectIndex, 1);

	std::vector<int> selectedNums;
	{
		int curPos = 0;
		CString token;
		while (!(token = strNumbers.Tokenize(_T(","), curPos)).IsEmpty())
			selectedNums.push_back(_ttoi(token));
	}
	const int bonusNum = _ttoi(strBonus);

	auto* pManager = CWinsNumberManager::GetInstance();
	auto& mapRounds = pManager->GetRoundMap();

	for (const auto& roundPair : mapRounds)
	{
		auto* pItem = roundPair.second;
		if (!pItem) continue;

		std::vector<int> winNums;
		for (int i = 0; i < 6; ++i)
			winNums.push_back(pItem->GetWinNumbers(i));
		const int winBonus = pItem->GetWinNumbers(6);

		std::vector<int> matchedNums;
		for (const auto& num : selectedNums)
		{
			if (std::find(winNums.begin(), winNums.end(), num) != winNums.end())
				matchedNums.push_back(num);
		}

		const bool bonusMatch = (bonusNum == winBonus);

		CString strRank;
		if (matchedNums.size() == 6) strRank = _T("1st");
		else if (matchedNums.size() == 5 && bonusMatch) strRank = _T("2nd");
		else if (matchedNums.size() == 5) strRank = _T("3rd");
		else if (matchedNums.size() == 4) strRank = _T("4th");
		else if (matchedNums.size() == 3) strRank = _T("5th");

		CString strMatchedCSV;
		for (size_t i = 0; i < matchedNums.size(); ++i)
		{
			strMatchedCSV.AppendFormat(_T("%d"), matchedNums[i]);
			if (i != matchedNums.size() - 1)
				strMatchedCSV += _T(",");
		}

		if (!strRank.IsEmpty())
		{
			const int nItem = m_pResultCtrl->GetItemCount();
			CString strDate = pItem->GetDate();
			CString strBonusText;
			strBonusText.Format(_T("%d"), winBonus);
			CString strRound;
			strRound.Format(_T("%d"), pItem->GetRound());

			m_pResultCtrl->InsertItem(nItem, strRank);
			m_pResultCtrl->SetItemText(nItem, 1, strRound);
			m_pResultCtrl->SetItemText(nItem, 2, strDate);
			m_pResultCtrl->SetItemText(nItem, 3, strMatchedCSV);
			m_pResultCtrl->SetItemText(nItem, 4, strBonusText);
		}
	}
}
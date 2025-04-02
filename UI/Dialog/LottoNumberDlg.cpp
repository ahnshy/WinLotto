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
	, m_nSplitPos(245)
{
	m_rcSplitBar.SetRectEmpty();
}

CLottoNumberDlg::~CLottoNumberDlg()
{
}

BOOL CLottoNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitExtractList();
	InitResultList();

	m_btnRemoveAll.Create(_T("Remove All"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1003);
	m_btnRemoveAll.SetFaceColor(RGB(255, 255, 255));

	m_btnExtract.Create(_T("Extract"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1004);
	m_btnExtract.SetFaceColor(RGB(255, 255, 255));

	return TRUE;
}

void CLottoNumberDlg::InitExtractList()
{
	m_pExtractCtrl = new CLottoListCtrl();
	CRect rcDummy(0, 0, 100, 100);

	m_pExtractCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1001);
	m_pExtractCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_pExtractCtrl->InitializeColumns();

	//m_pLottoCtrl->InsertColumn(0, _T("No."), LVCFMT_LEFT, 50);
	m_pExtractCtrl->InsertColumn(0, _T("Numbers"), LVCFMT_CENTER, 190);
	m_pExtractCtrl->InsertColumn(1, _T("Bonus"), LVCFMT_CENTER, 40);
}

void CLottoNumberDlg::InitResultList()
{
	m_pResultCtrl = new CLottoListCtrl();
	CRect rcDummy(0, 0, 100, 100);

	m_pResultCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1002);
	m_pResultCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_pResultCtrl->InitializeColumns();

	//m_pResultCtrl->InsertColumn(0, _T("Round"), LVCFMT_CENTER, 60);
	m_pResultCtrl->InsertColumn(0, _T("Numbers"), LVCFMT_CENTER, 180);
	m_pResultCtrl->InsertColumn(1, _T("Bonus"), LVCFMT_CENTER, 40);
	m_pResultCtrl->InsertColumn(2, _T("Rank"), LVCFMT_CENTER, 40);
	m_pResultCtrl->InsertColumn(3, _T("Date"), LVCFMT_CENTER, 100);

	//m_pResultCtrl->InsertColumn(0, _T("No."), LVCFMT_CENTER, 45);
	//m_pResultCtrl->InsertColumn(1, _T("Numbers"), LVCFMT_CENTER, 190);
	//m_pResultCtrl->InsertColumn(2, _T("Date"), LVCFMT_CENTER, 100);
	//m_pResultCtrl->InsertColumn(3, _T("Rank"), LVCFMT_CENTER, 35);
}

void CLottoNumberDlg::OnPaint()
{
	CPaintDC dc(this);

	//CBrush brush(RGB(150, 150, 150));
	//dc.FillRect(m_rcSplitBar, &brush);

	CBrush brush(RGB(92, 92, 92));
	dc.FillRect(m_rcSplitBar, &brush);

	dc.Draw3dRect(m_rcSplitBar, RGB(120, 120, 120), RGB(220, 220, 220));
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

	if (m_pExtractCtrl && ::IsWindow(m_pExtractCtrl->GetSafeHwnd()))
		m_pExtractCtrl->MoveWindow(10, topMargin, m_nSplitPos - 12, cy - topMargin - 10);

	if (m_pResultCtrl && ::IsWindow(m_pResultCtrl->GetSafeHwnd()))
		m_pResultCtrl->MoveWindow(m_nSplitPos + 4, topMargin, cx - m_nSplitPos - 14, cy - topMargin - 10);

	Invalidate(FALSE);
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
			m_pResultCtrl->InsertLottoRow(nItem, strMatchedCSV, strBonusText, strRank, strDate);
		}
	}
}
#include "stdafx.h"
#include <CommCtrl.h>
#include <vector>
#include <algorithm>
#include "LottoNumberDlg.h"
#include "../../resource.h"

BEGIN_MESSAGE_MAP(CLottoNumberDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(1002, &CLottoNumberDlg::OnBnClickedExtract) // 버튼 핸들러 연결
END_MESSAGE_MAP()

CLottoNumberDlg::CLottoNumberDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_EMPTY, pParent)
	, m_pLottoCtrl(nullptr)
{
}

CLottoNumberDlg::~CLottoNumberDlg()
{
}

BOOL CLottoNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pLottoCtrl = new CLottoListCtrl();
	CRect rcDummy(0, 0, 100, 100);
	m_pLottoCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1001);
	m_pLottoCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_pLottoCtrl->InitializeColumns();

	m_pLottoCtrl->InsertColumn(0, _T("No."), LVCFMT_LEFT, 50);
	m_pLottoCtrl->InsertColumn(1, _T("Numbers"), LVCFMT_LEFT, 400);
	m_pLottoCtrl->InsertColumn(2, _T("Bonus"), LVCFMT_LEFT, 50);




	//m_pLottoCtrl->InsertLottoRow(0, _T("11,16,19,21,27,31"), _T("30"));
	//m_pLottoCtrl->InsertLottoRow(1, _T("2,6,11,16,25,31"), _T("3"));
	//m_pLottoCtrl->InsertLottoRow(2, _T("5,7,8,12,28,33"), _T("7"));

	m_btnExtract.Create(_T("Extract"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1002);
	m_btnExtract.SetFaceColor(RGB(255, 255, 255));

	return TRUE;
}

void CLottoNumberDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_pLottoCtrl && ::IsWindow(m_pLottoCtrl->GetSafeHwnd()))
	{
		m_pLottoCtrl->MoveWindow(10, 50, cx - 20, cy - 60);
	}

	// 버튼 오른쪽 상단 고정
	if (::IsWindow(m_btnExtract.GetSafeHwnd()))
	{
		m_btnExtract.MoveWindow(cx - 90, 10, 80, 30);
	}
}

void CLottoNumberDlg::OnDestroy()
{
	if (m_pLottoCtrl)
	{
		if (::IsWindow(m_pLottoCtrl->GetSafeHwnd()))
			m_pLottoCtrl->DestroyWindow();
		delete m_pLottoCtrl;
		m_pLottoCtrl = nullptr;
	}

	CDialogEx::OnDestroy();
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

	int nItem = m_pLottoCtrl->GetItemCount();
	m_pLottoCtrl->InsertLottoRow(nItem, mainNums, bonusNum);
}

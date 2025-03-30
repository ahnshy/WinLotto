#include "stdafx.h"
#include <CommCtrl.h>
#include <vector>
#include <algorithm>
#include "LottoNumberDlg.h"
#include "../../resource.h"

BEGIN_MESSAGE_MAP(CLottoNumberDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(1002, &CLottoNumberDlg::OnBnClickedRemoveAll)
	ON_BN_CLICKED(1003, &CLottoNumberDlg::OnBnClickedExtract)
END_MESSAGE_MAP()

CLottoNumberDlg::CLottoNumberDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_EMPTY, pParent)
	, m_pExtractCtrl(nullptr)
	, m_pResultCtrl(nullptr)
{
}

CLottoNumberDlg::~CLottoNumberDlg()
{
}

BOOL CLottoNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pExtractCtrl = new CLottoListCtrl();
	CRect rcDummy(0, 0, 100, 100);
	m_pExtractCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1001);
	m_pExtractCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_pExtractCtrl->InitializeColumns();

	//m_pLottoCtrl->InsertColumn(0, _T("No."), LVCFMT_LEFT, 50);
	m_pExtractCtrl->InsertColumn(0, _T("Numbers"), LVCFMT_LEFT, 200);
	m_pExtractCtrl->InsertColumn(1, _T("Bonus"), LVCFMT_LEFT, 40);

	m_btnRemoveAll.Create(_T("Remove All"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1002);
	m_btnRemoveAll.SetFaceColor(RGB(255, 255, 255));

	m_btnExtract.Create(_T("Extract"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 80, 30), this, 1003);
	m_btnExtract.SetFaceColor(RGB(255, 255, 255));

	return TRUE;
}

void CLottoNumberDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_pExtractCtrl && ::IsWindow(m_pExtractCtrl->GetSafeHwnd()))
		m_pExtractCtrl->MoveWindow(10, 50, cx - 20, cy - 60);

	if (::IsWindow(m_btnExtract.GetSafeHwnd()))
		m_btnRemoveAll.MoveWindow(10, 10, 80, 25);

	if (::IsWindow(m_btnExtract.GetSafeHwnd()))
		m_btnExtract.MoveWindow(cx - 90, 10, 80, 25);
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

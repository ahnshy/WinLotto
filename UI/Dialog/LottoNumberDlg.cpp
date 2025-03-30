#include "stdafx.h"
#include <CommCtrl.h>
#include "LottoNumberDlg.h"
#include "../../resource.h"

BEGIN_MESSAGE_MAP(CLottoNumberDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_DESTROY()
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
	/*
    //m_pLottoCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED, rcDummy, this, 001);
	m_pLottoCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, rcDummy, this, 1001);

    //m_pLottoCtrl->InsertColumn(0, _T("Number"), LVCFMT_LEFT, 50);
    //for (int i = 0; i < 50; ++i)
    //{
    //    CString str;
    //    str.Format(_T("%d"), i + 1);
    //    m_pLottoCtrl->InsertItem(i, str);
    //}

	m_pLottoCtrl->InsertColumn(0, _T("Number"), LVCFMT_CENTER, 50);
	m_pLottoCtrl->InsertColumn(1, _T("Value"), LVCFMT_CENTER, 100);

	for (int i = 0; i < 50; ++i)
	{
		CString numStr, valueStr;
		numStr.Format(_T("%d"), i + 1);
		valueStr.Format(_T("%02d"), i + 1);

		int index = m_pLottoCtrl->InsertItem(i, numStr);

		m_pLottoCtrl->SetItemText(index, 1, valueStr);
	}
	*/

	//m_pLottoCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED, rcDummy, this, 1001);
	m_pLottoCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, rcDummy, this, 1001);
	m_pLottoCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);


	//m_font.CreatePointFont(180, DEFAULT_FONT); // 약 12~14pt짜리 더미폰트
	//m_pLottoCtrl->SetFont(&m_font);

	m_pLottoCtrl->InsertColumn(0, _T("Game"), LVCFMT_LEFT, 50);
	m_pLottoCtrl->InsertColumn(1, _T("Numbers"), LVCFMT_LEFT, 400);
	m_pLottoCtrl->InsertColumn(2, _T("Bonus"), LVCFMT_LEFT, 50);

	/*
	for (int i = 0; i < 50; ++i)
	{
		CString numStr, valueStr;
		numStr.Format(_T("%d"), i + 1);
		valueStr.Format(_T("%d"), i + 1);

		int index = m_pLottoCtrl->InsertItem(i, numStr);

		m_pLottoCtrl->SetItemText(index, 1, valueStr);
	}

	//m_pLottoCtrl->InsertColumn(0, _T("Numbers"), LVCFMT_LEFT, 50);
	for (int i = 0; i < 50; ++i)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_pLottoCtrl->InsertItem(i, str);
	}
	*/

	std::vector<CLottoListCtrl::LottoData> data = {
		{ { 11, 16, 19, 21, 27, 31 }, 30 },
		{ { 2, 6, 11, 16, 25, 31 }, 3 },
	};
	m_pLottoCtrl->SetData(data);

    return TRUE;
}

void CLottoNumberDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (m_pLottoCtrl && ::IsWindow(m_pLottoCtrl->GetSafeHwnd()))
    {
        m_pLottoCtrl->MoveWindow(10, 10, cx - 20, cy - 20);
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

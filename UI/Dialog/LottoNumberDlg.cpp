#include "stdafx.h"
#include "LottoNumberDlg.h"

BEGIN_MESSAGE_MAP(CLottoNumberDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

CLottoNumberDlg::CLottoNumberDlg(CWnd* pParent)
    : CDialogEx(IDD_MYDIALOG, pParent)
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
    m_pLottoCtrl->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED, rcDummy, this, 1001);

    m_pLottoCtrl->InsertColumn(0, _T("번호"), LVCFMT_LEFT, 50);
    for (int i = 0; i < 50; ++i)
    {
        CString str;
        str.Format(_T("%d"), i + 1);
        m_pLottoCtrl->InsertItem(i, str);
    }

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

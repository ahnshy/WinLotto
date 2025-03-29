#pragma once

#include "LottoListCtrl.h"

class CLottoNumberDlg : public CDialogEx
{
public:
    CLottoNumberDlg(CWnd* pParent = nullptr);
    virtual ~CLottoNumberDlg();

protected:
    CLottoListCtrl* m_pLottoCtrl;

protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();

    DECLARE_MESSAGE_MAP()
};

#pragma once

#include "../Control/LottoListCtrl.h"

class CLottoNumberDlg : public CDialogEx
{
public:
	CLottoNumberDlg(CWnd* pParent = nullptr);
	virtual ~CLottoNumberDlg();

protected:
	CLottoListCtrl* m_pLottoCtrl;
	CMFCButton m_btnExtract;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedExtract();

	DECLARE_MESSAGE_MAP()
};

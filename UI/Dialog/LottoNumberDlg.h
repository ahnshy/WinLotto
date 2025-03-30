#pragma once

#include "../Control/LottoListCtrl.h"

class CLottoNumberDlg : public CDialogEx
{
public:
	CLottoNumberDlg(CWnd* pParent = nullptr);
	virtual ~CLottoNumberDlg();

protected:
	CLottoListCtrl*		m_pExtractCtrl;
	
	CMFCButton			m_btnExtract;
	CMFCButton			m_btnRemoveAll;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRemoveAll();
	afx_msg void OnBnClickedExtract();

	DECLARE_MESSAGE_MAP()
};

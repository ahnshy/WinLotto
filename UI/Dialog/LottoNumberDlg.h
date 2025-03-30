#pragma once

#include "../Control/LottoListCtrl.h"

class CLottoNumberDlg : public CDialogEx
{
public:
	CLottoNumberDlg(CWnd* pParent = nullptr);
	virtual ~CLottoNumberDlg();

protected:
	CLottoListCtrl*		m_pExtractCtrl;
	CLottoListCtrl*		m_pResultCtrl;
	
	CMFCButton			m_btnExtract;
	CMFCButton			m_btnRemoveAll;

	void InitResultList();
	void InitExtractList();

	void UpdateResultList(int nSelectIndex);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRemoveAll();
	afx_msg void OnBnClickedExtract();

	DECLARE_MESSAGE_MAP()
};

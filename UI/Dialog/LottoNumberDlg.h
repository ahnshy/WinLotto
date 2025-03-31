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

	CRect				m_rcSplitBar;
	BOOL				m_bDragging;
	int					m_nSplitPos;

	void InitResultList();
	void InitExtractList();

	void UpdateResultList(int nSelectIndex);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRemoveAll();
	afx_msg void OnBnClickedExtract();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnSelChangeRandomList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

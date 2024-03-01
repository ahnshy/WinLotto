#pragma once

#include <afxcmn.h>
#include <afxlistctrl.h>
#include <afxole.h>

#include <afxdialogex.h>

#include "ResizablePage.h"

#include "../Controls/WinsListCtrl.h"
#include "../PropertySheet/TreePropSheetUtil.hpp"
#include "../../Data/WinsNumberManager.h"

UINT TaskCountLineFunc2(LPVOID pParam);
void AddFiles(CString strPath);


/////////////////////////////////////////////////////////////////////////////
// CPageWinsRoundList dialog
class CPageWinsRoundList : public CDialogEx
{
	DECLARE_DYNCREATE(CPageWinsRoundList)

public:
	CPageWinsRoundList();   // standard constructor
	~CPageWinsRoundList();

	enum			{ IDD = IDD_PAGE_WINS };


	void			SetList();

	void			Initialize();

	CMFCListCtrl*	GetListCtrl();
	void			SetTabRect(CRect &rc)									{			m_rtTab.CopyRect(rc);					}

public:
	void			MoveTabWindow();
	void			MoveAdjustWnd(CRect &rt);

protected:
	//MapWins*		m_pWins;
	//CMFCListCtrl	m_wndList;

	CWinsListCtrl	m_wndList;

	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	afx_msg void	OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg	void	OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg	void	OnPaint();

protected:
	CWnd*			m_pNofityWnd;
	
	CRect			m_rtTab;
	CStringArray	m_arListColumn;

	DECLARE_MESSAGE_MAP()
};

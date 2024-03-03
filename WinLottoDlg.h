
// WinLottoDlg.h : header file
//
#pragma once

#include "UI/Control/WinsListCtrl.h"

#include "UI/Control/MultiPaneCtrl/StyleDefine.h"
#include "UI/Control/MultiPaneCtrl/MultiPaneCtrl.h"
#include "UI/Control/OutlookTabCtrl/OutlookTabCtrl.h"

// CWinLottoDlg dialog
class CWinLottoDlg : public CDialogEx,
	MultiPaneCtrl::Ability,
	MultiPaneCtrl::Notify,
	public OutlookTabCtrl::Ability,
	public OutlookTabCtrl::Notify
{

// Construction
public:
	CWinLottoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WINLOTTO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CMFCButton m_btnMenus[10];
	OutlookTabCtrl m_wndOutlookTabCtrl;
	OutlookTabCtrlCustom1 m_OutlookTabStyle1;
	OutlookTabCtrlCustom2 m_OutlookTabStyle2;
	OutlookTabCtrlCustom3 m_OutlookTabStyle3;
	OutlookTabCtrlCustom4 m_OutlookTabStyle4;

	MultiPaneCtrlComplex m_MPCC;
	typedef MultiPaneCtrl::HPANE HPANE;

	CWinsListCtrl m_wndRoundWins;
	CListCtrl m_wndListFrequency; // to do...

	void SetTabsPosition();
	void SetDefaultLayout(MultiPaneCtrl::Tabs const &tabs);
	void SetListControl();

	void OnSelectionChanged(OutlookTabCtrl *ctrl) override;
	void OnRightButtonReleased(OutlookTabCtrl *ctrl, CPoint pt) override;
	void OnMenuButtonClicked(OutlookTabCtrl *ctrl, CRect const *rect) override;

	// Generated message map functions
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

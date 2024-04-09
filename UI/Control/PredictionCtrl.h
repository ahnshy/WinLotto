/////////////////////////////////////////////////////////////////////////////
// PredictionCtrl.h : header file
//

#pragma once
#include "../../resource.h"
#include "../../Helper/GdiPlusHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CPredictionCtrl

class CPredictionCtrl : public CDialogEx
{
	DECLARE_DYNCREATE(CPredictionCtrl)

// Construction
public:
	CPredictionCtrl();
	~CPredictionCtrl();

// Dialog Data
	//{{AFX_DATA(CPredictionCtrl)
	enum { IDD = IDD_DIALOG_EMPTY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPredictionCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	COLORREF GetRandomColor();
	INT32 GetSimulationWndRect(CRect& rc);

private:
	CGdiPlusHelper m_gdi;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPredictionCtrl)
	virtual BOOL	OnInitDialog();
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnPaint();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// SimulationCtrl.h : header file
//

#pragma once
#include "../../resource.h"
#include "../../Helper/GdiPlusHelper.h"

static const float GRAVITY = 0.5f;   // 중력 가속도
static const float DAMPING = 0.9f;   // 충돌 후 감쇠 계수

/////////////////////////////////////////////////////////////////////////////
// CSimulationCtrl

class CSimulationCtrl : public CDialogEx
{
	DECLARE_DYNCREATE(CSimulationCtrl)

// Construction
public:
	CSimulationCtrl();
	~CSimulationCtrl();

// Dialog Data
	//{{AFX_DATA(CSimulationCtrl)
	enum { IDD = IDD_DIALOG_EMPTY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSimulationCtrl)
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
	//{{AFX_MSG(CSimulationCtrl)
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
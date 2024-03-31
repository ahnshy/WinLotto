/////////////////////////////////////////////////////////////////////////////
// SimulationCtrl.h : header file
//

#pragma once

#include "ResizablePage.h"
#include "../../Helper/GdiPlusHelper.h"
#include "../PropertySheet/TreePropSheetUtil.hpp"

/////////////////////////////////////////////////////////////////////////////
// CSimulationCtrl dialog

class CSimulationCtrl
 : public CWnd,
   public TreePropSheet::CWhiteBackgroundProvider
{
	DECLARE_DYNCREATE(CSimulationCtrl)

// Construction
public:
	CSimulationCtrl();
	~CSimulationCtrl();

// Dialog Data
	//{{AFX_DATA(CSimulationCtrl)
	enum { IDD = IDD_PAGE_EMPTY };
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

private:
	CGdiPlusHelper m_gdi;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSimulationCtrl)
	virtual BOOL	OnInitDialog();
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnPaint();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
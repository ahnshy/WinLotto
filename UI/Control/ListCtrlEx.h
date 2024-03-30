#ifndef _CLISTCTRLEX_H_
#define _CLISTCTRLEX_H_

class CListCtrlEx : public CListCtrl
{
//	typedef CListCtrl inherited;

	DECLARE_DYNCREATE(CListCtrlEx)

// Construction
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFontCombo)     
	public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init();
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;
	COLORREF m_clrHText;
	COLORREF m_clrHBkgnd;
	COLORREF m_clrPercent;
    COLORREF m_clrHPercent;

// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif

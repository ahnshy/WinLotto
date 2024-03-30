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
	public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	virtual int OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn);

	void			SetTextColor(COLORREF color)					{ m_colorText = color; }
	void			SetFocusTextColor(COLORREF color)				{ m_colorFocusText = color; }
	void			SetBkColor(COLORREF color)						{ m_colorBk = color; }


// Implementation
public:
	void Init();
	COLORREF m_colorText;
	COLORREF m_colorBk;
	COLORREF m_colorFocusText;
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

// TreePropSheetSplitter.h
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2004 by Yves Tkaczyk
// (http://www.tkaczyk.net - yves@tkaczyk.net)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// Documentation: http://www.codeproject.com/property/treepropsheetex.asp
// CVS tree:      http://sourceforge.net/projects/treepropsheetex
//
//  /********************************************************************
//  *
//  * This code is an update of SimpleSplitter written by Robert A. T. Kaldy and 
//  * published on code project at http://www.codeproject.com/splitter/kaldysimplesplitter.asp.
//  *
//  *  //  CSimpleSplitter
//  *  //
//  *  //  Splitter window with CWnd-derived panes
//  *  //  (C) Robert A. T. Kaldy <kaldy@matfyz.cz>
//  *  //  last updated on 11.2.2004
//  *
//  *********************************************************************/
//
/////////////////////////////////////////////////////////////////////////////

//
//  CSimpleSplitter
//
//  Splitter window with CWnd-derived panes
//  (C) Robert A. T. Kaldy <kaldy@matfyz.cz>
//  last updated on 11.2.2004


#ifndef _TREEPROPSHEET_TREEPROPSHEETSPLITTER_H__INCLUDED_
#define _TREEPROPSHEET_TREEPROPSHEETSPLITTER_H__INCLUDED_

#define SSP_HORZ		1
#define SSP_VERT		2

namespace TreePropSheet
{

/*! \brief Splitter class to TreePropSheetEx

  @version 0.1 Initial release
  @version 0.2 (08/2004) Added individual minimum pane sizes
  @author Robert A. T. Kaldy (original CSimpleSplitter)
  @author Yves Tkaczyk <yves@tkaczyk.net> 
  @date 07/2004 */
class CTreePropSheetSplitter
 : public CWnd
{
public:
	CTreePropSheetSplitter(const int nPanes, const UINT nOrientation = SSP_HORZ, const int nMinSize = 50, const int nBarThickness = 5);
  CTreePropSheetSplitter(const int nPanes, const UINT nOrientation, const int* pzMinSize, const int nBarThickness);
	~CTreePropSheetSplitter();

	BOOL Create(CWnd* pParent,const CRect& rect,UINT nID = AFX_IDW_PANE_FIRST);
	BOOL Create(CWnd* pParent, UINT nID = AFX_IDW_PANE_FIRST);
	BOOL CreatePane(int nIndex, CWnd* pPaneWnd, DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszClassName = NULL);
	
// Methods
  /*! Returns the number of panes.
    @retval Number of panes as set in constructor. */
   int GetPaneCount() const
  {
    return m_nPanes; 
  }
	
  /*! Associates a winodw with the specified pane.
    @param nIndex Index to pane. Must be smaller than the total number of panes. 
    @param pPaneWnd Window to the registered with the pane. */
  void SetPane(int nIndex, CWnd* pPaneWnd);
	
  /*! Returns the window associated with the specified pane. 
    @retval Window associated with pane. */
  CWnd* GetPane(int nIndex) const;
	
  /*! Activate the specified pane. The focus is given to the associated window. 
    @param nIndex Index to pane to activiate. */
  virtual void SetActivePane(int nIndex);
	
  /*! Returns the current active pane.
  @param nIndex Is set to the current active pane. Must be smaller than the total 
                number of panes.
  @retval Pointer to current active pane or NULL if there is no pane with focus. */
  CWnd* GetActivePane(int& nIndex) const;
	
  /*! Set the size of each pane. 
    @param sizes Array of integer represneting pane size. */
  void SetPaneSizes(const int* sizes);
	
  /*! Returns the pane's rectangle for the specified pane. 
    @param nIndex Pane for which rectangle is queried. Must be smaller than the total
                  number of panes.
    @param rcPane Receive the specified pane rectangle. */
  void GetPaneRect(int nIndex, CRect& rcPane) const;
	
  /*! Returns the rectangle of the specified bar. The bar is the
     portion of the window grabbed by the user for resizing.
    @param nIndex Pane for which rectangle is queried. Must be smaller than the total
                  number of panes - 1.
    @param rcBar Receive the specified bar rectangle. */
  void GetBarRect(int nIndex, CRect& rcBar) const;

  /*! Indicates which panes should be frozen. If possible,
      frozen panes are not resized when the splitter window
      is resized.
    @param frozenPanes Array of boolean. Each bool represents a pane. If the value
                       is true, the pane is frozen. */
  void SetFrozenPanes(const bool* frozenPanes);

  /*! Unfreeze all panes. */ 
  void ResetFrozenPanes();

  /*! Indicates if the splitter should refresh all panes
      when the user is moving a bar. 
    @param bRealtime True to have real-time update. */
  void SetRealtimeUpdate(const bool bRealtime);

  /*! Returns true if the splitter is in real-time mode. 
    @retval bool True if in real-time mode. */
  bool IsRealtimeUpdate() const;

  /*! Indicates if the user can resize the panes. 
    @param bAllowUserResizing True if the user can resize the panes. */ 
  void SetAllowUserResizing(const bool bAllowUserResizing);

  /*! Returns true if user is allowed to resize the panes. 
    @retval bool True if panes are user resizable. */
  bool IsAllowUserResizing() const;

// Overridables
  /*! Update the provided area on the parent window. This can be overriden if 
      necessary. */
  virtual void UpdateParentRect(LPCRECT lpRectUpdate );

protected:
	void RecalcLayout();
	void ResizePanes();
	void InvertTracker();
  void GetAdjustedClientRect(CRect* rRect) const;
	
	//{{AFX_MSG(CTreePropSheetSplitter)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Helpers
protected:
  /*! Returns a handle to the appropriate cursor based on the resizing
      mode and the slitter type (horizontal or vertical).
      The cursor is loaded as a shared resource (LR_SHARED) and does not
      need to be destroyed.
    @return HCURSOR Handle to cursor or NULL if creation failed. */
  HCURSOR GetCursorHandle() const;
private:
  /*! Common part of instance initialization. */
  void CommonInit();

// Members
protected:
	const int m_nPanes;
	const UINT m_nOrientation;
  const int m_nBarThickness;

	int m_nTrackIndex, m_nTracker, m_nTrackerLength, m_nTrackerMouseOffset;
	
	CWnd** m_pane;
	int *m_size, *m_orig;
  /*! Array of minimum pane sizes. */
  int* m_pzMinSize;
  /*! Array of bools describing the frozen panes. */
  bool *m_frozen;
  /*! Number of forzen panes. */
  int m_nFrozenPaneCount;
  /*! Flag indicating if the splitter should refreshed its panes in realtime. */
  bool m_bRealtimeUpdate;
  /*! Flag indicating if the user can resize the panes. */
  bool m_bAllowUserResizing;
};

};  // namespace TreePropSheet

#endif  // _TREEPROPSHEET_TREEPROPSHEETSPLITTER_H__INCLUDED_

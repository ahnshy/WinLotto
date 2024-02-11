// TreePropSheetEx.h
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
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "TreePropSheetBase.h"
#include "TreePropSheetResizableLibHook.h"
#include <memory>

namespace TreePropSheet
{
  //! Splitter class forward declaration.
  class CTreePropSheetSplitter;

  /*! \brief Help context menu mode for the property sheet
  Describes which help context menu should be displayed when the 
  user right-clicks on the different controls embedded in the 
  property sheet. */
  enum enContextMenuMode
  {
    TPS_All,                    //!< Display context menu for all controls (similar to CTreePropSheet).
    TPS_PropertySheetControls,  //!< Display context menu on native property sheet controls (not on the tree or the page frame).  
    TPS_None                    //!< Do not display any help context menu.
  };

  /*! \brief Resizing mode for the tree
  Describes how the tree should be resized when the dialog is resized. */
  enum enTreeSizingMode
  {
    TSM_Fixed,              //!< Tree width is unchanged (if possible).
    TSM_Proportional,       //!< Tree width is changed proportionally to the sheet width.
  };

/*! @brief Extended version of CTreePropSheet

  CTreePropSheetEx is an extension of CTreePropSheet. The class 
  offers the following additional features:
  - Resizable dialog
  - Resizable tree
    - Real-time and classic splitter mode
    - Tree can have a fixed width or resize proportionally to the
      parent window
    - Tree width can be set after the Windows window has been created
  - Option to skip the empty pages
  - Configurable help context menu behavior

  The property sheet is resizable by default. To turn this feature off, call
  <pre>SetIsResizable( false )</pre>.

  @version 0.1 Initial version
  @version 0.2 (08/2004) Added individual minimum sizes for tree and frame
  @author Yves Tkaczyk <yves@tkaczyk.net>
  @date 07/2004 */
class CTreePropSheetEx
  : public CTreePropSheetBase  
{
  DECLARE_DYNCREATE(CTreePropSheetEx)
// Typedef
  //! Splitter class.
  typedef CTreePropSheetSplitter tSplitter;
  //! Managed pointer (auto_ptr) to splitter class.
  typedef std::auto_ptr<tSplitter> tSplitterPtr;
  //! Layout manager.
  typedef CTreePropSheetResizableLibHook tLayoutManager;
  //! Managed pointer (auto_ptr) to layout manager.
  typedef std::auto_ptr<tLayoutManager> tLayoutManagerPtr;

// Construction/Destruction
public:
	CTreePropSheetEx();
	CTreePropSheetEx(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTreePropSheetEx(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTreePropSheetEx();

// Properties

  //@{
  /*! Enable/disable resizing. This has to be called before the sheet is created.
    @param bIsResizable True to have the sheet resizable. 
    @return True if call was successful, false otherwise. */
  bool SetIsResizable(const bool bIsResizable);

  /*! Indicate if the property sheet is resizable.
    @return bool True if resizable, false otherwise. */
  bool IsResizable() const;

  /*! Set the sheet minimum size. 
    @param sizeMin Minimum size for the sheet. 
    @return True if call was successful, false otherwise. */
  void SetMinSize(const CSize& sizeMin);

  /*! Return the sheet minimum size. 
    @return Sheet minimum size or CSize(-1, -1) if no minimum size is defined. */
  CSize GetMinSize() const;

  /*! Sets the sheet maximum size.
    @param sizeMax Maximum size for the sheet. 
    @return True if call was successful, false otherwise. */
  void SetMaxSize(const CSize& sizeMax);

  /*! Returns the sheet maximum size.
    @return Sheet maximum size or CSize(-1, -1) if no minimum size is defined. */
  CSize GetMaxSize() const;

  /*! Set panes (tree or frame) minimum size.
    @param nPaneMinimumSize Pane (tree or frame) minimum width. 
    @return True if call was successful, false otherwise. */
  bool SetPaneMinimumSize(const int nPaneMinimumSize);

  /*! Returns the panes' minimum size. If the minimum sizes have been set with 
      SetPaneMinimumSizes, -1 is returned.
    @return Minimum pane size if the two minimum size are equals, -1 otherwise.
    @deprecated Use GetPaneMinimumSizes instead in order to retrieve the individual
                minimum sizes. */
  int GetPaneMinimumSize() const;
  
  /*! Set the individual minimum pane sizes. This allows to set different sizes 
      for the tree and for the frame. This is useful as the frame is very likely 
      to have a minimum size grater than the tree minimum size. 
    @param nTreeMinimumSize Tree minimum width. 
    @param nFrameMinimumSize Frame minimum width. 
    @return True if call was successful, false otherwise. */
  bool SetPaneMinimumSizes(const int nTreeMinimumSize, const int nFrameMinimumSize);

  /*! Returns the pane minimum sizes. 
    @param nTreeMinimumSize Contains tree minimum width. 
    @param nFrameMinimumSize Contains frame minimum width. */
  void GetPaneMinimumSizes(int& nTreeMinimumSize, int& nFrameMinimumSize) const;
  //@}

  //@{
  /*! Enable/disable if the user can resize the tree.
    @param bIsTreeResizable True to have the tree resizable. */
  void SetTreeIsResizable(const bool bIsTreeResizable);

  /*! Indicate if the user can resize the tree.
    @return True if the tree is resizable, false otherwise. */
  bool IsTreeResizable() const;

  /*! Set the splitter width. Calling this will have 
      no effect if not in tree mode. This method has 
      to be called before the windows are created. 
    @param nSplitterWidth Width of the splitter. If this
           is not called, default width is 5.
    @return True if successful, false otherwise, that 
            is not in tree mode or the windows have 
            already been created. */
  bool SetSplitterWidth(const int nSplitterWidth);

  /*! Returns the width of the splitter. 
    @return Width of the splitter. */
  int GetSplitterWidth() const;

  /*! Set the tree resizing Mode. The options are:
      - Fixed: The tree width does not changed when the sheet width
        changes. This means that the frame control (and the 
        pages inside it) is the only item that is resized when
        the shhet is resized.
      - Proportional: When the sheet is resized, both the tree control
        and the frame control are resized. 
    @param eSizingMode Sizing mode for the tree. */
  void SetTreeResizingMode(const enTreeSizingMode eSizingMode);

  /*! Returns the tree sizing mode. 
    @return Tree sizing mode. */
  enTreeSizingMode GetTreeResizingMode() const;
  //@}

  //@{
  /*! Indicates if the tree and frame should be resized in realtime when
      moving the splitter. 
    @param bRealtime True for realtime update. */
  void SetRealTimeSplitter(const bool bRealtime);

  /*! Returns true if the tree and frame are updated in realtime when 
      moving the splitter.
    @return True if realtime mode. */
  bool IsRealTimeSplitter() const;
  //@}

  //@{
  /*! Specify how the context menu should be handle:
    @param eContextMenuMode Defines if and for which items context help should be handled. */
  void SetContextMenuMode(const TreePropSheet::enContextMenuMode eContextMenuMode);

  /*! Returns the context menu mode. 
    @return Context menu mode as described in TreePropSheet::enContextMenuMode. */
  TreePropSheet::enContextMenuMode GetContextMenuMode() const;
  //@}

// Overridables
  /*! Specifies the width of the tree control, when the sheet is in tree
	    view mode. The default value (if this method is not called) is 150
	    pixels.

	    This method can be called at any time if the sheet is resizable. 

	  @param nWidth The width in pixels for the page tree. 
  	@return TRUE on success, FALSE otherwise (control created and
            not in resizable mode). */
	virtual BOOL SetTreeWidth(int nWidth);

// Overridables
  /*! Create the splitter control. This can be overidden to create a different
      object. 
    @return Managed pointer to splitter object. */
  virtual tSplitterPtr CreateSplitterObject() const;

  /*! Returns the splitter window or NULL if the sheet is not resizable.
      The splitter window holds the tree in the left pane and the frame
      in the right pane.
    @return Pointer to splitter object or NULL if not resizable. */
  virtual tSplitter* GetSplitterObject() const;

  /*! Return the CWnd associated with the splitter object. 
    @return CWnd associated with the splitter object or NULL if not defined. */
  virtual CWnd* GetSplitterWnd() const;

  /*! Create the layout manager object. 
  @return Managed pointer to layout manager.  */
  virtual tLayoutManagerPtr CreateLayoutManagerObject();

  /*! Returns the layout manager or NULL if the settings are such that
      it is not needed (not resizable and no splitter).
  @return Pointer to layout manager.  */
  virtual tLayoutManager* GetLayoutManagerObject() const;

	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTreePropSheetEx)
	//}}AFX_VIRTUAL


  // Message handlers
protected:
	//{{AFX_MSG(CTreePropSheetEx)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Helpers
private:
  /*! Initialize internal value. */
	void Init();
protected:
  /*! Indicate if setting the width or ratio width is a valid operation. 
    @return bool True if it is valid. Returns 0 if it is not possible to 
                 determine the tree width. */
  bool IsSettingWidthValid() const;

  /*! Update the internal layout manager with the minimum size. */
  void UpdateSheetMinimumSize();

  /*! Update the internal layout manager with the maximum size. */
  void UpdateSheetMaximumSize();

// Members
private:
  /*! Flag defining if the window is resizable. */
  bool m_bIsResizable;

  /*! Flag indicating if the tree is resizable. */
  bool m_bIsTreeResizable;

  /*! Tree sizing mode. */
  enTreeSizingMode m_eSizingMode;

  /*! Realtime spliter mode. */
  bool m_bIsRealtimeSplitterMode;

  /*! ResibleLib hook. */
  tLayoutManagerPtr m_pResizeHook;

  /*! Splitter used in tree mode. */
  tSplitterPtr m_pSplitter;

  /*! Context menu mode. */
  enContextMenuMode m_eContextMenuMode;

  /*! Sheet minimum size. */
  CSize m_sizeSheetMinimumSize;

  /*! Sheet maximum size. */
  CSize m_sizeSheetMaximumSize;

  /*! Splitter width. */
  int m_nSplitterWidth;

  /*! Minimum tree size. */
  int m_nTreeMinimumSize;

  /*! Minimum tree size. */
  int m_nFrameMinimumSize;
};

}
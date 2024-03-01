#pragma once

#include <afxcmn.h>
#include <afxlistctrl.h>
#include <afxtabctrl.h>
#include <afxole.h>

#include "ResizablePage.h"

#include "../Controls/WinsListCtrl.h"
#include "../PropertySheet/TreePropSheetUtil.hpp"
#include "../../Data/WinsNumberManager.h"

UINT TaskCountLineFunc(LPVOID pParam);
void AddFiles(CString strPath);


/////////////////////////////////////////////////////////////////////////////
// CPageWins dialog
class CPageWins 
	: public CResizablePage,
	public TreePropSheet::CWhiteBackgroundProvider
{
	DECLARE_DYNCREATE(CPageWins)

public:
	CPageWins();   // standard constructor
	~CPageWins();

	enum			{ IDD = IDD_PAGE_WINS };

//	void			SetData(MapWins* pMap);
	void			SetList();

	void			Initialize();
	void			AddFiles();
	DWORD			GetTargetFileCount();
	BOOL			TaskCountLine(CString strPath);
	void			ShowResult();
	void			UpdateStatus();
	void			UpdateStaticText(UINT uID);
	void			UpdateStaticText(CString strText);

	CMFCListCtrl*	GetListCtrl();
	CProgressCtrl*	GetProgressCtrl();
	
	void			TaskFinish()											{			m_pThread = NULL;						}

	void			SetPath(CString strPath)								{			m_strPath = strPath;					}
	CString			GetPath()												{			return m_strPath;						}

	void			SetFileFilter(CString strExt)							{			m_strFileExts = strExt;	ParseFilter();	}

	void			SetIncludeSubDirectory(BOOL bInclude)					{			m_bIncludeSubPath = bInclude;			}
	BOOL			GetIncludeSubDirectory()								{			return m_bIncludeSubPath;				}

protected:
	//MapWins*		m_pWins;
	//CMFCListCtrl	m_wndList;
	CMFCTabCtrl		m_wndTab;

	CWinsListCtrl	m_wndList;
	CProgressCtrl	m_wndProgress;
	DWORD			m_dwTotalFiles;

	BOOL			ParseFilter();
	void			FindFiles(CString strPath);

	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	afx_msg void	OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnable(BOOL bEnable);

protected:
	CWnd*			m_pNofityWnd;
	CWinThread*		m_pThread;
	CString			m_strPath;
	CString			m_strFileExts;
	CString			m_strStaticMessage;
	CPtrArray		m_arFiles;
	CStringArray	m_arFileExt;
	BOOL			m_bIncludeSubPath;


	CStringArray	m_arListColumn;

	DECLARE_MESSAGE_MAP()
};

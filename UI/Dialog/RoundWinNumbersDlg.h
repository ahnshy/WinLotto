#pragma once

#include <afxcmn.h>
#include <afxlistctrl.h>
#include <afxtabctrl.h>
#include <afxole.h>
#include <afxdialogex.h>

#include "../Controls/WinsListCtrl.h"
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
	void			InitCtrl();
	void			CalcContols();

	void			ShowTabWindow(int nTabIndex);
	void			MoveTabWindow();

	void			AddFiles();
	DWORD			GetTargetFileCount();
	BOOL			TaskCountLine(CString strPath);
	void			ShowResult();
	void			UpdateStatus();
	void			UpdateStaticText(UINT uID);
	void			UpdateStaticText(CString strText);

	CMFCListCtrl*	GetListCtrl();
	CProgressCtrl*	GetProgressCtrl();

	void			ChangeTabStyle();
	void			CleanUp();
	
	void			TaskFinish()											{			m_pThread = NULL;						}

	void			SetPath(CString strPath)								{			m_strPath = strPath;					}
	CString			GetPath()												{			return m_strPath;						}

	void			SetFileFilter(CString strExt)							{			m_strFileExts = strExt;	ParseFilter();	}

	void			SetIncludeSubDirectory(BOOL bInclude)					{			m_bIncludeSubPath = bInclude;			}
	BOOL			GetIncludeSubDirectory()								{			return m_bIncludeSubPath;				}

protected:
	//MapWins*		m_pWins;
	//CMFCListCtrl	m_wndList;
	CTabCtrl		m_wndTab;
	CRect			m_rcTab;
	CPtrArray				m_arWnd;
	CDialogEx*		m_pCurrent;

	CWinsListCtrl	m_wndList;
	CProgressCtrl	m_wndProgress;
	DWORD			m_dwTotalFiles;

	BOOL			ParseFilter();
	void			FindFiles(CString strPath);

	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	//afx_msg void	OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg	void	OnTcnSelchangeTabResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnEnable(BOOL bEnable);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg	void	OnWindowPosChanged(WINDOWPOS* lpwndpos);

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

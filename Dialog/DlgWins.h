#pragma once

#include "../resource.h"
#include "afxcmn.h"

UINT TaskCountLineFunc(LPVOID pParam);
void AddFiles(CString strPath);
/////////////////////////////////////////////////////////////////////////////
// CDlgWins dialog
class CDlgWins : public CDialog
{
public:
	CDlgWins(CWnd* pParent = NULL);   // standard constructor
	~CDlgWins();

	enum			{ IDD = IDC_LIST_WINS };

	void			Initialize();
	void			AddFiles();
	DWORD			GetTargetFileCount();
	BOOL			TaskCountLine(CString strPath);
	void			ShowResult();
	void			UpdateStatus();
	void			UpdateStaticText(UINT uID);
	void			UpdateStaticText(CString strText);

	CListCtrl*		GetListCtrl();
	CProgressCtrl*	GetProgressCtrl();
	
	void			TaskFinish()											{			m_pThread = NULL;						}

	void			SetPath(CString strPath)								{			m_strPath = strPath;					}
	CString			GetPath()												{			return m_strPath;						}

	void			SetFileFilter(CString strExt)							{			m_strFileExts = strExt;	ParseFilter();	}

	void			SetIncludeSubDirectory(BOOL bInclude)					{			m_bIncludeSubPath = bInclude;			}
	BOOL			GetIncludeSubDirectory()								{			return m_bIncludeSubPath;				}

protected:
	CListCtrl		m_wndList;
	CProgressCtrl	m_wndProgress;
	DWORD			m_dwTotalFiles;

	BOOL			ParseFilter();
	void			FindFiles(CString strPath);

	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	afx_msg void	OnItemchangedListRecord(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	CWnd*			m_pNofityWnd;
	CWinThread*		m_pThread;
	CString			m_strPath;
	CString			m_strFileExts;
	CString			m_strStaticMessage;
	CPtrArray		m_arFiles;
	CStringArray	m_arFileExt;
	BOOL			m_bIncludeSubPath;

	DECLARE_MESSAGE_MAP()
};

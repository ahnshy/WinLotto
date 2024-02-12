#pragma once

/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class
class CWinsNumberManager
{
public:
	CWinsNumberManager(void);
	~CWinsNumberManager(void);

	static CWinsNumberManager*	GetInstance();
	static void				DestroyInstance();

	// Init
	void					Initialize();
	void					SetEmpty();

	void					SaveConfig();
	void					ReadConfig();

	// Getter / Setter
	CString					GetPath()										{			return m_DataObject.m_strPath;							}
	void					SetPath(CString strPath)						{			m_DataObject.m_strPath		= strPath;					}

	CString					GetCurrentPath()								{			return m_DataObject.m_strCurrentPath;					}
	void					SetCurrentPath(CString strPath)					{			m_DataObject.m_strCurrentPath= strPath;					}

	CString					GetFilter()										{			return m_DataObject.m_strFilter;						}
	void					SetFilter(CString strFilter)					{			m_DataObject.m_strFilter		= strFilter;			}

	int						GetFilterIndex()								{			return m_DataObject.m_nIndex;							}
	void					SetFilterIndex(int nIndex)						{			m_DataObject.m_nIndex		=	nIndex;					}

	BOOL					GetIncludeSubDirectory()						{			return m_DataObject.m_bIncludeSubDirectory;				}
	void					SetIncludeSubDirectory(BOOL bInclude)			{			m_DataObject.m_bIncludeSubDirectory		=	bInclude;	}


	// Static :: Path
	static const CString	PathName;
	static const CString	CurrentPath;
	static const CString	IncludeSubDirectory;

	// About FileFilter
	static const CString	FileFilter;
	static const CString	FileFilterIndex;

protected:
	SMainDlgValueObject		m_DataObject;
	static CWinsNumberManager*	m_pWinsNumberManager;
};
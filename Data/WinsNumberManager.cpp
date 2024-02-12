/////////////////////////////////////////////////////////////////////////
// #20160724 Work Ahnshy :: Winning Number Manage Class
// CWinsNumberManager class

/////////////////////////////////////////////////////////////////////////////
// Architecture - will be updated
//
//

#include "StdAfx.h"
#include "WinsNumberManager.h"

#include "../Util/IniUtil.h"

// FileInfo
#define CONFIG_NARCISSUS_FILENAME							TEXT("Narcissus.ini")
#define CONFIG_SECTION_CONFIG								TEXT("Config")

#define CONFIG_NARCISSUS_PATH								TEXT("path")
#define CONFIG_NARCISSUS_INCLUDESUBDIRECTORY				TEXT("includesubdirectory")
#define CONFIG_NARCISSUS_CURRENTPATH						TEXT("currentpath")
#define CONFIG_NARCISSUS_FILTER								TEXT("filter")
#define CONFIG_NARCISSUS_FILTERINDEX						TEXT("filterindex")

// About KeyString
static const CString	PathName;
static const CString	IncludeSubDirectory;
static const CString	CurrentDirectory;
static const CString	FileFilter;
static const CString	FileFilterIndex;

// Static :: Magnify Init
const CString CWinsNumberManager::PathName						= CONFIG_NARCISSUS_PATH;
const CString CWinsNumberManager::IncludeSubDirectory			= CONFIG_NARCISSUS_INCLUDESUBDIRECTORY;
const CString CWinsNumberManager::CurrentPath					= CONFIG_NARCISSUS_CURRENTPATH;
const CString CWinsNumberManager::FileFilter					= CONFIG_NARCISSUS_FILTER;
const CString CWinsNumberManager::FileFilterIndex				= CONFIG_NARCISSUS_FILTERINDEX;

CWinsNumberManager*	CWinsNumberManager::m_pWinsNumberManager			= NULL;
/////////////////////////////////////////////////////////////////////////////
// Management Instance
CWinsNumberManager* CWinsNumberManager::GetInstance()
{
	if (m_pWinsNumberManager != NULL)
		return m_pWinsNumberManager;

	m_pWinsNumberManager = new CWinsNumberManager;
	return m_pWinsNumberManager;
}

void CWinsNumberManager::DestroyInstance()
{
	if (NULL == m_pWinsNumberManager)
		return;

	delete m_pWinsNumberManager;
	m_pWinsNumberManager = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWinsNumberManager
CWinsNumberManager::CWinsNumberManager(void)
{
	SetEmpty();

	if (m_pWinsNumberManager != NULL)
		ASSERT(FALSE);
	else
		m_pWinsNumberManager = this;
}

CWinsNumberManager::~CWinsNumberManager(void)
{
	SetEmpty();
}

void CWinsNumberManager::SetEmpty()
{
	m_DataObject.Empty();
}

void CWinsNumberManager::ReadConfig()
{
	GetPrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::PathName, NULL, m_DataObject.m_strPath.GetBuffer(BUFSIZ), BUFSIZ, CONFIG_NARCISSUS_FILENAME);
	m_DataObject.m_strPath.ReleaseBuffer();
	if (m_DataObject.m_strPath.IsEmpty())
	{
		GetModuleFileName(NULL, m_DataObject.m_strPath.GetBuffer(BUFSIZ), BUFSIZ);
		m_DataObject.m_strPath.ReleaseBuffer();

		PathRemoveFileSpec(m_DataObject.m_strPath.GetBuffer(BUFSIZ));
		m_DataObject.m_strPath.ReleaseBuffer();

		PathAddBackslash(m_DataObject.m_strPath.GetBuffer(BUFSIZ));
		m_DataObject.m_strPath.ReleaseBuffer();
	}

	GetPrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::CurrentPath, NULL, m_DataObject.m_strCurrentPath.GetBuffer(BUFSIZ), BUFSIZ, CONFIG_NARCISSUS_FILENAME);
	m_DataObject.m_strCurrentPath.ReleaseBuffer();

	GetPrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::FileFilter, NULL, m_DataObject.m_strFilter.GetBuffer(BUFSIZ), BUFSIZ, CONFIG_NARCISSUS_FILENAME);
	m_DataObject.m_strFilter.ReleaseBuffer();

	m_DataObject.m_nIndex = GetPrivateProfileInt(CONFIG_SECTION_CONFIG, CWinsNumberManager::FileFilterIndex, 0, CONFIG_NARCISSUS_FILENAME);

	CString strBuffer;
	GetPrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::IncludeSubDirectory, NULL, strBuffer.GetBuffer(BUFSIZ), BUFSIZ, CONFIG_NARCISSUS_FILENAME);
	strBuffer.ReleaseBuffer();
	if (strBuffer.IsEmpty())
		strBuffer.Append(_T("true"));

	m_DataObject.m_bIncludeSubDirectory = CIniUtil::GetBoolean(strBuffer);
}

void CWinsNumberManager::SaveConfig()
{
	VERIFY(WritePrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::PathName, m_DataObject.m_strPath, CONFIG_NARCISSUS_FILENAME));

	VERIFY(WritePrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::CurrentPath, m_DataObject.m_strCurrentPath, CONFIG_NARCISSUS_FILENAME));

	VERIFY(WritePrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::FileFilter, m_DataObject.m_strFilter, CONFIG_NARCISSUS_FILENAME));

	CString strBuffer;
	strBuffer.Format(_T("%d"), m_DataObject.m_nIndex);
	VERIFY(WritePrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::FileFilterIndex, strBuffer, CONFIG_NARCISSUS_FILENAME));

	VERIFY(WritePrivateProfileString(CONFIG_SECTION_CONFIG, CWinsNumberManager::IncludeSubDirectory, CIniUtil::Boolean2String(m_DataObject.m_bIncludeSubDirectory), CONFIG_NARCISSUS_FILENAME));
}

void CWinsNumberManager::Initialize()
{	
}
#include "StdAfx.h"
#include "PathManager.h"
#include "../Helper/DirectoryHelper.h"

#define BACKSLASH	_T('\\')
#define PATH_LOG_FOLDERNAME							TEXT("log")
#define PATH_IMAGES_FOLDERNAME						TEXT("images")
#define PATH_CONFIG_FOLDERNAME						TEXT("config")
#define PATH_STORAGE_FOLDERNAME						TEXT("storage")
#define PATH_COMPANYNAME							TEXT("")
#define PATH_PROGRAMNAME							TEXT("WinLotto")

#define PATH_ROUND_FILENAME							TEXT("rounds.html")

const CString CPathManager::LogForderName			= PATH_LOG_FOLDERNAME;
const CString CPathManager::ConfigForderName		= PATH_CONFIG_FOLDERNAME;
const CString CPathManager::StorageForderName		= PATH_STORAGE_FOLDERNAME;
const CString CPathManager::ImageForderName			= PATH_IMAGES_FOLDERNAME;
const CString CPathManager::CompanyName				= PATH_COMPANYNAME;

const CString CPathManager::RoundFileName			= PATH_ROUND_FILENAME;

CPathManager*	CPathManager::m_pPathManager		= NULL;

CPathManager* CPathManager::GetInstance()
{
	if (m_pPathManager != NULL)
		return m_pPathManager;

	m_pPathManager = new CPathManager;
	return m_pPathManager;
}

void CPathManager::DestroyInstance()
{
	if (NULL == m_pPathManager)
		return;

	delete m_pPathManager;
	m_pPathManager = NULL;
}

CPathManager::CPathManager(void)
{
	SetEmpty();

	if (m_pPathManager != NULL)
		ASSERT(FALSE);
	else
		m_pPathManager = this;
}

CPathManager::~CPathManager(void)
{
	SetEmpty();
}

void CPathManager::SetEmpty()
{
	m_strBinPath.Empty();
	m_strLogPath.Empty();
	m_strImagePath.Empty();
	m_strDataPath.Empty();
	m_strEnvFile.Empty();
}

void CPathManager::Initialize()
{	
	// BinPath => "exeÀÇ °æ·Î\"
	m_strBinPath	= CDirectoryHelper::GetBinaryPath();
	m_strBinPath.TrimRight(BACKSLASH);
	m_strBinPath.AppendChar(BACKSLASH);

	// LogPath => "BinPath\Log\"
	m_strLogPath	= m_strBinPath;
	m_strLogPath.Append(CPathManager::LogForderName);
	m_strLogPath.AppendChar(BACKSLASH);
	CreateDirectory(m_strLogPath, NULL);

	// DatPath => "BinPath\Images\"
	m_strImagePath	= m_strBinPath;
	m_strImagePath.Append(CPathManager::ImageForderName);
	m_strImagePath.AppendChar(BACKSLASH);

	// ConfigPath => "BinPath\Config\"
	m_strConfigPath	= m_strBinPath;
	m_strConfigPath.Append(CPathManager::ConfigForderName);
	m_strConfigPath.AppendChar(BACKSLASH);
	CreateDirectory(m_strConfigPath, NULL);

	// StoragePath => "BinPath\Storage\"
	m_strStoragePath= m_strBinPath;
	m_strStoragePath.Append(CPathManager::StorageForderName);
	m_strStoragePath.AppendChar(BACKSLASH);
	CreateDirectory(m_strStoragePath, NULL);

	// TempPath => _T("%USERPROFILE%\AppData\Local\Temp\XXXXXXXX\")
	m_strTempPath = CDirectoryHelper::GetTempPath();
	m_strTempPath.TrimRight(BACKSLASH);
	m_strTempPath.AppendChar(BACKSLASH);
	CreateDirectory(m_strTempPath, NULL);

	// Round File (static resource)
	m_strRoundFile.Append(m_strStoragePath);
	m_strRoundFile.Append(PATH_ROUND_FILENAME);
}

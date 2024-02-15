#pragma once

class CPathManager
{
public:
	CPathManager(void);
	~CPathManager(void);

	static CPathManager*	GetInstance();
	static void				DestroyInstance();

	// Init
	void					Initialize();
	void					SetEmpty();

	// Getter::Forder
	CString					GetBinPath()							{ return m_strBinPath;			}
	CString					GetLogPath()							{ return m_strLogPath;			}
	CString					GetImagePath()							{ return m_strImagePath;		}
	CString					GetConfigPath()							{ return m_strConfigPath;		}
	CString					GetStoragePath()						{ return m_strStoragePath;		}
	CString					GetDataPath()							{ return m_strDataPath;			}
	CString					GetTempPath()							{ return m_strTempPath;			}
	
	CString					GetRoundFilePath()						{ return m_strRoundFile;		}

	// Getter

	// Setter
	void					SetTempPath(CString	strPath)			{ m_strTempPath	= strPath;		}
	void					SetWorkPath(CString	strPath)			{ m_strDataPath	= strPath;		}

	// Static :: ForderName
	static const CString	LogForderName;
	static const CString	VoicesForderName;
	static const CString	ConfigForderName;
	static const CString	StorageForderName;
	static const CString	ImageForderName;
	static const CString	DemoSampleForderName;
	static const CString	CompanyName;
	static const CString	ProgramName;
	
	// Static :: FileName
	static const CString	RoundFileName;

protected:
	CString					m_strBinPath;
	CString					m_strLogPath;
	CString					m_strImagePath;
	CString					m_strConfigPath;
	CString					m_strStoragePath;
	CString					m_strDataPath;
	CString					m_strTempPath;
	CString					m_strEnvFile;

	CString					m_strRoundFile;

	static CPathManager*	m_pPathManager;
};

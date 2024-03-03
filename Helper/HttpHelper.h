#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: HttpHelper class
// CHttpHelper class
// Bug Report :: ahnshy@naver.com

#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

class CHttpHelper
{
public:
    CHttpHelper();
    ~CHttpHelper();
public:
    BOOL GetHttpFile(CString strUrl,  CString strPath);

private:
	BOOL Initialize();
private:
    HINTERNET _internet_handle;
};

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: HttpHelper class
// CHttpHelper class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "HttpHelper.h"

#define _CRT_SECURE_NO_WARNINGS

CHttpHelper::CHttpHelper()
{
	_internet_handle = nullptr;
	Initialize();
}

CHttpHelper::~CHttpHelper()
{
	if (nullptr != _internet_handle)
	{
		InternetCloseHandle(_internet_handle);
		_internet_handle = nullptr;
	}
}
BOOL CHttpHelper::Initialize()
{
	_internet_handle = InternetOpen(_T("agent"), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
	if (nullptr == _internet_handle)
		return FALSE;

	return TRUE;
}
BOOL CHttpHelper::GetHttpFile(CString strUrl, CString strPath)
{
	BOOL bResult = FALSE;
	do {
		if (nullptr == _internet_handle)
			break;

		const HINTERNET open_url_handle = InternetOpenUrl(_internet_handle, strUrl, nullptr, 0, INTERNET_FLAG_RELOAD, 0);
		if (nullptr == open_url_handle)
			break;

		DWORD dwStatus = 0;
		DWORD dwStatusSize = sizeof(dwStatus);
		if (FALSE == HttpQueryInfo(open_url_handle, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwStatusSize, nullptr))
			break;

		if (sizeof(dwStatus) != dwStatusSize)
			break;

		if (HTTP_STATUS_OK != dwStatus)
			break;

		const int nBufferSize = BUFSIZ * 2;
		char szBuffer[nBufferSize] = {};
		CFile file;
		if (file.Open(strPath, CFile::modeCreate | CFile::modeReadWrite))
		{
				DWORD dwReadByte = 0, dwReadAvailable = 0;
				do {
					if (FALSE == InternetQueryDataAvailable(open_url_handle,&dwReadAvailable, 0, 0))
					{
							//result = false;
							//__debug_printf_trace("InternetQueryDataAvailable failed.");
							break;
					}

					if (dwReadAvailable > nBufferSize)
						dwReadAvailable = nBufferSize;

					if (FALSE == InternetReadFile(open_url_handle, szBuffer, dwReadAvailable, &dwReadByte))
							break;

					if (0 < dwReadByte)
					{
						file.Write(szBuffer, dwReadByte);
						memset(szBuffer, 0x00, dwReadByte);
					}
				} while (0 != dwReadAvailable);
				file.Close();
		}
		InternetCloseHandle(open_url_handle);
	} while (FALSE);

	return bResult;
}
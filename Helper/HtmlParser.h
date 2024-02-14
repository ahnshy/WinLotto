#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: Paser Utility
// CHtmlParser class
// Bug Report :: ahnshy@naver.com

class CHtmlParser
{
public:
    CHtmlParser();
    ~CHtmlParser();
public:
    //long GetWinsNumber(CString strPath, MapWins& wins);
	long GetRounds(CString strPath, OUT CStringArray& strOut);

private:
    BOOL Initialize();
private:

};

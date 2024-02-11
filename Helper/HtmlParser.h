#pragma once

/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: Paser Utility
// CHtmlParser class
// Bug Report :: ahnshy@naver.com

#include "../Data/WinsItem.h"

class CHtmlParser
{
public:
    CHtmlParser();
    ~CHtmlParser();
public:
    long GetWinsNumber(CString strPath, MapWins& wins);

private:
    BOOL Initialize();
private:

};

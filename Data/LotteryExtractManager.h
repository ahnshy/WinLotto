#pragma once

#include "afxTempl.h"

/****************************************************************************
Class : CLotteryExtractManager
Description : È®·ü ±â¹Ý ÃßÃ·±â ( ÃÖ´ë È®·ü 1/100000000 (1¾ï) )
****************************************************************************/
template <class T> 
class CLotteryExtractManager
{
public:
	CLotteryExtractManager();

	void Initialize();
	void Add(T* minmax, INT32 nRate);
	BOOL Lottery();
	const T* GetNumbers();

protected:
	enum{ HUNDRED_MILLION = 100000000 };

	CArray<INT32, INT32> m_arrRate;
	CArray<T, T&> m_arrNumber;

	INT32 m_nCount;
};
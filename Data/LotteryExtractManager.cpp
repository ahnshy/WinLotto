#include "LotteryExtractManager.h"

template <class T> CLotteryExtractManager<T>::CLotteryExtractManager()
{
	m_nCount = -1;
}

template <class T> void CLotteryExtractManager<T>::Initialize()
{
	m_arrRate.RemoveAll();
	m_arrNumber.RemoveAll();
}

/*-----------------------------------------------------------------------------
	함수명 : Add
	인  자 : t : 추가할 타입 rate : 확률  (1/rate)의 확률, 
	리턴값 : 없음
	설  명 : 확률데이타를 추가한다. 
-----------------------------------------------------------------------------*/
template <class T> void CLotteryExtractManager<T>::Add(T* t, INT32 nRate)
{
	nRate = HUNDRED_MILLION / nRate;

	if (m_arrRate.GetSize() > 0)
		m_arrRate.Add(nRate + m_arrRate[ m_arrRate.GetSize()-1 ]);
	else 
		m_arrRate.Add(nRate);

	m_arrNumber.Add( *t );
}

/*-----------------------------------------------------------------------------
	함수명 : Lottery
	인  자 : 없음 
	리턴값 : 성공 시 TRUE, 실패 시 FALSE
-----------------------------------------------------------------------------*/
template <class T> BOOL CLotteryExtractManager<T>::Lottery()
{
	INT32 size = m_arrRate.GetSize();	ASSERT( size > 0 );
	if( size <= 0 ) return FALSE;

	INT32 seed = ( rand()*rand()+rand() ) % HUNDRED_MILLION;
	if( seed <= m_arrRate[ size-1 ] )
	{
		for( INT32 i=0; i<size; i++ )
		{
			if( seed <= m_arrRate[i] )
			{
				m_nCount = i;
				break;
			}
		}
		return TRUE;
	}

	m_nCount = -1;
	return FALSE;
}

/*-----------------------------------------------------------------------------
	함수명 : GetNumbers
	인  자 : 없음 
	리턴값 : 당첨된 데이타 타입 (없으면 NULL)
	설  명 : 당첨된 데이타 타입을 얻는다. 
-----------------------------------------------------------------------------*/
template <class T> const T* CLotteryExtractManager<T>::GetNumbers()
{
	if (m_nCount == -1) return NULL;
	return &m_arrNumber[m_nCount];
}
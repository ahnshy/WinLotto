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
	�Լ��� : Add
	��  �� : t : �߰��� Ÿ�� rate : Ȯ��  (1/rate)�� Ȯ��, 
	���ϰ� : ����
	��  �� : Ȯ������Ÿ�� �߰��Ѵ�. 
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
	�Լ��� : Lottery
	��  �� : ���� 
	���ϰ� : ���� �� TRUE, ���� �� FALSE
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
	�Լ��� : GetNumbers
	��  �� : ���� 
	���ϰ� : ��÷�� ����Ÿ Ÿ�� (������ NULL)
	��  �� : ��÷�� ����Ÿ Ÿ���� ��´�. 
-----------------------------------------------------------------------------*/
template <class T> const T* CLotteryExtractManager<T>::GetNumbers()
{
	if (m_nCount == -1) return NULL;
	return &m_arrNumber[m_nCount];
}
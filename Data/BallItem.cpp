/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: BallItem class
// CBallItem class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "BallItem.h"

#define _CRT_SECURE_NO_WARNINGS

CBallItem::CBallItem()
{
	SetEmpty();
}

CBallItem::CBallItem(RectF rc, COLORREF c)
{
	CBallItem();
	m_rc = rc, m_color = c;
	m_sizeDirection.SetSize(1, 1);
}

CBallItem::~CBallItem()
{
}

void CBallItem::SetEmpty()
{
	m_rc.X = m_rc.Y = m_rc.Width = m_rc.Height = 0;
	m_sizeDirection.SetSize(1, 1);
}

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
	m_fVelocity = PointF(0.0f, 0.0f);
}

CBallItem::~CBallItem()
{
}

void CBallItem::SetEmpty()
{
	m_rc.X = m_rc.Y = m_rc.Width = m_rc.Height = 0;
	m_fVelocity = PointF(0.0f, 0.0f);
	m_bSleeping = false;
}

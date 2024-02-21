#pragma once

#include <gdiplus.h>
using namespace Gdiplus;

/////////////////////////////////////////////////////////////////////////
// #20240219 Work Ahnshy :: BallItem class
// CBallItem class
// Bug Report :: ahnshy@naver.com
using namespace std;

class CBallItem
{
public:
	CBallItem();
	CBallItem(RectF rc, COLORREF c);;
	~CBallItem();

	void		SetEmpty();

	RectF&		GetRect()				{ return m_rc; }
	COLORREF	GetColor()				{ return m_color; }

	DWORD		SetRect(RectF rc)	{ m_rc = rc; }
	COLORREF	SetColor(COLORREF c){ m_color = c; }

private:
	RectF			m_rc;
	COLORREF		m_color;
};
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

	void			SetEmpty();

	// Getter
	RectF&			GetRect()					{ return m_rc;			}
	COLORREF		GetColor()					{ return m_color;		}
	PointF&			GetVelocity()				{ return m_fVelocity;	}

	// Setter
	void			SetRect(RectF rc)			{ m_rc = rc;			}
	void			SetColor(COLORREF c)		{ m_color = c;			}
	void			SetVelocity(PointF v)		{ m_fVelocity = v;		}

private:
	RectF			m_rc;
	COLORREF		m_color;
	PointF			m_fVelocity;
};
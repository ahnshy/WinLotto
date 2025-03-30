// LottoListCtrl.cpp

#include "stdafx.h"
#include "LottoListCtrl.h"
#include <math.h>

CLottoListCtrl::CLottoListCtrl() : m_animationTick(0) {}
CLottoListCtrl::~CLottoListCtrl() {}

BEGIN_MESSAGE_MAP(CLottoListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CLottoListCtrl::OnCustomDraw)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CLottoListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	SetTimer(1, 30, NULL);
}

void CLottoListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	m_animationTick++;
	Invalidate(FALSE);
	CListCtrl::OnTimer(nIDEvent);
}

void CLottoListCtrl::SetData(const vector<LottoData>& data)
{
	m_data = data;
	DeleteAllItems();
	for (size_t i = 0; i < data.size(); ++i)
	{
		CString str;
		str.Format(_T("%d"), (int)(i + 1));
		InsertItem((int)i, str);
		SetItemText((int)i, 1, _T(""));
		SetItemText((int)i, 2, _T(""));
	}
	Invalidate();
}

void CLottoListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		int idx = (int)pLVCD->nmcd.dwItemSpec;
		if (idx < 0 || idx >= (int)m_data.size())
			return;

		CRect rcSubItem;
		GetSubItemRect(idx, 1, LVIR_BOUNDS, rcSubItem);

		Graphics g(pLVCD->nmcd.hdc);
		g.SetSmoothingMode(SmoothingModeAntiAlias);

		bool selected = GetItemState(idx, LVIS_SELECTED) & LVIS_SELECTED;

		DrawBalls(g, rcSubItem, m_data[idx], selected);

		*pResult = CDRF_SKIPDEFAULT;
	}
}

void CLottoListCtrl::DrawBalls(Graphics& g, CRect rc, const LottoData& lotto, bool selected)
{
	const int BALL_SIZE = 26;
	const int SPACING = 6;
	int startX = rc.left + 5;
	int centerY = rc.CenterPoint().y;

	Gdiplus::Pen pen(Color(255, 0, 0, 0));
	Gdiplus::SolidBrush textBrush(Color(255, 0, 0, 0));
	Gdiplus::FontFamily fontFamily(DEFAULT_FONT);
	Gdiplus::Font font(&fontFamily, 120, FontStyleBold, UnitPixel);
	Gdiplus::StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);

	int glow = (int)(abs(sin(m_animationTick * 0.1)) * 30);

	for (size_t i = 0; i < lotto.numbers.size(); ++i)
	{
		int number = lotto.numbers[i];
		CRect ballRect(startX, centerY - BALL_SIZE / 2, startX + BALL_SIZE, centerY + BALL_SIZE / 2);

		Color baseColor = GetBallColor(number);
		Color glowColor = Color(255, min(baseColor.GetR() + glow, 255), min(baseColor.GetG() + glow, 255), min(baseColor.GetB() + glow, 255));
		SolidBrush brush(glowColor);
		g.FillEllipse(&brush, ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE);

		g.DrawEllipse(&pen, ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE);

		CString str;
		str.Format(_T("%d"), number);
		g.DrawString(str, -1, &font, RectF((float)ballRect.left, (float)ballRect.top, (float)BALL_SIZE, (float)BALL_SIZE), &sf, &textBrush);

		startX += BALL_SIZE + SPACING;
	}

	// 보너스 공도 동그랗게
	CRect bonusRect(startX, centerY - BALL_SIZE / 2, startX + BALL_SIZE, centerY + BALL_SIZE / 2);
	SolidBrush bonusBrush(Color(255, 169, 169, 169));
	g.FillEllipse(&bonusBrush, bonusRect.left, bonusRect.top, BALL_SIZE, BALL_SIZE);
	g.DrawEllipse(&pen, bonusRect.left, bonusRect.top, BALL_SIZE, BALL_SIZE);

	CString bonusStr;
	bonusStr.Format(_T("%d"), lotto.bonus);
	g.DrawString(bonusStr, -1, &font, RectF((float)bonusRect.left, (float)bonusRect.top, (float)BALL_SIZE, (float)BALL_SIZE), &sf, &textBrush);
}

Color CLottoListCtrl::GetBallColor(int number)
{
	if (number <= 10) return Color(255, 255, 215, 0);
	if (number <= 20) return Color(255, 30, 144, 255);
	if (number <= 30) return Color(255, 220, 20, 60);
	if (number <= 40) return Color(255, 50, 205, 50);
	return Color(255, 169, 169, 169);
}
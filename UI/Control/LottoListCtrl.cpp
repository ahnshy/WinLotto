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
		if (idx < 0)
			return;

		CRect rcSubItem;
		GetSubItemRect(idx, 1, LVIR_BOUNDS, rcSubItem);

		CString numbersText = GetItemText(idx, 1);
		CString bonusText = GetItemText(idx, 2);

		Graphics g(pLVCD->nmcd.hdc);
		g.SetSmoothingMode(SmoothingModeAntiAlias);

		bool selected = GetItemState(idx, LVIS_SELECTED) & LVIS_SELECTED;
		bool hot = (GetHotItem() == idx);

		DrawBalls(g, rcSubItem, selected, hot, numbersText, bonusText);

		*pResult = CDRF_SKIPDEFAULT;
	}
}

void CLottoListCtrl::DrawBalls(Graphics& g, CRect rc, bool selected, bool hot, const CString& numbersText, const CString& bonusText)
{
	const int BALL_SIZE = 26;
	const int SPACING = 6;
	int startX = rc.left + 5;
	int centerY = rc.CenterPoint().y;

	SolidBrush textBrush(Color(255, 0, 0, 0));
	FontFamily fontFamily(DEFAULT_FONT);
	Gdiplus::Font font(&fontFamily, 9.0f, FontStyleBold, UnitPixel);
	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);

	std::vector<int> numbers;
	CString token;
	int curPos = 0;
	while ((token = numbersText.Tokenize(_T(","), curPos)) != "")
		numbers.push_back(_ttoi(token));

	for (size_t i = 0; i < numbers.size(); ++i)
	{
		int number = numbers[i];
		CRect ballRect(startX, centerY - BALL_SIZE / 2, startX + BALL_SIZE, centerY + BALL_SIZE / 2);

		Color baseColor = GetBallColor(number);
		SolidBrush brush(baseColor);
		g.FillEllipse(&brush, ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE);

		Pen pen(Color(255, 0, 0, 0));
		g.DrawEllipse(&pen, ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE);

		CString str;
		str.Format(_T("%d"), number);
		g.DrawString(str, -1, &font, RectF((float)ballRect.left, (float)ballRect.top, (float)BALL_SIZE, (float)BALL_SIZE), &sf, &textBrush);

		startX += BALL_SIZE + SPACING;
	}

	// 보너스 공
	//int nBonus = _ttoi(bonusText);
	CRect bonusRect(startX, centerY - BALL_SIZE / 2, startX + BALL_SIZE, centerY + BALL_SIZE / 2);
	SolidBrush bonusBrush(Color(255, 169, 169, 169));
	g.FillEllipse(&bonusBrush, bonusRect.left, bonusRect.top, BALL_SIZE, BALL_SIZE);
	Pen pen(Color(255, 0, 0, 0));
	g.DrawEllipse(&pen, bonusRect.left, bonusRect.top, BALL_SIZE, BALL_SIZE);

	CString bonusStr;
	bonusStr.Format(_T("%d"), _ttoi(bonusText));
	g.DrawString(bonusStr, -1, &font, RectF((float)bonusRect.left, (float)bonusRect.top, (float)BALL_SIZE, (float)BALL_SIZE), &sf, &textBrush);

	// Hover 시 하늘색 사각 테두리 강조 (라운드 대신 일반 사각형)
	if (hot)
	{
		Pen hoverPen(Color(255, 173, 216, 230), 2.0f);
		CRect rcHover = rc;
		rcHover.InflateRect(-2, -2);
		g.DrawRectangle(&hoverPen, rcHover.left, rcHover.top, rcHover.Width(), rcHover.Height());
	}
}

Color CLottoListCtrl::GetBallColor(int number)
{
	if (number <= 10) return Color(255, 255, 215, 0);
	if (number <= 20) return Color(255, 30, 144, 255);
	if (number <= 30) return Color(255, 220, 20, 60);
	if (number <= 40) return Color(255, 50, 205, 50);
	return Color(255, 169, 169, 169);
}

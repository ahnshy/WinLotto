#include "stdafx.h"
#include "LottoListCtrl.h"

CLottoListCtrl::CLottoListCtrl()
{
}

CLottoListCtrl::~CLottoListCtrl()
{
}

BEGIN_MESSAGE_MAP(CLottoListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CLottoListCtrl::OnCustomDraw)
END_MESSAGE_MAP()

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
		*pResult = CDRF_NOTIFYPOSTPAINT;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT)
	{
		CRect rcItem;
		GetItemRect((int)pLVCD->nmcd.dwItemSpec, &rcItem, LVIR_BOUNDS);

		Gdiplus::Graphics g(pLVCD->nmcd.hdc);
		g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		int number = (int)pLVCD->nmcd.dwItemSpec + 1;
		bool selected = GetItemState(pLVCD->nmcd.dwItemSpec, LVIS_SELECTED) & LVIS_SELECTED;

		DrawLottoBall(g, rcItem, number, selected);

		*pResult = CDRF_SKIPDEFAULT;
	}
}

void CLottoListCtrl::DrawLottoBall(Gdiplus::Graphics& g, CRect rc, int number, bool selected)
{
	const int BALL_SIZE = 24;
	CPoint center = rc.CenterPoint();
	CRect ballRect(center.x - BALL_SIZE / 2, center.y - BALL_SIZE / 2, center.x + BALL_SIZE / 2, center.y + BALL_SIZE / 2);

	// 1. 그림자
	Gdiplus::SolidBrush shadowBrush(Gdiplus::Color(60, 0, 0, 0));
	CRect shadowRect = ballRect + CPoint(2, 2);
	g.FillEllipse(&shadowBrush, Gdiplus::Rect(shadowRect.left, shadowRect.top, BALL_SIZE, BALL_SIZE));

	// 2. 공 그라데이션
	Gdiplus::Color color = GetBallColor(number);
	Gdiplus::GraphicsPath path;
	path.AddEllipse(ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE);

	Gdiplus::PathGradientBrush gradient(&path);
	gradient.SetCenterColor(Gdiplus::Color(255, min(color.GetR() + 60, 255), min(color.GetG() + 60, 255), min(color.GetB() + 60, 255)));
	Gdiplus::Color surround[] = { color };
	INT nCount = 1;
	gradient.SetSurroundColors(surround, &nCount);

	g.FillEllipse(&gradient, Gdiplus::Rect(ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE));

	// 3. 테두리
	Gdiplus::Pen pen(selected ? Gdiplus::Color(255, 255, 100, 100) : Gdiplus::Color(255, 0, 0, 0), selected ? 2.0f : 1.0f);
	g.DrawEllipse(&pen, Gdiplus::Rect(ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE));

	// 4. 텍스트
	CString str;
	str.Format(_T("%d"), number);
	Gdiplus::Font font(DEFAULT_FONT, 10, Gdiplus::FontStyleBold);
	Gdiplus::StringFormat sf;
	sf.SetAlignment(Gdiplus::StringAlignmentCenter);
	sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	Gdiplus::SolidBrush textBrush(selected ? Gdiplus::Color(255, 255, 255) : Gdiplus::Color(255, 0, 0, 0));
	g.DrawString(str, -1, &font, Gdiplus::RectF((float)ballRect.left, (float)ballRect.top, (float)BALL_SIZE, (float)BALL_SIZE), &sf, &textBrush);
}

Gdiplus::Color CLottoListCtrl::GetBallColor(int number) const
{
	if (number <= 10) return Gdiplus::Color(255, 255, 215, 0);    // 노랑
	if (number <= 20) return Gdiplus::Color(255, 30, 144, 255);   // 파랑
	if (number <= 30) return Gdiplus::Color(255, 220, 20, 60);    // 빨강
	if (number <= 40) return Gdiplus::Color(255, 50, 205, 50);    // 초록
	return Gdiplus::Color(255, 169, 169, 169);                    // 회색
}

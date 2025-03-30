#include "stdafx.h"
#include "LottoListCtrl.h"

CLottoListCtrl::CLottoListCtrl() {}
CLottoListCtrl::~CLottoListCtrl() {}

BEGIN_MESSAGE_MAP(CLottoListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CLottoListCtrl::OnCustomDraw)
END_MESSAGE_MAP()

void CLottoListCtrl::InitializeColumns()
{
	static CImageList imageList;
	imageList.Create(1, 36, ILC_COLOR32, 1, 1);
	SetImageList(&imageList, LVSIL_SMALL);
}

void CLottoListCtrl::InsertLottoRow(int index, const CString& numbersText, const CString& bonusText)
{
	CString gameStr;
	gameStr.Format(_T("%d"), index + 1);
	InsertItem(index, gameStr);
	SetItemText(index, 1, numbersText);
	SetItemText(index, 2, bonusText);
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
		int index = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		Gdiplus::Graphics g(pDC->GetSafeHdc());
		g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		bool selected = (GetItemState(index, LVIS_SELECTED) & LVIS_SELECTED) != 0;
		bool hot = (GetHotItem() == index);

		CRect rcNumbers, rcBonus, rcFull;
		GetSubItemRect(index, 1, LVIR_BOUNDS, rcNumbers);
		GetSubItemRect(index, 2, LVIR_BOUNDS, rcBonus);
		GetItemRect(index, &rcFull, LVIR_BOUNDS);

		if (selected)
		{
			rcFull.InflateRect(-1, -1);
			Gdiplus::SolidBrush solidBrush(Gdiplus::Color(128, 204, 232, 255));
			g.FillRectangle(&solidBrush, rcFull.left, rcFull.top, rcFull.Width(), rcFull.Height());
		}

		CString numbers = GetItemText(index, 1);
		CString bonus = GetItemText(index, 2);

		DrawBalls(g, rcNumbers, numbers, selected, hot);
		DrawBonusBall(g, rcBonus, bonus, selected);

		*pResult = CDRF_SKIPDEFAULT;
	}
}

void CLottoListCtrl::DrawBalls(Gdiplus::Graphics& g, const CRect& rc, const CString& numbersText, bool selected, bool hot)
{
	const int BALL_SIZE = 26;
	const int SPACING = 6;

	int x = rc.left + 5;
	int y = rc.CenterPoint().y;

	int fontHeight = BALL_SIZE / 2;
	Gdiplus::Font font(L"Arial", static_cast<Gdiplus::REAL>(fontHeight), Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 0, 0, 0));

	CString token;
	int curPos = 0;
	while (!(token = numbersText.Tokenize(_T(","), curPos)).IsEmpty())
	{
		int number = _ttoi(token);
		Gdiplus::Color color = GetBallColor(number);
		Gdiplus::SolidBrush brush(color);

		CRect r(x, y - BALL_SIZE / 2, x + BALL_SIZE, y + BALL_SIZE / 2);
		g.FillEllipse(&brush, r.left, r.top, BALL_SIZE, BALL_SIZE);
		Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0));
		g.DrawEllipse(&pen, r.left, r.top, BALL_SIZE, BALL_SIZE);

		CString str;
		str.Format(_T("%d"), number);
		g.DrawString(str, -1, &font, Gdiplus::RectF((Gdiplus::REAL)r.left, (Gdiplus::REAL)r.top, (Gdiplus::REAL)BALL_SIZE, (Gdiplus::REAL)BALL_SIZE), &format, &textBrush);

		x += BALL_SIZE + SPACING;
	}
}

void CLottoListCtrl::DrawBonusBall(Gdiplus::Graphics& g, const CRect& rc, const CString& bonusText, bool selected)
{
	const int BALL_SIZE = 26;
	int fontHeight = BALL_SIZE / 2;
	int x = rc.CenterPoint().x - BALL_SIZE / 2;
	int y = rc.CenterPoint().y;

	Gdiplus::Font font(DEFAULT_FONT, static_cast<Gdiplus::REAL>(fontHeight), Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	int bonus = _ttoi(bonusText);
	Gdiplus::SolidBrush bonusBrush(Gdiplus::Color(255, 169, 169, 169));
	CRect r(x, y - BALL_SIZE / 2, x + BALL_SIZE, y + BALL_SIZE / 2);
	g.FillEllipse(&bonusBrush, r.left, r.top, BALL_SIZE, BALL_SIZE);
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0));
	g.DrawEllipse(&pen, r.left, r.top, BALL_SIZE, BALL_SIZE);

	CString str;
	str.Format(_T("%d"), bonus);
	Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 0, 0, 0));
	g.DrawString(str, -1, &font, Gdiplus::RectF((Gdiplus::REAL)r.left, (Gdiplus::REAL)r.top, (Gdiplus::REAL)BALL_SIZE, (Gdiplus::REAL)BALL_SIZE), &format, &textBrush);
}

Gdiplus::Color CLottoListCtrl::GetBallColor(int number)
{
	if (number <= 10) return Gdiplus::Color(255, 255, 215, 0);
	if (number <= 20) return Gdiplus::Color(255, 30, 144, 255);
	if (number <= 30) return Gdiplus::Color(255, 220, 20, 60);
	if (number <= 40) return Gdiplus::Color(255, 50, 205, 50);
	return Gdiplus::Color(255, 169, 169, 169);
}

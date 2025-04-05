#include "stdafx.h"
#include "LottoListCtrl.h"

CLottoListCtrl::CLottoListCtrl() {}
CLottoListCtrl::~CLottoListCtrl() {}

BEGIN_MESSAGE_MAP(CLottoListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CLottoListCtrl::OnCustomDraw)
END_MESSAGE_MAP()

void CLottoListCtrl::InitializeColumns()
{
	CImageList imageList;
	imageList.Create(1, 36, ILC_COLOR32, 1, 1);
	SetImageList(&imageList, LVSIL_SMALL);
}

void CLottoListCtrl::InsertLottoRow(int nIndex, const CString& strWinsNumber, const CString& strBonus)
{
	//CString gameStr;
	//gameStr.Format(_T("%d"), index + 1);
	//InsertItem(index, gameStr);
	//SetItemText(index, 1, numbersText);
	//SetItemText(index, 2, bonusText);
	InsertItem(nIndex, _T(""));
	SetItemText(nIndex, 0, strWinsNumber);
	SetItemText(nIndex, 1, strBonus);
}

void CLottoListCtrl::InsertLottoRow(int nIndex, const CString& strWinsNumber, const CString& strBonus, const CString& strRank, const CString& strDate)
{
	InsertItem(nIndex, _T(""));
	SetItemText(nIndex, 0, strWinsNumber);
	SetItemText(nIndex, 1, strBonus);
	SetItemText(nIndex, 2, strRank);
	SetItemText(nIndex, 3, strDate);
}

CString CLottoListCtrl::GetColumnName(int nColumn)
{
	TCHAR buf[256] = { 0 };
	LVCOLUMN col;
	col.mask = LVCF_TEXT;
	col.pszText = buf;
	col.cchTextMax = 256;

	if (GetColumn(nColumn, &col))
		return CString(col.pszText);
	return _T("");
}

void CLottoListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYSUBITEMDRAW;
		break;

	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM) :
	{
		int index = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		int subItem = pLVCD->iSubItem;

		CString colName = GetColumnName(subItem);

		if (colName.CompareNoCase(_T("Numbers")) == 0 ||
			colName.CompareNoCase(_T("Bonus")) == 0)
		{
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			Gdiplus::Graphics g(pDC->GetSafeHdc());
			g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

			const bool selected = (GetItemState(index, LVIS_SELECTED) & LVIS_SELECTED) != 0;
			const bool hot = (GetHotItem() == index);

			CRect rc;
			GetSubItemRect(index, subItem, LVIR_BOUNDS, rc);

			if (colName.CompareNoCase(_T("Numbers")) == 0)
			{
				CString numbers = GetItemText(index, subItem);
				DrawBalls(g, rc, numbers, selected, hot);
			}
			else if (colName.CompareNoCase(_T("Bonus")) == 0)
			{
				CString bonus = GetItemText(index, subItem);
				DrawBonusBall(g, rc, bonus, selected);
			}

			*pResult = CDRF_SKIPDEFAULT; // 우리가 다 그렸으니 기본 드로우 생략
		}
		else
		{
			*pResult = CDRF_DODEFAULT; // 기본 텍스트 드로우 실행
		}
	}
											break;


	}
}


void CLottoListCtrl::DrawBalls(Gdiplus::Graphics& g, const CRect& rc, const CString& numbersText, bool selected, bool hot)
{
	const int BALL_SIZE = 26;
	const int SPACING = 6;

	int x = rc.left + 5;
	int y = rc.CenterPoint().y;

	int fontHeight = BALL_SIZE / 2;
	Gdiplus::Font font(DEFAULT_FONT, static_cast<Gdiplus::REAL>(fontHeight), Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
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
	if (number < 10) return Gdiplus::Color(255, 251, 196, 0);
	if (number < 20) return Gdiplus::Color(255, 105, 200, 242);
	if (number < 30) return Gdiplus::Color(255, 255, 114, 114);
	if (number < 40) return Gdiplus::Color(255, 170, 170, 170);
	return Gdiplus::Color(255, 176, 216, 64);
}

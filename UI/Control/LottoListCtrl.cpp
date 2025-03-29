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
        GetItemRect(static_cast<int>(pLVCD->nmcd.dwItemSpec), &rcItem, LVIR_BOUNDS);

        Gdiplus::Graphics graphics(pLVCD->nmcd.hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

        int number = static_cast<int>(pLVCD->nmcd.dwItemSpec) + 1;

        DrawLottoBall(graphics, rcItem, number);

        *pResult = CDRF_SKIPDEFAULT; // 기본 그리기 안 함
    }
}

void CLottoListCtrl::DrawLottoBall(Gdiplus::Graphics& g, CRect rc, int number)
{
    const int BALL_SIZE = 15;
    CPoint center = rc.CenterPoint();
    CRect ballRect(center.x - BALL_SIZE / 2, center.y - BALL_SIZE / 2, center.x + BALL_SIZE / 2, center.y + BALL_SIZE / 2);

    Gdiplus::Color ballColor = GetBallColor(number);
    Gdiplus::SolidBrush brush(ballColor);
    Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0));

    // Draw circle
    g.FillEllipse(&brush, Gdiplus::Rect(ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE));
    g.DrawEllipse(&pen, Gdiplus::Rect(ballRect.left, ballRect.top, BALL_SIZE, BALL_SIZE));

    // Draw number
    CString str;
    str.Format(_T("%d"), number);
    Gdiplus::Font font(L"Arial", 7);
    Gdiplus::StringFormat sf;
    sf.SetAlignment(Gdiplus::StringAlignmentCenter);
    sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 0, 0, 0));
    g.DrawString(str, -1, &font, Gdiplus::RectF((float)ballRect.left, (float)ballRect.top, (float)BALL_SIZE, (float)BALL_SIZE), &sf, &textBrush);
}

Gdiplus::Color CLottoListCtrl::GetBallColor(int number) const
{
    // 단순 샘플로 색상 그룹 (원하는대로 커스텀 가능)
    if (number <= 10) return Gdiplus::Color(255, 255, 200, 0);    // 노랑
    if (number <= 20) return Gdiplus::Color(255, 0, 102, 204);   // 파랑
    if (number <= 30) return Gdiplus::Color(255, 255, 0, 0);     // 빨강
    if (number <= 40) return Gdiplus::Color(255, 102, 204, 0);   // 초록
    return Gdiplus::Color(255, 128, 128, 128);                   // 회색
}

#pragma once

#include <afxcmn.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class CLottoListCtrl : public CListCtrl
{
public:
	CLottoListCtrl();
	virtual ~CLottoListCtrl();

protected:
	void DrawLottoBall(Gdiplus::Graphics& g, CRect rc, int number, bool selected);
	Gdiplus::Color GetBallColor(int number) const;

protected:
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

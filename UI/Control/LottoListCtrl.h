#pragma once
#include <afxcmn.h>
#include <gdiplus.h>
#include <vector>
#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

class CLottoListCtrl : public CListCtrl
{
public:
	CLottoListCtrl();
	virtual ~CLottoListCtrl();

protected:
	ULONG_PTR m_animationTick;

	void DrawBalls(Gdiplus::Graphics& g, CRect rc, bool selected, bool hot, const CString& numbersText, const CString& bonusText);
	Color GetBallColor(int number);

protected:
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
};
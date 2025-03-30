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

	struct LottoData
	{
		vector<int> numbers;
		int bonus;
	};

	void SetData(const vector<LottoData>& data);

protected:
	vector<LottoData> m_data;
	ULONG_PTR m_animationTick;

	void DrawBalls(Gdiplus::Graphics& g, CRect rc, const LottoData& lotto, bool selected);
	Color GetBallColor(int number);

protected:
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
};

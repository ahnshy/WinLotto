#pragma once
#include <afxcmn.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class CLottoListCtrl : public CListCtrl
{
public:
	CLottoListCtrl();
	virtual ~CLottoListCtrl();

	void InitializeColumns();

	void InsertLottoRow(int nIndex, const CString& strWinsNumber, const CString& strBonus);
	void InsertLottoRow(int nIndex, const CString& strWinsNumber, const CString& strBonus, const CString& strRank, const CString& strDate);

protected:
	void DrawBalls(Gdiplus::Graphics& g, const CRect& rc, const CString& numbersText, bool selected, bool hot);
	void DrawBonusBall(Gdiplus::Graphics& g, const CRect& rc, const CString& bonusText, bool selected);
	Gdiplus::Color GetBallColor(int number);

	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
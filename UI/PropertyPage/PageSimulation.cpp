/////////////////////////////////////////////////////////////////////////////
// PageSimulation.cpp : implementation file
//

#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include "PageSimulation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSimulation property page

IMPLEMENT_DYNCREATE(CPageSimulation, CResizablePage)

CPageSimulation::CPageSimulation() : CResizablePage(CPageSimulation::IDD)
{
	//{{AFX_DATA_INIT(CPageSimulation)
	//}}AFX_DATA_INIT

  m_psp.dwFlags &= (~PSP_HASHELP);
  m_psp.dwFlags |= PSP_USEHICON;
  HICON hIconTab = AfxGetApp()->LoadIcon( IDI_NOTE );
  m_psp.hIcon = hIconTab;
}

CPageSimulation::~CPageSimulation()
{
}

void CPageSimulation::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSimulation)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSimulation, CResizablePage)
	//{{AFX_MSG_MAP(CPageSimulation)
	//ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSimulation message handlers

BOOL CPageSimulation::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	m_gdi.Init();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPageSimulation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( HasWhiteBackground() )
	{
		pDC->SetBkMode(TRANSPARENT);
		return ::GetSysColorBrush( COLOR_WINDOW );
	}

	return  CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
}

COLORREF CPageSimulation::GetRandomColor()
{
	return RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255));
}

void CPageSimulation::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CString strText;
	float fRadios = sqrt(sqrt(double(rc.Width() * rc.Height())));
	for (INT32 nNumber = 1 ; nNumber < 50 ; nNumber++)
	{
		strText.Format(_T("%d"), nNumber);
		m_gdi.DrawBall(dc.GetSafeHdc(), RectF(10 *nNumber, 10 * ((nNumber % 10) + 10), fRadios, fRadios), GetRandomColor(), strText, TRUE);
	}
}

BOOL CPageSimulation::OnEraseBkgnd(CDC* pDC) 
{
	// TCClientDCyour message handler code here and/or call default
	//CRect rt;
	//GetClientRect(&rt);  // 클라이언트 영역의 크기 계산
	//pDC->FillSolidRect(&rt, RGB(0, 0, 0));  // 클라이언트 영역 크기만큼 흰색으로 채운다

	CRect rc;
	GetClientRect(&rc);

	//  LOGIN DIALOG - Append Ahnshy
#define		COLOR_PAGE_SIMULATION_DLG_BG_RGB						RGB(5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG							Color(255, 5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE			Color(255, 20, 77, 126)

	m_gdi.DrawGradientBackGound(pDC->GetSafeHdc(), rc, COLOR_PAGE_SIMULATION_DLG_BG_RGB, COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE, 10);


	return TRUE;  // 상위 클래스의 OnEraseBkgnd 함수를 호출하지 않아야 바꾼 배경색이 적용된다
	// return CDialog::OnEraseBkgnd(pDC);

	//return CDialog::OnEraseBkgnd(pDC);
}

void CPageSimulation::OnDestroy()
{
	CDialog::OnDestroy();
}

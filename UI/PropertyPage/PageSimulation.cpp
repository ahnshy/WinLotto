/////////////////////////////////////////////////////////////////////////////
// PageSimulation.cpp : implementation file
//

#include "stdafx.h"

#include "PageSimulation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//  LOGIN DIALOG - Append Ahnshy
#define		COLOR_PAGE_SIMULATION_DLG_BG_RGB						RGB(5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG							Color(255, 5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE			Color(255, 20, 77, 126)

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

void CPageSimulation::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	COLORREF m_bkgndColor = RGB(5, 36, 63);
	Graphics graphics(dc.GetSafeHdc());
	Bitmap mBitmap(rc.Width(), rc.Height());
	Graphics mGraphics(&mBitmap);
	mGraphics.SetSmoothingMode(SmoothingModeHighQuality);
	mGraphics.SetInterpolationMode(InterpolationModeHighQuality);

	float const nMargin = 2;
	RectF rcTarget(-nMargin, -nMargin, (float)rc.Width() + nMargin, (float)rc.Height() + nMargin);

	SolidBrush brushBk(COLOR_PAGE_SIMULATION_DLG_BG);
	mGraphics.FillRectangle(&brushBk, rcTarget);

	
//	rcTarget.Inflate(m_nFeather, m_nFeather);
	rcTarget.Inflate(25, 25);
	Color colorBackGound(255, GetRValue(m_bkgndColor), GetGValue(m_bkgndColor), GetBValue(m_bkgndColor));
	Color colorCenter(COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE);
	Color colorSurrounds[] = { colorBackGound };
	INT nColorCnt = _countof(colorSurrounds);

	GraphicsPath path;
	path.AddEllipse(rcTarget);

	PathGradientBrush brush(&path);
	brush.SetSurroundColors(colorSurrounds, &nColorCnt);
	brush.SetCenterPoint(PointF((rc.Width() / 2), (rc.Height() / 2)));
	brush.SetCenterColor(colorCenter);

	mGraphics.FillPath(&brush, &path);

	CachedBitmap cachedBitmap(&mBitmap, &graphics);
	graphics.DrawCachedBitmap(&cachedBitmap, 0, 0);
}

BOOL CPageSimulation::OnEraseBkgnd(CDC* pDC) 
{
	// TCClientDCyour message handler code here and/or call default
	//CRect rt;
	//GetClientRect(&rt);  // 클라이언트 영역의 크기 계산
	//pDC->FillSolidRect(&rt, RGB(0, 0, 0));  // 클라이언트 영역 크기만큼 흰색으로 채운다

	return TRUE;  // 상위 클래스의 OnEraseBkgnd 함수를 호출하지 않아야 바꾼 배경색이 적용된다
	// return CDialog::OnEraseBkgnd(pDC);

	//return CDialog::OnEraseBkgnd(pDC);
}

void CPageSimulation::OnDestroy()
{
	CDialog::OnDestroy();
}

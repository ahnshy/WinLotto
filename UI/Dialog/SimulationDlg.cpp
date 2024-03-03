/////////////////////////////////////////////////////////////////////////////
// PageSimulation.cpp : implementation file
//

#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include "SimulationDlg.h"

#include "../../Data/SimulationManager.h"

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
	//ON_WM_SYSCOMMAND()
	ON_WM_EXITSIZEMOVE()
	ON_WM_SIZE()
	ON_WM_TIMER()
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

	CRect rc;
	GetSimulationWndRect(rc);


	CSimulationManager *pManager = CSimulationManager::GetInstance();
	if (pManager)
	{
		pManager->Initialize(rc);
		pManager->SetBallDeployment(rc, 20);
	}

	::SetTimer(GetSafeHwnd(), 1000, 30, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSimulation::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent==1000)
	{
		CSimulationManager *pManager = CSimulationManager::GetInstance();
		if (!pManager)
			return;

		CRect rc;
		GetSimulationWndRect(rc);

		CString strText;
		double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;
		double fMargin = fDiameter;

		RectF rcBall;
		MapBalls m = pManager->GetBalls();
		for (MapBalls::iterator itor = m.begin() ; itor != m.end() ; ++itor)
		{
			rcBall = itor->second->GetRect();
			if (rcBall.X + rcBall.Width >= rc.Width() || (rcBall.X <= 0))
				itor->second->GetDirection().cx = -itor->second->GetDirection().cx;

			if (rcBall.Y + rcBall.Height >= rc.Height() || (rcBall.Y <= 0))
				itor->second->GetDirection().cy = -itor->second->GetDirection().cy;
			
			rcBall.X += itor->second->GetDirection().cx;
			rcBall.Y += itor->second->GetDirection().cy;

			itor->second->SetRect(rcBall);
		}

		InvalidateRect(NULL);
	}

	CResizablePage::OnTimer(nIDEvent);
}

//void CPageSimulation::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	CResizablePage::OnSysCommand(nID, lParam);
//}

void CPageSimulation::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	GetClientRect(rc);

	CSimulationManager *pManager = CSimulationManager::GetInstance();
	if (pManager)
	{
		pManager->AdjustBallPos(rc);
		InvalidateRect(NULL);
	}

	CResizablePage::OnSize(nType, cx, cy);
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

//void CPageSimulation::OnPaint() 
//{
//	CPaintDC dc(this);
//
//	CRect rc;
//	GetClientRect(&rc);
//
//	//  LOGIN DIALOG - Append Ahnshy
//	#define		COLOR_PAGE_SIMULATION_DLG_BG_RGB						RGB(5, 36, 63)
//	#define		COLOR_PAGE_SIMULATION_DLG_BG							Color(255, 5, 36, 63)
//	#define		COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE			Color(255, 20, 77, 126)
//
//	m_gdi.DrawGradientBackGound(dc.GetSafeHdc(), rc, COLOR_PAGE_SIMULATION_DLG_BG_RGB, COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE, 10);
//
//	CSimulationManager *pManager = CSimulationManager::GetInstance();
//	if (!pManager)
//		return;
//
//	//CRect rc;
//	//GetClientRect(&rc);
//
//	CString strText;
//	double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;
//	double fMargin = fDiameter;
//
//	MapBalls m = pManager->GetBalls();
//	for (MapBalls::iterator itor = m.begin() ; itor != m.end() ; ++itor)
//	{
//		strText.Format(_T("%d"), itor->first);
//		//itor->second.SetRect(RectF((((itor->first % 10)*fDiameter) +itor->first +fMargin), fMargin + (((itor->first / 10)) * fDiameter*2), fDiameter, fDiameter));
//		m_gdi.DrawBall(dc.GetSafeHdc(), itor->second->GetRect(), itor->second->GetColor(), strText, TRUE);
//	}
//}

void CPageSimulation::OnPaint() 
{
	
	CPaintDC dc(this);
	//GetClientRect(&rc);
	//dc.FillSolidRect(&rc, RGB(255, 255, 255));

	CRect rc;
	GetSimulationWndRect(rc);

	//  LOGIN DIALOG - Append Ahnshy
#define		COLOR_PAGE_SIMULATION_DLG_BG_RGB						RGB(5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG							Color(255, 5, 36, 63)
#define		COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE			Color(255, 20, 77, 126)

	m_gdi.BufferDrawGradientBackGound(rc, COLOR_PAGE_SIMULATION_DLG_BG_RGB, COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE, 10);

	CSimulationManager *pManager = CSimulationManager::GetInstance();
	if (!pManager)
		return;

	//CRect rc;
	//GetClientRect(&rc);

	CString strText;
	double fDiameter = (sqrt(double(rc.Width() * rc.Height()))) * 0.06;
	double fMargin = fDiameter;

	MapBalls m = pManager->GetBalls();
	for (MapBalls::iterator itor = m.begin() ; itor != m.end() ; ++itor)
	{
		strText.Format(_T("%d"), itor->first);
		//itor->second.SetRect(RectF((((itor->first % 10)*fDiameter) +itor->first +fMargin), fMargin + (((itor->first / 10)) * fDiameter*2), fDiameter, fDiameter));
		m_gdi.BufferDrawBall(itor->second->GetRect(), itor->second->GetColor(), strText, TRUE);
	}

	m_gdi.BufferBitblt(dc.GetSafeHdc(), rc);
}

BOOL CPageSimulation::OnEraseBkgnd(CDC* pDC) 
{
	//CRect rc;
	//GetClientRect(&rc);
	//pDC->FillSolidRect(&rc, RGB(255, 255, 255));

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CPageSimulation::OnDestroy()
{
	CSimulationManager *pManager = CSimulationManager::GetInstance();
	if (pManager)
		pManager->DestroyInstance();

	CResizablePage::OnDestroy();
}

INT32 CPageSimulation::GetSimulationWndRect(CRect& rc)
{
	GetClientRect(&rc);
	//rc.top += 50;
	//rc.left += 30;

	return 1;
}

void CPageSimulation::OnExitSizeMove()
{
	CResizablePage::OnExitSizeMove();
}
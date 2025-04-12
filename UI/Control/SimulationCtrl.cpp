/////////////////////////////////////////////////////////////////////////////
// SimulationCtrl.cpp : implementation file
//

#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include "SimulationCtrl.h"

#include "../../Data/SimulationManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimulationCtrl property page

IMPLEMENT_DYNCREATE(CSimulationCtrl, CDialogEx)

CSimulationCtrl::CSimulationCtrl() : CDialogEx(CSimulationCtrl::IDD)
{
	//{{AFX_DATA_INIT(CSimulationCtrl)
	//}}AFX_DATA_INIT

  //m_psp.dwFlags &= (~PSP_HASHELP);
  //m_psp.dwFlags |= PSP_USEHICON;
  //HICON hIconTab = AfxGetApp()->LoadIcon( IDI_NOTE );
  //m_psp.hIcon = hIconTab;
}

CSimulationCtrl::~CSimulationCtrl()
{
}

void CSimulationCtrl::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimulationCtrl)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimulationCtrl, CDialogEx)
	//{{AFX_MSG_MAP(CSimulationCtrl)
	//ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulationCtrl message handlers

BOOL CSimulationCtrl::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	m_gdi.Init();

	CRect rc;
	GetClientRect(&rc);

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

void CSimulationCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	GetClientRect(rc);

	CSimulationManager *pManager = CSimulationManager::GetInstance();
	if (pManager)
	{
		pManager->AdjustBallPos(rc);
		InvalidateRect(NULL);
	}

	CDialogEx::OnSize(nType, cx, cy);
}

HBRUSH CSimulationCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//if( HasWhiteBackground() )
	{
		pDC->SetBkMode(TRANSPARENT);
		return ::GetSysColorBrush( COLOR_WINDOW );
	}

	return  CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

COLORREF CSimulationCtrl::GetRandomColor()
{
	return RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255));
}

void CSimulationCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	m_gdi.BufferDrawGradientBackGound(rc, COLOR_PAGE_SIMULATION_DLG_BG_RGB, COLOR_PAGE_SIMULATION_DLG_BG_HIGHLIGHT_CIRCLE, 8);

	float centerX = rc.Width() / 2.0f;
	float centerY = rc.Height() / 2.0f;
	PointF containerCenter(centerX, centerY);
	float baseRadius = min(rc.Width(), rc.Height()) / 2.0f - 10.0f;
	float containerRadius = baseRadius * 0.5f;

	m_gdi.BufferDrawContainer(rc, containerCenter, containerRadius);
	CSimulationManager *pManager = CSimulationManager::GetInstance();
	if (!pManager)
		return;

	CString strText;
	auto& balls = pManager->GetBalls();
	for (auto& kv : balls)
	{
		DWORD number = kv.first;
		CBallItem* pBall = kv.second;
		if (!pBall) continue;

		strText.Format(_T("%d"), number);
		m_gdi.BufferDrawBall(pBall->GetRect(), pBall->GetColor(), strText, TRUE);
	}

	m_gdi.BufferBitblt(dc.GetSafeHdc(), rc);
}

BOOL CSimulationCtrl::OnEraseBkgnd(CDC* pDC)
{
	//CRect rc;
	//GetClientRect(&rc);
	//pDC->FillSolidRect(&rc, RGB(255, 255, 255));

	return TRUE; 
	//return CDialog::OnEraseBkgnd(pDC);
}

void CSimulationCtrl::OnDestroy()
{
	CDialogEx::OnDestroy();
}

INT32 CSimulationCtrl::GetSimulationWndRect(CRect& rc)
{
	GetClientRect(&rc);
	return 1;
}

void CSimulationCtrl::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1000)
	{
		CSimulationManager* pManager = CSimulationManager::GetInstance();
		if (!pManager) return;

		CRect rc;
		GetClientRect(&rc);

		float baseRadius = min(rc.Width(), rc.Height()) / 2.f - 10.f;
		float containerRadius = baseRadius * 0.5f;

		float centerX = rc.Width() / 2.f;
		float centerY = rc.Height() / 2.f;

		auto& balls = pManager->GetBalls();
		for (auto& kv : balls)
		{
			CBallItem* pBall = kv.second;
			if (!pBall) continue;

			RectF ballRect = pBall->GetRect();
			PointF vel = pBall->GetVelocity();

			vel.Y += GRAVITY;
			ballRect.X += vel.X;
			ballRect.Y += vel.Y;

			float radius = ballRect.Width / 2.f;
			float cx = ballRect.X + radius;
			float cy = ballRect.Y + radius;

			float dx = cx - centerX;
			float dy = cy - centerY;
			float dist = sqrtf(dx * dx + dy * dy);

			if (dist + radius > containerRadius)
			{
				float nx = dx / dist;
				float ny = dy / dist;

				float overlap = (dist + radius) - containerRadius;
				cx -= nx * overlap;
				cy -= ny * overlap;
				ballRect.X = cx - radius;
				ballRect.Y = cy - radius;

				float dot = vel.X * nx + vel.Y * ny;
				vel.X -= 2.f * dot * nx;
				vel.Y -= 2.f * dot * ny;

				vel.X *= DAMPING;
				vel.Y *= DAMPING;
			}
			pBall->SetRect(ballRect);
			pBall->SetVelocity(vel);
		}
		Invalidate(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}
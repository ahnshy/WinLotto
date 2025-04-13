﻿/////////////////////////////////////////////////////////////////////////////
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
	m_fCircleRatio = 0.9f;
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
	float containerRadius = baseRadius * 0.9f;

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
		float containerRadius = baseRadius * 0.9f;
		PointF center((float)rc.Width() / 2.f, (float)rc.Height() / 2.f);

		auto& balls = pManager->GetBalls();

		// 1. 공별 개별 처리
		for (auto& kv : balls)
		{
			CBallItem* pBall = kv.second;
			if (!pBall) continue;

			// Sleep된 공은 건너뜀
			if (pBall->IsSleeping())
				continue;

			RectF r = pBall->GetRect();
			PointF v = pBall->GetVelocity();

			// 중력 및 마찰 적용
			v.Y += 0.25f;
			v.X *= 0.99f;
			v.Y *= 0.99f;

			// 이동
			r.X += v.X;
			r.Y += v.Y;

			// 느리면 Sleep 처리
			if (fabs(v.X) < 0.05f && fabs(v.Y) < 0.05f)
			{
				if (fabs(v.X) + fabs(v.Y) < 0.08f)
				{
					v.X = 0.f;
					v.Y = 0.f;
					pBall->SetSleeping(true);  // ✅ Sleep 처리
				}
			}

			// 원 경계 충돌
			PointF c(r.X + r.Width / 2.f, r.Y + r.Height / 2.f);
			float dx = c.X - center.X;
			float dy = c.Y - center.Y;
			float dist = sqrtf(dx * dx + dy * dy);
			float radius = r.Width / 2.f;

			if (dist + radius >= containerRadius)
			{
				float nx = dx / dist;
				float ny = dy / dist;
				float overlap = (dist + radius - containerRadius) + 0.5f;

				c.X -= nx * overlap;
				c.Y -= ny * overlap;
				r.X = c.X - radius;
				r.Y = c.Y - radius;

				float dot = v.X * nx + v.Y * ny;
				v.X -= 2.f * dot * nx;
				v.Y -= 2.f * dot * ny;

				v.X *= 0.8f;
				v.Y *= 0.8f;

				// 느리면 Sleep 처리
				if (fabs(v.X) < 0.05f && fabs(v.Y) < 0.05f)
				{
					v.X = 0.f;
					v.Y = 0.f;
					pBall->SetSleeping(true);
				}
			}

			pBall->SetRect(r);
			pBall->SetVelocity(v);
		}

		// 2. 공-공 충돌 처리
		const float restitution = 0.9f;
		for (auto it1 = balls.begin(); it1 != balls.end(); ++it1)
		{
			for (auto it2 = std::next(it1); it2 != balls.end(); ++it2)
			{
				CBallItem* b1 = it1->second;
				CBallItem* b2 = it2->second;
				if (!b1 || !b2) continue;

				RectF r1 = b1->GetRect();
				RectF r2 = b2->GetRect();

				PointF c1(r1.X + r1.Width / 2.f, r1.Y + r1.Height / 2.f);
				PointF c2(r2.X + r2.Width / 2.f, r2.Y + r2.Height / 2.f);

				float dx = c2.X - c1.X;
				float dy = c2.Y - c1.Y;
				float dist = sqrtf(dx * dx + dy * dy);
				float minDist = r1.Width / 2.f + r2.Width / 2.f;

				if (dist < minDist && dist > 0.001f)
				{
					float nx = dx / dist;
					float ny = dy / dist;
					float overlap = (minDist - dist) / 2.f;

					// 위치 보정
					r1.X -= nx * overlap;
					r1.Y -= ny * overlap;
					r2.X += nx * overlap;
					r2.Y += ny * overlap;

					PointF v1 = b1->GetVelocity();
					PointF v2 = b2->GetVelocity();

					float v1n = v1.X * nx + v1.Y * ny;
					float v2n = v2.X * nx + v2.Y * ny;
					float p = (2.f * (v1n - v2n)) / 2.f;

					v1.X -= p * nx * restitution;
					v1.Y -= p * ny * restitution;
					v2.X += p * nx * restitution;
					v2.Y += p * ny * restitution;

					b1->SetRect(r1);
					b2->SetRect(r2);
					b1->SetVelocity(v1);
					b2->SetVelocity(v2);

					// 충돌했으니 둘 다 깨움
					b1->SetSleeping(false);
					b2->SetSleeping(false);
				}
			}
		}

		Invalidate(FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}

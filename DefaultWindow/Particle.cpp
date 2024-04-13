#include "stdafx.h"
#include "Particle.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CParticle::CParticle()
	: m_fAngle(0), m_fMoveDist(0)
{
}

CParticle::~CParticle()
{
	Release();
}

void CParticle::Initialize()
{
	m_tInfo = {0, 0, 4, 4};
	m_fSpeed = 3.f;
	m_eObjID = OBJ_EFFECT;
	m_eRenderID = RENDER_EFFECT;
	m_eDir = CObjMgr::Get_Instance()->Get_Player()->Get_Dir();
}

int CParticle::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CParticle::Late_Update()
{
	m_fMoveTime += 0.1f;

	if (m_eDir == RIGHT)
	{
		m_tInfo.fX += cosf(m_fAngle * PI / 180.f);
		m_tInfo.fY += (sin(m_fAngle * PI / 180.f)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) *
			0.3f;
	}

	else if (m_eDir == LEFT)
	{
		m_tInfo.fX -= cosf(m_fAngle * PI / 180.f);
		m_tInfo.fY += (sin(m_fAngle * PI / 180.f)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) *
			0.3f;
	}

	else if (m_eDir == UP)
	{
		m_tInfo.fY -= 1.f;
		m_tInfo.fX -= (sin(m_fAngle)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime) * 0.3f;
	}

	else if (m_eDir == DOWN)
	{
		m_tInfo.fY += 1.f;
		m_tInfo.fX -= (sin(m_fAngle)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime) * 0.3f;
	}

	m_fMoveDist += abs((sin(m_fAngle)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime) * 0.3f);

	if (m_fMoveDist >= 50.f)
		m_bDead = true;

	if (m_fMoveTime >= 3.f)
		m_bDead = true;
}

void CParticle::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  4, 4, hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, MAGENTA);
}

void CParticle::Release()
{
}

#include "stdafx.h"
#include "Head.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

void CHead::Initialize()
{
	m_fSpeed = 3.f;
	m_eObjID = OBJ_EFFECT;
	m_eRenderID = RENDER_EFFECT;
	m_eDir = CObjMgr::Get_Instance()->Get_Player()->Get_Dir();
}

int CHead::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CHead::Late_Update()
{
	Move();
	if (m_fMoveTime >= 5.f)
		m_bDead = true;
}

void CHead::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC, 0, 0,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CHead::Release()
{
}

void CHead::Move()
{
	if (m_eDir == LEFT)
	{
		m_fMoveTime += 0.1f;
		m_tInfo.fX += cosf(100.f * (PI / 180.f)) * m_fSpeed * m_fMoveTime;
		m_tInfo.fY -= sinf(100.f * (PI / 180.f)) * m_fSpeed * m_fMoveTime
			- (0.5f * 2.f * m_fMoveTime * m_fMoveTime);
	}
	else if (m_eDir == RIGHT)
	{
		m_fMoveTime += 0.1f;
		m_tInfo.fX += cosf(80.f * (PI / 180.f)) * m_fSpeed * m_fMoveTime;
		m_tInfo.fY -= sinf(80.f * (PI / 180.f)) * m_fSpeed * m_fMoveTime
			- (0.5f * 2.f * m_fMoveTime * m_fMoveTime);
	}
	else
	{
		m_fMoveTime += 0.1f;
		m_tInfo.fX += cosf(90.f * (PI / 180.f)) * m_fSpeed * m_fMoveTime;
		m_tInfo.fY -= sinf(90.f * (PI / 180.f)) * m_fSpeed * m_fMoveTime
			- (0.5f * 2.f * m_fMoveTime * m_fMoveTime);
	}
}

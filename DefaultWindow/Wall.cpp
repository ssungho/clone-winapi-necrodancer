#include "stdafx.h"
#include "Wall.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CWall::CWall()
{
}

CWall::~CWall()
{
	Release();
}

void CWall::Initialize()
{
	m_eObjID = OBJ_WALL;
	m_tInfo = {0.f, 0.f, 48.f, 96.f};
	m_eWallID = NORMAL;
	m_pFrameKey = L"Boss_Wall";
	m_eRenderID = RENDER_GAME_OBJECT;
	m_iHp = 1;
	m_pParticleKey = L"Particle_Dirt";
}

int CWall::Update()
{
	if (m_bDead)
	{
		Draw_Particle(m_pParticleKey);
		return OBJ_DEAD;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CWall::Late_Update()
{
	if (m_eWallID == STONE)
	{
		if (m_iHp < 2)
			m_pFrameKey = L"Cracked_Stone_Wall";
	}

	if (m_eWallID == SHOP)
	{
		if (m_iHp < 3)
			m_pFrameKey = L"Cracked_Shop_Wall";
	}
}

void CWall::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY - 10, // 테스트중
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CWall::Release()
{
}

void CWall::Motion_Change()
{
}

void CWall::Create_Particle()
{
}

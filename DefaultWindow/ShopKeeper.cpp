#include "stdafx.h"
#include "ShopKeeper.h"

#include "BmpMgr.h"
#include "ScrollMgr.h"

CShopKeeper::CShopKeeper()
{
	// 94 76 8
}

CShopKeeper::~CShopKeeper()
{
}

void CShopKeeper::Initialize()
{
	m_tInfo = {0, 0, 94.f, 76.f};
	m_pFrameKey = L"Shopkeeper";
	m_fSpeed = 3.0f;
	m_iMaxHp = 9;
	m_iHp = 9;
	m_iAttack = 10; // 0.5 ¶ó´Â ¶æ
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_pParticleKey = L"Particle_Blood";
	m_tFrame = {0, 7, 0, (unsigned long)522 / 2, GetTickCount()};

	Set_Heart(m_iMaxHp);
}

int CShopKeeper::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();
	Update_Heart();

	return OBJ_NOEVENT;
}

void CShopKeeper::Late_Update()
{
	__super::Move_Frame();
}

void CShopKeeper::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY - 24,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
	Render_Heart(hDC);
}

void CShopKeeper::Release()
{
}

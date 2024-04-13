#include "stdafx.h"
#include "Trap.h"

#include "BmpMgr.h"
#include "ScrollMgr.h"

CTrap::CTrap()
{
}

CTrap::~CTrap()
{
	Release();
}

void CTrap::Initialize()
{
	m_tInfo = {0, 0, 28, 32};
	m_tFrame = {0, 1, 0, 50, GetTickCount()};
	m_eObjID = OBJ_TRAP;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eTrapID = TRAP_BOUNCE;
}

int CTrap::Update()
{
	__super::Update_Rect();
	return 0;
}

void CTrap::Late_Update()
{
	if (m_bOnTrap)
	{
		Play_Trap_Anim();
	}
}

void CTrap::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CTrap::Release()
{
}

void CTrap::Play_Trap_Anim()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
			m_bOnTrap = false;
		}
	}
}

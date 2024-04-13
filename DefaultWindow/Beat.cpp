#include "stdafx.h"
#include "Beat.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CBeat::CBeat()
	: m_bOnHeart(false), m_bDisplay(true), m_bSuccess(false)
{
}

CBeat::~CBeat()
{
	Release();
}

void CBeat::Initialize()
{
	m_tInfo = {0, 0, 12.f, 64.f};
	m_pFrameKey = L"Beat_Marker";
	m_eObjID = OBJ_BEAT;
	m_eRenderID = RENDER_UI;
	m_fSpeed = 5.f; // 일단 고정... 4.17
}

int CBeat::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eDir == RIGHT)
	{
		m_tInfo.fX += m_fSpeed;
	}
	else
	{
		m_tInfo.fX -= m_fSpeed;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBeat::Late_Update()
{
}

void CBeat::Render(HDC hDC)
{
	if (!m_bDisplay)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left, m_tRect.top,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC, 0, 0,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CBeat::Release()
{
}

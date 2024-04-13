#include "stdafx.h"
#include "Heart.h"

#include "BmpMgr.h"
#include "ScrollMgr.h"

CHeart::CHeart()
{
}

CHeart::~CHeart()
{
	Release();
}

void CHeart::Initialize()
{
	// 24, 22
	m_tInfo = {0.f, 0.f, 24.f, 24.f};
	m_pFrameKey = L"UI_Small_Heart";
	m_eObjID = OBJ_UI;
	m_eRenderID = RENDER_UI;
}

int CHeart::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CHeart::Late_Update()
{
}

void CHeart::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	auto iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	auto iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  0,
	                  0,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CHeart::Release()
{
}

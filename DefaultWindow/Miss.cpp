#include "stdafx.h"
#include "Miss.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CMiss::CMiss()
	: m_dwTime(GetTickCount())
{
}

CMiss::~CMiss()
{
	Release();
}

void CMiss::Initialize()
{
	m_tInfo = {0.f, 0.f, 36.f, 13.f};
	m_pFrameKey = L"Text_Miss";
	m_eObjID = OBJ_UI;
	m_eRenderID = RENDER_UI;
	m_fMoveTime = 0;
}

int CMiss::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fMoveTime += 1.2f;
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMiss::Late_Update()
{
	if (m_dwTime + m_iBpm * 1.5f < GetTickCount())
		m_bDead = true;
}

void CMiss::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left, m_tRect.top - m_fMoveTime,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  0, 0,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  RGB(255, 0, 255));
}

void CMiss::Release()
{
}

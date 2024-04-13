#include "stdafx.h"
#include "Text.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CText::CText()
	: m_dwTime(GetTickCount())
{
}

CText::~CText()
{
	Release();
}

void CText::Initialize()
{
	m_eObjID = OBJ_UI;
	m_eRenderID = RENDER_UI;
	m_fMoveTime = 0;
}

int CText::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	m_fMoveTime += 1.2f;
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CText::Late_Update()
{
	if (m_dwTime + m_iBpm * 1.5f < GetTickCount())
		m_bDead = true;
}

void CText::Render(HDC hDC)
{
	auto iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	auto iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HFONT font = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
	                        VARIABLE_PITCH | FF_ROMAN, L"»ﬁ∏’µ’±Ÿ«ÏµÂ∂Û¿Œ");
	SelectObject(hDC, font);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));

	TextOut(hDC, m_tInfo.fX + iScrollX - 32, m_tInfo.fY + iScrollY - 10, m_pFrameKey, lstrlen(m_pFrameKey));
}

void CText::Release()
{
}

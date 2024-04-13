#include "stdafx.h"
#include "BeatHeart.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "ScrollMgr.h"

CBeatHeart::CBeatHeart()
	: m_iBeatCnt(0), m_bOnFrame(false), m_dwBeatTimer(0)
{
}

CBeatHeart::~CBeatHeart()
{
	Release();
}

void CBeatHeart::Initialize()
{
	m_tInfo = {WINCX / 2.f, 500.f, 82.f, 104.f};
	m_pFrameKey = L"Beat_Heart";
	m_tFrame = {0, 1, 0, 30, GetTickCount()};
	m_eObjID = OBJ_UI;
	m_eRenderID = RENDER_BEAT_HEART;
}

int CBeatHeart::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBeatHeart::Late_Update()
{
	if (m_bOnFrame)
	{
		if (m_dwBeatTimer + 50 < GetTickCount())
		{
			Move_Frame();
			m_bOnFrame = false;
			m_dwBeatTimer = 0;
		}
	}
}

void CBeatHeart::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  WINCX / 2 - (int)(m_tInfo.fCX / 2.f),
	                  HEART_POSY - (int)(m_tInfo.fCX / 2.f),
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);

	HFONT font = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
	                        VARIABLE_PITCH | FF_ROMAN, L"휴먼둥근헤드라인");
	const int iCombo = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Combo();
	if (iCombo >= 5)
	{
		SelectObject(hDC, font);
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));
		// TODO
		TextOut(hDC, WINCX / 2 - 45, HEART_POSY + 65, L"코인 배수: 2", lstrlen(L"코인 배수: 2"));
	}
}

void CBeatHeart::Release()
{
}

void CBeatHeart::Heart_Beat()
{
	Move_Frame();
	m_dwBeatTimer = GetTickCount();
	m_bOnFrame = true;
}

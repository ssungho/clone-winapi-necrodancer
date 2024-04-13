#include "stdafx.h"
#include "GreenSlime.h"

#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"

CGreenSlime::CGreenSlime()
{
}

CGreenSlime::~CGreenSlime()
{
	Release();
}

void CGreenSlime::Initialize()
{
	m_tInfo = {0, 0, 52.f, 54.f};
	m_pFrameKey = L"Slime_Green_Left";
	m_fSpeed = 3.0f;
	m_eDir = LEFT;
	m_eCurState = IDLE;
	m_iMaxHp = 1;
	m_iHp = 1;
	m_iAttack = 0;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 1;
	m_iAlphaPosY = -18;
	m_pParticleKey = L"Particle_Green";
	Set_Heart(m_iMaxHp);
}

int CGreenSlime::Update()
{
	if (m_bDead)
	{
		Draw_Particle();
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_slime_death_01.ogg", SOUND_MONSTER_DEAD, 0.5f);
		Leave();
		return OBJ_DEAD;
	}

	if (m_iBeatCount >= m_iNeedToDoAct)
	{
		Do_Act();
		m_iBeatCount = 0;
	}

	Update_Rect();
	Update_Heart();

	return OBJ_NOEVENT;
}

void CGreenSlime::Late_Update()
{
	Move();
	Motion_Change();
	Move_Frame();
}

void CGreenSlime::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY + m_iAlphaPosY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
	Render_Heart(hDC);
}

void CGreenSlime::Release()
{
}

void CGreenSlime::Do_Act()
{
	if (!m_bMove)
	{
		m_bMove = true;
		m_fPreYPos = m_tInfo.fY;
		m_eCurState = MOVE;
	}
}

void CGreenSlime::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eDir)
		{
		case LEFT:
			m_pFrameKey = L"Slime_Green_Left";
			m_tFrame = FRAME{0, 3, 0, (DWORD)m_iBpm / 4, GetTickCount()};
			break;

		case RIGHT:
			m_pFrameKey = L"Slime_Green_Right";
			m_tFrame = FRAME{0, 3, 0, (DWORD)m_iBpm / 4, GetTickCount()};
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CGreenSlime::Move()
{
	if (m_bMove)
	{
		m_fMoveTime += 0.5f;
		m_tInfo.fY -= static_cast<float>(sin(45)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) * 0.5f;

		if (m_fPreYPos < m_tInfo.fY)
		{
			m_bMove = false;
			m_tInfo.fY = m_fPreYPos;
			m_fMoveTime = 0.f;
			m_eCurState = IDLE;
		}
	}
}

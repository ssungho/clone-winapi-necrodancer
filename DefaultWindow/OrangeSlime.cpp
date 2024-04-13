#include "stdafx.h"
#include "OrangeSlime.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"

COrangeSlime::COrangeSlime()
{
}

COrangeSlime::~COrangeSlime()
{
	Release();
}

void COrangeSlime::Initialize()
{
	m_tInfo = {0, 0, 52.f, 54.f};
	m_pFrameKey = L"Slime_Orange_Left";
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 1;
	m_iHp = 1;
	m_iAttack = 1;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 1;
	m_iAlphaPosY = -18;
	m_pParticleKey = L"Particle_Orange";
	Set_Heart(m_iMaxHp);
}

int COrangeSlime::Update()
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

void COrangeSlime::Late_Update()
{
	Move();
	Motion_Change();
	Move_Frame();
}

void COrangeSlime::Render(HDC hDC)
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

void COrangeSlime::Release()
{
}

void COrangeSlime::Do_Act()
{
	Patrol();
}

void COrangeSlime::Move()
{
	if (m_bMove)
	{
		m_fMoveTime += 0.5f;

		if (m_eDir == RIGHT)
		{
			m_tInfo.fX += 4.8f;
			m_tInfo.fY -= static_cast<float>(sin(45)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) *
				0.5f;

			if (m_fPreYPos < m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos;
				m_fMoveTime = 0.f;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(this));
				m_eCurState = IDLE;
			}
		}

		else if (m_eDir == LEFT)
		{
			m_tInfo.fX -= 4.8f;
			m_tInfo.fY -= static_cast<float>(sin(45)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) *
				0.5f;

			if (m_fPreYPos < m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos;
				m_fMoveTime = 0.f;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(this));
				m_eCurState = IDLE;
			}
		}

		else if (m_eDir == UP)
		{
			m_tInfo.fY -= 4.8f;
			if (m_fPreYPos - TILECY > m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos - TILECY;
				m_fMoveTime = 0.f;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(this));
				m_eCurState = IDLE;
			}
		}

		else if (m_eDir == DOWN)
		{
			m_tInfo.fY += 4.8f;
			if (m_fPreYPos + TILECY < m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos + TILECY;
				m_fMoveTime = 0.f;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(this));
				m_eCurState = IDLE;
			}
		}
	}
}

void COrangeSlime::Patrol()
{
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pMyTile = pTileMgr->Get_Tile(this);

	DIRECTION eNextDir = DIR_END;
	switch (m_eDir)
	{
	case UP: eNextDir = RIGHT;
		break;
	case LEFT: eNextDir = UP;
		break;
	case DOWN: eNextDir = LEFT;
		break;
	case RIGHT: eNextDir = DOWN;
		break;
	}

	auto pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(
		m_tInfo.fX + static_cast<float>(iDx[static_cast<int>(eNextDir)] * TILECX),
		m_tInfo.fY + static_cast<float>(iDy[static_cast<int>(eNextDir)] * TILECY)));

	auto pOwner = pDestTile->Get_Owner();
	if (pOwner == nullptr)
	{
		m_eDir = eNextDir;

		if (!m_bMove)
		{
			m_bMove = true;
			m_fPreYPos = m_tInfo.fY;

			static_cast<CTile*>(pMyTile)->Leave_Here();
			pDestTile->Set_Owner(this);
			m_eCurState = MOVE;
		}
	}
	else if (pOwner->Get_ObjID() == OBJ_PLAYER)
	{
		CObj* pEffect = nullptr;
		TCHAR* pKey = L"";

		switch (eNextDir)
		{
		case LEFT:
			pKey = L"Attack_Enemy_Left";
			pEffect = CAbstractFactory<CEffect>::Create(pKey, 54.f, 48.f, pDestTile, 50);
			break;

		case RIGHT:
			pKey = L"Attack_Enemy_Right";
			pEffect = CAbstractFactory<CEffect>::Create(pKey, 54.f, 48.f, pDestTile, 50);
			break;

		case UP:
			pKey = L"Attack_Enemy_Up";
			pEffect = CAbstractFactory<CEffect>::Create(pKey, 48.f, 54.f, pDestTile, 50);
			break;

		case DOWN:
			pKey = L"Attack_Enemy_Down";
			pEffect = CAbstractFactory<CEffect>::Create(pKey, 48.f, 54.f, pDestTile, 50);
			break;
		}

		static_cast<CEffect*>(pEffect)->Set_EffectType(EFFECT_MONSTER_ATTACK);
		pEffect->Set_Frame({0, 4, 0, (DWORD)50, GetTickCount()});
		pEffect->Set_Dir(eNextDir);

		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
		CObjMgr::Get_Instance()->Player_Damaged(m_iAttack);

		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_ATTACK);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_slime_attack.ogg", SOUND_MONSTER_ATTACK, 0.5f);
	}
}

void COrangeSlime::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eDir)
		{
		case LEFT:
		case DOWN:
			m_pFrameKey = L"Slime_Orange_Left";
			m_tFrame = FRAME{0, 3, 0, (DWORD)m_iBpm / 4, GetTickCount()};
			break;

		case RIGHT:
		case UP:
			m_pFrameKey = L"Slime_Orange_Right";
			m_tFrame = FRAME{0, 3, 0, (DWORD)m_iBpm / 4, GetTickCount()};
			break;
		}

		m_ePreState = m_eCurState;
	}
}

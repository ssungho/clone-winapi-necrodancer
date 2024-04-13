#include "stdafx.h"
#include "Zombie.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"

// 이친구는 방향을 지정해줘야함. // BUG
CZombie::CZombie()
{
}

CZombie::~CZombie()
{
	Release();
}


void CZombie::Initialize()
{
	m_tInfo = {0, 0, 48.f, 50.f};
	m_pFrameKey = L"Zombie_Left";
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 1;
	m_iHp = 1;
	m_iAttack = 2;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 2;
	m_iAlphaPosY = -18;
	m_pParticleKey = L"Particle_Yellow";
	Set_Heart(m_iMaxHp);
}

int CZombie::Update()
{
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_zombie_death.ogg", SOUND_MONSTER_DEAD, 0.5f);
		Draw_Particle();
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

void CZombie::Late_Update()
{
	Move();
	Motion_Change();
	__super::Move_Frame();
}

void CZombie::Render(HDC hDC)
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

void CZombie::Release()
{
}

void CZombie::Do_Act()
{
	Patrol();
}

void CZombie::Move()
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

void CZombie::Patrol()
{
	if (m_bMove) return;

	auto pTileMgr = CTileMgr::Get_Instance();
	auto pMyTile = pTileMgr->Get_Tile(this);
	CTile* pDestTile = nullptr;
	CTile* pOffSideTile = nullptr;

	DIRECTION eOffSideDir = DIR_END;
	switch (m_eDir)
	{
	case UP:
		pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX, m_tInfo.fY - TILECY));
		pOffSideTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX, m_tInfo.fY + TILECY));
		eOffSideDir = DOWN;
		break;

	case DOWN:
		pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX, m_tInfo.fY + TILECY));
		pOffSideTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX, m_tInfo.fY - TILECY));
		eOffSideDir = UP;
		break;

	case LEFT:
		pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX - TILECX, m_tInfo.fY));
		pOffSideTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX + TILECX, m_tInfo.fY));
		eOffSideDir = RIGHT;
		break;

	case RIGHT:
		pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX + TILECX, m_tInfo.fY));
		pOffSideTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(m_tInfo.fX - TILECX, m_tInfo.fY));
		eOffSideDir = LEFT;
		break;
	}

	assert(pDestTile);

	auto pOwner = pDestTile->Get_Owner();

	if (pOwner)
	{
		auto eID = pOwner->Get_ObjID();

		if (eID == OBJ_PLAYER)
		{
			CObj* pEffect = nullptr;
			TCHAR* pKey = L"";
			switch (m_eDir)
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
			pEffect->Set_Dir(m_eDir);

			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
			CObjMgr::Get_Instance()->Player_Damaged(m_iAttack);
		}
		else if (eID == OBJ_MONSTER ||
			eID == OBJ_WALL ||
			eID == OBJ_ITEM)
		{
			m_eDir = eOffSideDir;

			if (!m_bMove)
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}
			static_cast<CTile*>(pMyTile)->Leave_Here();
			pOffSideTile->Set_Owner(this); // 원래 DestTile
			m_eCurState = MOVE;
		}
	}
	else
	{
		if (!m_bMove)
		{
			m_bMove = true;
			m_fPreYPos = m_tInfo.fY;
		}
		static_cast<CTile*>(pMyTile)->Leave_Here();
		pDestTile->Set_Owner(this);
		m_eCurState = MOVE;
	}
}

void CZombie::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		auto pLKey = L"Zombie_Left";
		auto pRKey = L"Zombie_Right";
		auto pUKey = L"Zombie_Up";
		auto pDKey = L"Zombie_Down";

		if (m_eCurState == IDLE)
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;

			case DOWN:
				m_pFrameKey = pDKey;
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};


			case UP:
				m_pFrameKey = pUKey;
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;
			}
		}

		else if (m_eCurState == MOVE)
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{4, 7, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{4, 7, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;

			case DOWN:
				m_pFrameKey = pDKey;
				m_tFrame = FRAME{4, 7, 0, (unsigned long)m_iBpm / 2, GetTickCount()};

			case UP:
				m_pFrameKey = pUKey;
				m_tFrame = FRAME{4, 7, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;
			}
		}

		m_ePreState = m_eCurState;
	}
}

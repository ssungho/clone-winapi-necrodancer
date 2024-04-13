#include "stdafx.h"
#include "GreenDragon.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "Wall.h"

CGreenDragon::CGreenDragon()
{
}

CGreenDragon::~CGreenDragon()
{
	Release();
}

void CGreenDragon::Initialize()
{
	m_tInfo = {0, 0, 108, 102.f};
	m_pFrameKey = L"Dragon_Green_Left";
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 4;
	m_iHp = 4;
	m_iAttack = 2;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 2;
	m_iAlphaPosY = 6 - TILECY; // TODO : �����ؾߵ�.
	m_pParticleKey = L"Particle_Green";
	Set_Heart(m_iMaxHp);
}

int CGreenDragon::Update()
{
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_dragon_death.ogg", SOUND_MONSTER_DEAD, 0.5f);
		Draw_Particle();
		Drop_Item();
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

void CGreenDragon::Late_Update()
{
	Move();
	Motion_Change();
	Move_Frame();
}

void CGreenDragon::Render(HDC hDC)
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
	Render_Heart_Boss(hDC);
}

void CGreenDragon::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		auto pLKey = L"Dragon_Green_Left";
		auto pRKey = L"Dragon_Green_Right";

		if (m_eCurState == IDLE)
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;

			case DOWN:
			case UP:
				m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;
			}
		}
		else
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;

			case DOWN:
			case UP:
				m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;
			}
		}

		m_ePreState = m_eCurState;
	}
}

void CGreenDragon::Do_Act()
{
	Chase();
}

void CGreenDragon::Move()
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

void CGreenDragon::Chase()
{
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pPlayerTile = pTileMgr->Get_Tile_On_Player();
	auto pMyTile = pTileMgr->Get_Tile(this);

	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	const int iCenter = 0;
	int iSearchStart = 0;

	if (iPosY > iCenter) iSearchStart = 0;
	else if (iPosX > iCenter) iSearchStart = 1;
	else if (iPosY < iCenter) iSearchStart = 2;
	else iSearchStart = 3;

	for (int i = 0; i < 4; ++i)
	{
		int iIndex = i + iSearchStart;
		iIndex %= 4;

		auto iDirPosX = iDx[iIndex] * TILECX;
		auto iDirPosY = iDy[iIndex] * TILECY;
		auto pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(
			m_tInfo.fX + (float)iDirPosX,
			m_tInfo.fY + (float)iDirPosY));

		if (!pDestTile) continue;

		auto pOwner = pDestTile->Get_Owner();

		if (iDirPosX > 0) m_eDir = RIGHT;
		else if (iDirPosX < 0) m_eDir = LEFT;
		else if (iDirPosY > 0) m_eDir = DOWN;
		else m_eDir = UP;

		if (pOwner == nullptr)
		{
			if (!m_bMove)
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			CSoundMgr::Get_Instance()->StopSound(SOUND_DRAGON_WALK);
			CSoundMgr::Get_Instance()->PlaySoundW(L"en_dragon_walk_02.ogg", SOUND_DRAGON_WALK, 0.4f);

			static_cast<CTile*>(pMyTile)->Leave_Here();
			pDestTile->Set_Owner(this);
			break;
		}

		if (pOwner->Get_ObjID() == OBJ_WALL)
		{
			auto eWallID = static_cast<CWall*>(pOwner)->Get_WallID();
			if (eWallID == STONE || eWallID == NORMAL || eWallID == SHOP ||
				eWallID == DOOR_H || eWallID == DOOR_V)
			{
				pOwner->Set_Dead();
				pDestTile->Leave_Here();

				if (!m_bMove)
				{
					m_bMove = true;
					m_fPreYPos = m_tInfo.fY;
				}

				CSoundMgr::Get_Instance()->StopSound(SOUND_DRAGON_WALK);
				CSoundMgr::Get_Instance()->PlaySoundW(L"en_dragon_attack_melee", SOUND_DRAGON_WALK, 0.5f);

				static_cast<CTile*>(pMyTile)->Leave_Here();
				pDestTile->Set_Owner(this);
				break;
			}
		}

		if (pOwner->Get_ObjID() == OBJ_PLAYER)
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

			CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_ATTACK);
			CSoundMgr::Get_Instance()->PlaySoundW(L"en_dragon_attack_melee", SOUND_MONSTER_ATTACK, 0.5f);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
			CObjMgr::Get_Instance()->Player_Damaged(m_iAttack);
			break;
		}
	}

	m_eCurState = MOVE;
}

void CGreenDragon::Release()
{
}

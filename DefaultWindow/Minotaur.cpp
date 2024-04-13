#include "stdafx.h"
#include "Minotaur.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "Wall.h"

CMinotaur::CMinotaur()
	: m_bCharge(false), m_bCollision(false), m_iFrameStart(0), m_iStunCnt(0)
{
}

CMinotaur::~CMinotaur()
{
	Release();
}

void CMinotaur::Initialize()
{
	m_tInfo = {0, 0, 100.f, 98.f};
	m_pFrameKey = L"Minotaur_Left";
	m_iFrameStart = 0;
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 3;
	m_iHp = 3;
	m_iAttack = 4;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 1;
	m_iAlphaPosY = -48;
	m_pParticleKey = L"Particle_Blood";
	Set_Heart(m_iMaxHp); // TODO
}

int CMinotaur::Update()
{
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_minotaur_death.ogg", SOUND_MONSTER_DEAD, 0.5f);
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

	__super::Update_Rect();
	Update_Heart();
	return OBJ_NOEVENT;
}

void CMinotaur::Late_Update()
{
	Move();
	Motion_Change();
	Move_Frame();
}

void CMinotaur::Render(HDC hDC)
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

void CMinotaur::Release()
{
}

void CMinotaur::Do_Act()
{
	// BUG : 살짝 수정 필요
	switch (m_eCurState)
	{
	case IDLE:
		Detect();
		break;

	case MOVE:
		Chase();
		break;

	case CHARGE:
		Charge();
		break;

	case STUN:
		Stun();
		break;

	default:
		break;
	}
}

void CMinotaur::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		auto pLKey = L"Minotaur_Left";
		auto pRKey = L"Minotaur_Right";

		// logic
		if (m_eCurState == IDLE ||
			m_eCurState == MOVE)
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 0;
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 0;
				break;

			case DOWN:
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 0;
				break;

			case UP:
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 0;
				break;
			}
		}
		else if (m_eCurState == CHARGE)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MINO_CHARGE);
			CSoundMgr::Get_Instance()->PlaySoundW(L"en_minotaur_charge.ogg", SOUND_MINO_CHARGE, 0.5f);

			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{4, 4, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 4;
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{4, 4, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 4;
				break;

			case DOWN:
				m_tFrame = FRAME{4, 4, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 4;
				break;

			case UP:
				m_tFrame = FRAME{4, 4, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 4;
				break;
			}
		}
		else if (m_eCurState == STUN)
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{5, 8, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 5;
				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{5, 8, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 5;
				break;

			case DOWN:
				m_tFrame = FRAME{5, 8, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 5;
				break;

			case UP:
				m_tFrame = FRAME{5, 8, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				m_iFrameStart = 5;
				break;
			}
		}
		m_ePreState = m_eCurState;
	}
}

void CMinotaur::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = m_iFrameStart;
	}
}

void CMinotaur::Move()
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
			}
		}
	}
}

void CMinotaur::Charge()
{
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pMyTile = pTileMgr->Get_Tile(this);

	int iDir = -1;
	switch (m_eDir)
	{
	case UP: iDir = 0;
		break;
	case LEFT: iDir = 1;
		break;
	case DOWN: iDir = 2;
		break;
	case RIGHT: iDir = 3;
		break;
	}

	int iDirX = iDx[iDir];
	int iDirY = iDy[iDir];

	auto pDestTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
		pMyTile->Get_Info().fX + (float)(iDirX * TILECX),
		pMyTile->Get_Info().fY + (float)(iDirY * TILECY)));

	assert(pDestTile);

	if (pDestTile->Get_Owner() == nullptr)
	{
		m_bMove = true;
		m_fPreYPos = m_tInfo.fY;
		static_cast<CTile*>(pMyTile)->Leave_Here();
		pDestTile->Set_Owner(this);
	}
	else if (pDestTile->Get_Owner()->Get_ObjID() == OBJ_WALL)
	{
		auto pOwner = static_cast<CWall*>(pDestTile->Get_Owner());
		auto eID = pOwner->Get_WallID();

		if (eID == NORMAL || eID == STONE || eID == DOOR_H || eID == DOOR_V)
		{
			pOwner->Set_Dead();
			pDestTile->Leave_Here();
		}

		m_bCollision = true;
		m_eCurState = STUN;
	}
	else if (pDestTile->Get_Owner()->Get_ObjID() == OBJ_PLAYER ||
		pDestTile->Get_Owner()->Get_ObjID() == OBJ_MONSTER)
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

		if (pDestTile->Get_Owner()->Get_ObjID() == OBJ_PLAYER)
		{
			CObjMgr::Get_Instance()->Player_Damaged(m_iAttack);
		}
		else
		{
			pDestTile->Get_Owner()->On_Damaged(m_iAttack);
			pDestTile->Get_Owner()->Set_IsAttacked();
		}

		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_ATTACK);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_minotaur_attack.ogg", SOUND_MONSTER_ATTACK, 0.5f);

		m_bCollision = true;
		m_eCurState = STUN;
	}
}

void CMinotaur::Chase()
{
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pPlayerTile = pTileMgr->Get_Tile_On_Player(); // 플레이어 위치
	auto pMyTile = pTileMgr->Get_Tile(this); // 내 위치

	// 내 위치에서 플레이어의 위치를 뺀 값
	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	if (abs(iPosX) <= 1 || abs(iPosY) <= 1)
	{
		if (iPosY < 0)
			m_eDir = DOWN;
		else if (iPosX > 0)
			m_eDir = LEFT;
		else if (iPosY > 0)
			m_eDir = UP;
		else
			m_eDir = RIGHT;

		m_bCharge = true;
		m_eCurState = CHARGE;
		return;
	}

	const int iCenter = 0;
	int iSearchStart = 0;

	if (iPosY > iCenter)
	{
		iSearchStart = 0;
	}
	else if (iPosX > iCenter)
	{
		iSearchStart = 1;
	}
	else if (iPosY < iCenter)
	{
		iSearchStart = 2;
	}
	else
	{
		iSearchStart = 3;
	}

	for (int i = 0; i < 4; ++i)
	{
		int iIndex = i + iSearchStart;
		if (iIndex > 3)
			iIndex -= 4;

		auto iDirPosX = iDx[iIndex] * TILECX;
		auto iDirPosY = iDy[iIndex] * TILECY;

		auto pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(
			m_tInfo.fX + (float)iDirPosX,
			m_tInfo.fY + (float)iDirPosY));

		if (!pDestTile)
		{
			continue;
		}

		auto pOwner = pDestTile->Get_Owner();

		if (pOwner == nullptr)
		{
			if (iDirPosY < 0)
				m_eDir = UP;
			else if (iDirPosX < 0)
				m_eDir = LEFT;
			else if (iDirPosY > 0)
				m_eDir = DOWN;
			else
				m_eDir = RIGHT;

			if (!m_bMove)
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			static_cast<CTile*>(pMyTile)->Leave_Here();
			pDestTile->Set_Owner(this);
			break;
		}
	}
}

void CMinotaur::Stun()
{
	if (m_bCollision)
	{
		++m_iStunCnt;
	}

	if (m_iStunCnt >=
		3)
	{
		m_bCollision = false;
		m_iStunCnt = 0;
		m_eCurState = IDLE;
	}
}

void CMinotaur::Detect()
{
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pPlayerTile = pTileMgr->Get_Tile_On_Player(); // 플레이어 위치
	auto pMyTile = pTileMgr->Get_Tile(this); // 내 위치

	// 내 위치에서 플레이어의 위치를 뺀 값
	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	// 내 위치에서 플레이어의 위치를 뺀게 양수면.. 내가 플레이어보다 아래에 있는것 즉 방향을 위로 해야함
	if (iPosY < 0)
		m_eDir = DOWN;
	else if (iPosX > 0)
		m_eDir = LEFT;
	else if (iPosY > 0)
		m_eDir = UP;
	else
		m_eDir = RIGHT;

	if (abs(iPosX) >= TILECX * 6 || abs(iPosY) >= TILECY * 6)
	{
		m_eCurState = IDLE;
	}
	else
	{
		m_eCurState = MOVE;
	}
}

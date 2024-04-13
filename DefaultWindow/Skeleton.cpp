#include "stdafx.h"
#include "Skeleton.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "Tile.h"
#include "TileMgr.h"

CSkeleton::CSkeleton()
{
}

CSkeleton::~CSkeleton()
{
	Release();
}

void CSkeleton::Initialize()
{
	m_tInfo = {0, 0, 48.f, 50.f};
	m_pFrameKey = L"Skeleton_White_Left";
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 1;
	m_iHp = 1;
	m_iAttack = 1; // 0.5 라는 뜻
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 1;
	m_iAlphaPosY = -18;
	m_pParticleKey = L"Particle_White";
	Set_Heart(m_iMaxHp);
}

int CSkeleton::Update()
{
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_skel_death.ogg", SOUND_MONSTER_DEAD, 0.5f);
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

void CSkeleton::Late_Update()
{
	Move();
	Motion_Change();
	__super::Move_Frame();
}

void CSkeleton::Render(HDC hDC)
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

void CSkeleton::Release()
{
}

void CSkeleton::Do_Act()
{
	switch (m_eCurState)
	{
	case IDLE:
		m_eCurState = MOVE;
		break;

	case MOVE:
		Chase(true);
		break;

	default:
		break;
	}
}

void CSkeleton::Move()
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

void CSkeleton::Chase(bool _bCanAttack)
{
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pPlayerTile = pTileMgr->Get_Tile_On_Player(); // 플레이어 위치
	auto pMyTile = pTileMgr->Get_Tile(this); // 내 위치

	// 내 위치에서 플레이어의 위치를 뺀 값
	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	const int iCenter = 0;
	int iSearchStart = 0;

	if (iPosY > iCenter) // 상으로 이동
	{
		iSearchStart = 0;
	}
	else if (iPosX > iCenter) // 좌로 이동
	{
		iSearchStart = 1;
	}
	else if (iPosY < iCenter) // 하로 이동
	{
		iSearchStart = 2;
	}
	else // 우로 이동
	{
		iSearchStart = 3;
	}

	for (int i = 0; i < 4; ++i)
	{
		// 이 코드 다 찾아서 고쳐야될듯
		int iIndex = i + iSearchStart;
		iIndex %= 4;

		auto iDirPosX = iDx[iIndex] * TILECX;
		auto iDirPosY = iDy[iIndex] * TILECY;

		auto pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(
			m_tInfo.fX + (float)iDirPosX,
			m_tInfo.fY + (float)iDirPosY));

		if (!pDestTile) continue;

		auto pOwner = pDestTile->Get_Owner();

		if (pOwner == nullptr)
		{
			if (iDirPosX > 0)
				m_eDir = RIGHT;
			else if (iDirPosX < 0)
				m_eDir = LEFT;
			else if (iDirPosY > 0)
				m_eDir = DOWN;
			else
				m_eDir = UP;

			if (!m_bMove)
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			static_cast<CTile*>(pMyTile)->Leave_Here();
			pDestTile->Set_Owner(this);
			break;
		}

		if (pOwner->Get_ObjID() == OBJ_PLAYER)
		{
			if (iDirPosX > 0)
				m_eDir = RIGHT;
			else if (iDirPosX < 0)
				m_eDir = LEFT;
			else if (iDirPosY > 0)
				m_eDir = DOWN;
			else
				m_eDir = UP;

			if (_bCanAttack)
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
				CSoundMgr::Get_Instance()->PlaySoundW(L"en_skel_attack_melee.ogg", SOUND_MONSTER_ATTACK, 0.5f);

				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
				CObjMgr::Get_Instance()->Player_Damaged(m_iAttack);
			}
			break;
		}
	}

	m_eCurState = IDLE;
}

void CSkeleton::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		auto pLKey = L"Skeleton_White_Left";
		auto pRKey = L"Skeleton_White_Right";

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
			case UP:
				m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;
			}
		}
		else
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
			case UP:
				m_tFrame = FRAME{4, 7, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;
			}
		}

		m_ePreState = m_eCurState;
	}
}

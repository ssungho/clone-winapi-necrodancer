#include "stdafx.h"
#include "YellowSkeleton.h"

#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"

CYellowSkeleton::CYellowSkeleton()
{
}

CYellowSkeleton::~CYellowSkeleton()
{
	Release();
}

void CYellowSkeleton::Initialize()
{
	m_tInfo = {0, 0, 48.f, 50.f};
	m_pFrameKey = L"Skeleton_Yellow_Left";
	m_iFrameStart = 0;
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 2;
	m_iHp = 2;
	m_iAttack = 3; // TODO : 넘썜
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 1;
	m_iAlphaPosY = -18;
	m_pParticleKey = L"Particle_Yellow";
	Set_Heart(m_iMaxHp);
}

int CYellowSkeleton::Update()
{
	if (m_bDead)
	{
		Draw_Particle();
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_skel_death.ogg", SOUND_MONSTER_DEAD, 0.5f);
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

void CYellowSkeleton::Late_Update()
{
	Move();
	Motion_Change();
	Move_Frame();
}

void CYellowSkeleton::Render(HDC hDC)
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

void CYellowSkeleton::Release()
{
}

void CYellowSkeleton::Do_Act()
{
	if (m_bAttacked)
	{
		m_eCurState = HIT;
		Throw(L"Particle_Yellow_Head", 18, 18);
		m_bAttacked = false;
	}

	switch (m_eCurState)
	{
	case IDLE:
		m_eCurState = MOVE;
		break;

	case MOVE:
		Chase(true);
		break;

	case HIT:
		Escape();
		break;

	default:
		break;
	}
}

void CYellowSkeleton::Escape()
{
	// 현재 플레이어의 위치
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pPlayerTile = pTileMgr->Get_Tile_On_Player();
	auto pMyTile = static_cast<CTile*>(pTileMgr->Get_Tile(this));

	// 내 위치에서 플레이어의 위치를 뺀 값
	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	int iCenter = 0;
	int iDir;

	// 반시계 방향 
	if (iPosX > iCenter)
	{
		iDir = 3;
	}
	else if (iPosX < iCenter)
	{
		iDir = 1;
	}
	else if (iPosY > iCenter)
	{
		iDir = 2;
	}
	else
	{
		iDir = 0;
	}

	auto pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(
		m_tInfo.fX + (float)(iDx[iDir] * TILECX),
		m_tInfo.fY + (float)(iDy[iDir] * TILECY)
	));

	// 목적지가 존재하고 && 목적지에 소유자가 존재하지 않을 경우
	if (pDestTile != nullptr && pDestTile->Get_Owner() == nullptr)
	{
		if (iDx[iDir] > 0)
		{
			m_eDir = RIGHT;
		}
		else if (iDx[iDir] < 0)
		{
			m_eDir = LEFT;
		}
		else if (iDy[iDir] > 0)
		{
			m_eDir = DOWN;
		}
		else
		{
			m_eDir = UP;
		}

		if (!m_bMove)
		{
			m_bMove = true;
			m_fPreYPos = m_tInfo.fY;
		}

		pMyTile->Leave_Here();
		pDestTile->Set_Owner(this);
	}
}

void CYellowSkeleton::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = m_iFrameStart;
	}
}

void CYellowSkeleton::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		auto pLKey = L"Skeleton_Yellow_Left";
		auto pRKey = L"Skeleton_Yellow_Right";

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
			case UP:
				m_tFrame = FRAME{4, 7, 0, (unsigned long)m_iBpm / 2, GetTickCount()};
				break;
			}
		}
		else if (m_eCurState == HIT)
		{
			switch (m_eDir)
			{
			case LEFT:
				m_pFrameKey = pLKey;
				m_tFrame = FRAME{8, 8, 0, (unsigned long)m_iBpm, GetTickCount()};

				break;

			case RIGHT:
				m_pFrameKey = pRKey;
				m_tFrame = FRAME{8, 8, 0, (unsigned long)m_iBpm, GetTickCount()};

				break;

			case DOWN:
			case UP:
				m_tFrame = FRAME{8, 8, 0, (unsigned long)m_iBpm, GetTickCount()};
				break;
			}
			m_iFrameStart = 8;
		}

		m_ePreState = m_eCurState;
	}
}

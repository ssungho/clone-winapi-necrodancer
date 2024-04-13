#include "stdafx.h"
#include "DeathMetal.h"

#include "AbstractFactory.h"
#include "Bat.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "Inven.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Skeleton.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "YellowSkeleton.h"

CDeathMetal::CDeathMetal()
	: m_bOnShield(false), m_iStartFrame(0),
	  m_eCurPhase(P_FIRST), m_ePrePhase(P_END), m_iSoundCnt(0),
	  m_iSoundMaxCnt(0), m_iSummonCnt(0), m_iSummonMaxCnt(0), m_bCanSummon(false), m_bFirstDetect(false)
{
	// no shield : 2페이즈 진입시 애니메이션 조정
}

CDeathMetal::~CDeathMetal()
{
	Release();
}

void CDeathMetal::Initialize()
{
	CTileMgr::Get_Instance()->Get_Tile(225)->Set_FrameKey(L"Stair_Locked_Tile");

	m_tInfo = {200.f, 200.f, 176.f, 106.f};
	// m_bIsFlip = false;
	m_pFrameKey = L"Death_Metal_Left";
	m_fSpeed = 3.0f;
	m_eDir = DOWN;
	m_eCurState = IDLE;
	m_iMaxHp = 9;
	m_iHp = m_iMaxHp;
	m_iStartFrame = 2;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_bOnShield = true;
	m_iNeedToDoAct = 3;
	m_iAttack = 3;
	m_iAlphaPosY = -TILECX;
	Set_Heart(m_iMaxHp);
}

int CDeathMetal::Update()
{
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_DEATH);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_deathmetal_death.ogg", SOUND_BOSS_DEATH, 0.5f);
		CTileMgr::Get_Instance()->Get_Tile(225)->Set_FrameKey(L"Stair_Tile");
		return OBJ_DEAD;
	}

	Update_Phase();

	if (m_bAttacked)
	{
		if (m_iBeatCount >= 1)
			--m_iBeatCount;

		Do_Act();
	}
	else
	{
		if (m_iBeatCount >= m_iNeedToDoAct)
		{
			Do_Act();
			m_iBeatCount = 0;
		}
	}

	m_bAttacked = false;

	__super::Update_Rect();
	Update_Heart();

	return OBJ_NOEVENT;
}

void CDeathMetal::Late_Update()
{
	Play_Sound();
	Move();
	Motion_Change();
	Move_Frame();
}

void CDeathMetal::Render(HDC hDC)
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
	Render_Heart_Boss(hDC, 10, -10);
}

void CDeathMetal::Release()
{
}

void CDeathMetal::Motion_Change()
{
	if ((m_ePreState != m_eCurState))
	{
		switch (m_eCurState)
		{
		case IDLE:
			if (m_bOnShield)
			{
				if (m_eDir == RIGHT)
				{
					m_pFrameKey = L"Death_Metal_Right";
					m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 0;
				}
				else if (m_eDir == LEFT)
				{
					m_pFrameKey = L"Death_Metal_Left";
					m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 0;
				}
				else if (m_eDir == UP)
				{
					m_tFrame = FRAME{4, 5, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 4;
				}
				else if (m_eDir == DOWN)
				{
					m_tFrame = FRAME{2, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 2;
				}
			}
			else
			{
				if (m_eDir == RIGHT)
				{
					m_pFrameKey = L"Death_Metal_Right";
					m_tFrame = FRAME{8, 9, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 8;
				}
				else if (m_eDir == LEFT)
				{
					m_pFrameKey = L"Death_Metal_Left";
					m_tFrame = FRAME{8, 9, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 8;
				}
				else
				{
					m_tFrame = FRAME{8, 9, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 8;
				}
			}

			break;
		case MOVE:
			if (m_bOnShield)
			{
				if (m_eDir == RIGHT)
				{
					m_pFrameKey = L"Death_Metal_Right";
					m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 0;
				}
				else if (m_eDir == LEFT)
				{
					m_pFrameKey = L"Death_Metal_Left";
					m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 0;
				}
				else if (m_eDir == UP)
				{
					m_tFrame = FRAME{4, 5, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 4;
				}
				else if (m_eDir == DOWN)
				{
					m_tFrame = FRAME{2, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 2;
				}
			}
			else
			{
				if (m_eDir == RIGHT)
				{
					m_pFrameKey = L"Death_Metal_Right";
					m_tFrame = FRAME{8, 9, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 8;
				}
				else if (m_eDir == LEFT)
				{
					m_pFrameKey = L"Death_Metal_Left";
					m_tFrame = FRAME{8, 9, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 8;
				}
				else
				{
					m_tFrame = FRAME{8, 9, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 8;
				}
			}

			break;

		case SUMMON:
			if (m_bOnShield)
			{
				if (m_eDir == RIGHT)
				{
					m_pFrameKey = L"Death_Metal_Right";
					m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 0;
				}
				else if (m_eDir == LEFT)
				{
					m_pFrameKey = L"Death_Metal_Left";
					m_tFrame = FRAME{0, 1, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 0;
				}
				else if (m_eDir == UP)
				{
					m_tFrame = FRAME{4, 5, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 4;
				}
				else if (m_eDir == DOWN)
				{
					m_tFrame = FRAME{2, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 2;
				}
			}
			else
			{
				if (m_eDir == RIGHT)
				{
					m_pFrameKey = L"Death_Metal_Right";
					m_tFrame = FRAME{6, 7, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 6;
				}
				else if (m_eDir == LEFT)
				{
					m_pFrameKey = L"Death_Metal_Left";
					m_tFrame = FRAME{6, 7, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 6;
				}
				else
				{
					m_tFrame = FRAME{6, 7, 0, (unsigned long)m_iBpm, GetTickCount()};
					m_iStartFrame = 6;
				}
			}

			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CDeathMetal::Do_Act()
{
	auto pPlayer = CObjMgr::Get_Instance()->Get_Player();

	// 원래 앞에 있던거임
	if (m_bAttacked)
	{
		m_iSummonCnt = 0;
		m_iSoundCnt = 0;
		m_bCanSummon = false;
	}
	else
	{
		if (m_bCanSummon)
		{
			++m_iSummonCnt;
		}
		else
		{
			++m_iSoundCnt;
		}

		m_iBeatCount = 0;
	}

	switch (m_eCurPhase)
	{
	case P_FIRST:

		if (m_bAttacked)
		{
			if (On_Face(pPlayer))
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_HIT);
				CSoundMgr::Get_Instance()->PlaySound(L"en_deathmetal_shieldhit.ogg", SOUND_BOSS_HIT, 0.5f);

				Summon();
				Heal(CInven::Get_Instance()->Get_Attack());
				Escape();
			}
			else
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_HIT);
				CSoundMgr::Get_Instance()->PlaySound(L"en_deathmetal_hit_01.ogg", SOUND_BOSS_HIT, 0.5f);
				Teleport();
			}
		}
		else
		{
			if (On_Range())
			{
				Chase();
			}
		}

		if (m_iHp < 7)
		{
			m_eCurPhase = P_SECOND;
			Throw(L"Particle_Shield", 90, 46);
		}

		break;

	case P_SECOND:

		if (m_bAttacked)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_HIT);
			CSoundMgr::Get_Instance()->PlaySoundW(L"en_deathmetal_hit_02.ogg", SOUND_BOSS_HIT, 0.5f);

			Teleport();

			m_bAttacked = false;
		}
		else
		{
			if (m_iSummonCnt >= m_iSummonMaxCnt)
			{
				Summon();
			}

			else if (On_Range())
			{
				Escape();
			}
		}

		if (m_iHp < 5)
		{
			m_eCurPhase = P_THIRD;
		}

		break;

	case P_THIRD:

		if (m_bAttacked)
		{
			Teleport();

			m_bAttacked = false;
		}
		else
		{
			if (m_iSummonCnt >= m_iSummonMaxCnt)
			{
				Summon();
			}

			if (On_Range())
			{
				Escape();
			}
		}

		if (m_iHp < 3)
		{
			m_eCurPhase = P_FOURTH;
		}

		break;

	case P_FOURTH:

		if (m_bAttacked)
		{
			Teleport();

			m_bAttacked = false;
		}
		else
		{
			if (m_iSummonCnt >= m_iSummonMaxCnt)
			{
				Summon();
			}
			else
			{
				Chase_Or_Escape();
			}
		}

		if (m_iHp < 1)
		{
			Set_Dead();
		}

		break;
	}
}

void CDeathMetal::Move()
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

				if (m_bCanSummon && m_eCurPhase >= P_SECOND)
				{
					m_eCurState = SUMMON;
				}
				else
				{
					m_eCurState = MOVE;
				}
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

				if (m_bCanSummon && m_eCurPhase >= P_SECOND)
				{
					m_eCurState = SUMMON;
				}
				else
				{
					m_eCurState = MOVE;
				}
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

				if (m_bCanSummon && m_eCurPhase >= P_SECOND)
				{
					m_eCurState = SUMMON;
				}
				else
				{
					m_eCurState = MOVE;
				}
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

				if (m_bCanSummon && m_eCurPhase >= P_SECOND)
				{
					m_eCurState = SUMMON;
				}
				else
				{
					m_eCurState = MOVE;
				}
			}
		}
	}
}

void CDeathMetal::Summon()
{
	int iSummonCount = rand() % 3 + 1;

	switch (m_eCurPhase)
	{
	case P_FIRST:
		if (m_eDir == LEFT || m_eDir == RIGHT)
		{
			auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY - TILECY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(
						OBJ_MONSTER, CAbstractFactory<CBat>::Create(
							m_tInfo.fX, m_tInfo.fY - TILECY, m_iBpm));
				}
			}

			pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY + TILECY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(
						OBJ_MONSTER, CAbstractFactory<CBat>::Create(
							m_tInfo.fX, m_tInfo.fY + TILECY, m_iBpm));
				}
			}
		}
		else
		{
			auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX - TILECX, m_tInfo.fY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER,
					                                    CAbstractFactory<CBat>::Create(
						                                    m_tInfo.fX - TILECX, m_tInfo.fY, m_iBpm));
				}
			}

			pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX + TILECX, m_tInfo.fY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER,
					                                    CAbstractFactory<CBat>::Create(
						                                    m_tInfo.fX + TILECX, m_tInfo.fY, m_iBpm));
				}
			}
		}
		break;

	case P_SECOND:
		if (m_eDir == LEFT || m_eDir == RIGHT)
		{
			auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY - TILECY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(
						OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(
							m_tInfo.fX, m_tInfo.fY - TILECY, m_iBpm));
				}
			}

			pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY + TILECY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(
						OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(
							m_tInfo.fX, m_tInfo.fY + TILECY, m_iBpm));
				}
			}
		}
		else
		{
			auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX - TILECX, m_tInfo.fY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr
				)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER,
					                                    CAbstractFactory<CSkeleton>::Create(
						                                    m_tInfo.fX - TILECX, m_tInfo.fY, m_iBpm));
				}
			}

			pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX + TILECX, m_tInfo.fY));
			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER,
					                                    CAbstractFactory<CSkeleton>::Create(
						                                    m_tInfo.fX + TILECX, m_tInfo.fY, m_iBpm));
				}
			}
		}
		break;

	case P_THIRD:
		for (int i = 0; i < iSummonCount; ++i)
		{
			auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
				DM_CENTER_X + iDx[i] * TILECX,
				DM_CENTER_Y + iDy[i] * TILECY));

			if (pTile != nullptr)
			{
				if (pTile->Get_Owner() == nullptr)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER,
					                                    CAbstractFactory<CYellowSkeleton>::Create(
						                                    pTile->Get_Info().fX, pTile->Get_Info().fY, m_iBpm));
				}
			}
		}
		break;

	case P_FOURTH:
		Fire();
		break;
	}

	m_iSummonCnt = 0;
	m_bCanSummon = false;

	// 원래는 IDLE
	m_eCurState = MOVE;
}

void CDeathMetal::Chase()
{
	auto pTileMgr = CTileMgr::Get_Instance();

	auto pPlayerTile = pTileMgr->Get_Tile_On_Player();
	auto pMyTile = pTileMgr->Get_Tile(this);

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

				if (m_iSummonCnt > 0)
					m_eCurState = SUMMON;
				else
					m_eCurState = IDLE;
			}

			static_cast<CTile*>(pMyTile)->Leave_Here();
			pDestTile->Set_Owner(this);
			break;
		}
	}
}

void CDeathMetal::Escape()
{
	// 현재 플레이어의 위치
	auto pTileMgr = CTileMgr::Get_Instance();
	auto pPlayerTile = pTileMgr->Get_Tile_On_Player();
	auto pMyTile = static_cast<CTile*>(pTileMgr->Get_Tile(this));

	// 내 위치에서 플레이어의 위치를 뺀 값
	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	const int iCenter = 0;
	int iSearchStart = 0;
	int iJumpIndex = 0;

	// 반시계 방향 
	if (iPosX > iCenter)
	{
		// 우 상 하 가능
		iSearchStart = 3;
		iJumpIndex = 1;
	}
	else if (iPosX < iCenter)
	{
		// 좌 하 상가능
		iSearchStart = 1;
		iJumpIndex = 3;
	}
	else if (iPosY > iCenter)
	{
		// 하 좌 우 가능
		iSearchStart = 2;
		iJumpIndex = 0;
	}
	else
	{
		// 상 좌 우 가능
		iSearchStart = 0;
		iJumpIndex = 2;
	}

	CTile* pDestTile = nullptr;

	for (int i = 0; i < 4; ++i)
	{
		int iIndex = i + iSearchStart;
		if (iIndex > 3)
			iIndex -= 4;

		if (iIndex == iJumpIndex)
			continue;

		pDestTile = dynamic_cast<CTile*>(pTileMgr->Get_Tile(
			m_tInfo.fX + (float)(iDx[iIndex] * TILECX),
			m_tInfo.fY + (float)(iDy[iIndex] * TILECY)
		));

		// 목적지가 존재하고 && 목적지에 소유자가 존재하지 않을 경우
		if (pDestTile != nullptr && pDestTile->Get_Owner() == nullptr)
		{
			if (iDx[iIndex] > 0)
			{
				m_eDir = RIGHT;
			}
			else if (iDx[iIndex] < 0)
			{
				m_eDir = LEFT;
			}
			else if (iDy[iIndex] > 0)
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

				if (m_iSummonCnt > 0)
					m_eCurState = SUMMON;
				else
					m_eCurState = IDLE;
			}

			pMyTile->Leave_Here();
			pDestTile->Set_Owner(this);

			break;
		}
	}
}

void CDeathMetal::Teleport()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	auto pCurTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this));
	auto pStartTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(504, 312));

	while (true)
	{
		int iX = rand() % DM_RANGE_X;
		int iY = rand() % DM_RANGE_Y;

		auto pTile = static_cast<CTile*>(CTileMgr::Get_Instance()->
			Get_Tile(pStartTile->Get_Info().fX + float(iX * TILECX),
			         pStartTile->Get_Info().fY + float(iY * TILECY)));

		assert(pTile);

		if (pTile->Get_Owner() == nullptr)
		{
			pCurTile->Leave_Here();
			pTile->Set_Owner(this);
			Set_Pos(pTile);
			break;
		}
	}
}

bool CDeathMetal::On_Range()
{
	for (int y = 0; y < DM_RANGE_Y; ++y)
	{
		for (int x = 0; x < DM_RANGE_X; ++x)
		{
			float fCheckX = DM_SEARCH_START_X + (float)(TILECX * x);
			float fCheckY = DM_SEARCH_START_Y + (float)(TILECY * y);

			auto pTile = CTileMgr::Get_Instance()->Get_Tile(fCheckX, fCheckY);

			if (pTile == nullptr)
				continue;

			auto pObj = static_cast<CTile*>(pTile)->Get_Owner();

			if (pObj == nullptr)
				continue;

			if (pObj->Get_ObjID() == OBJ_PLAYER)
			{
				if (!m_bFirstDetect)
				{
					CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_SUMMON);
					CSoundMgr::Get_Instance()->PlaySound(L"en_deathmetal_welcome.ogg", SOUND_BOSS_SUMMON, 0.5f);
					m_bFirstDetect = true;
				}

				return true;
			}
		}
	}
	return false;
}

void CDeathMetal::Chase_Or_Escape() // 세로축은 따라가고 가로축은 도망간다.
{
	// 현재 플레이어의 위치
	auto pTileMgr = CTileMgr::Get_Instance();

	auto pPlayerTile = pTileMgr->Get_Tile_On_Player();
	auto pMyTile = pTileMgr->Get_Tile(this);

	// 내 위치에서 플레이어의 위치를 뺀 값
	int iPosX = (int)pMyTile->Get_Info().fX - (int)pPlayerTile->Get_Info().fX;
	int iPosY = (int)pMyTile->Get_Info().fY - (int)pPlayerTile->Get_Info().fY;

	if (abs(iPosX) <= 1)
	{
		Chase();
	}
	else if (abs(iPosY) <= 1)
	{
		Run_Offside();
	}
	else
	{
		int i = rand() % 2;
		if (i == 0)
		{
			Chase();
		}
		else
		{
			Run_Offside();
		}
	}
}

void CDeathMetal::Fire()
{
	stack<CTile*> tileStack;

	auto pOnTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this));
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	float fPosX = 504.f;
	float fPosY = pOnTile->Get_Info().fY;

	for (int i = 0; i < DM_RANGE_X; ++i)
	{
		tileStack.push(
			static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(fPosX + static_cast<float>(TILECX * i), fPosY)));
	}

	bool bAttack = false;
	while (!tileStack.empty())
	{
		auto pDestTile = tileStack.top();
		CObj* pFireball = CAbstractFactory<CEffect>::Create(
			L"Fireball", 48.f, 48.f, pDestTile, m_iBpm);

		static_cast<CEffect*>(pFireball)->Set_EffectType(EFFECT_FIREBALL);
		pFireball->Set_Frame({0, 6, 0, (DWORD)m_iBpm / 8, GetTickCount()});
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pFireball);

		if ((abs(pPlayer->Get_Info().fY - pDestTile->Get_Info().fY) <= FLOAT_RANGE) ||
			(abs(pDestTile->Get_Info().fY - pPlayer->Get_Info().fY) <= FLOAT_RANGE))
		{
			bAttack = true;
		}

		tileStack.pop();
	}

	if (bAttack)
		CObjMgr::Get_Instance()->Get_Player()->On_Damaged(m_iAttack);
}

void CDeathMetal::Run_Offside()
{
	auto pPlayer = CObjMgr::Get_Instance()->Get_Player();

	auto iDir = pPlayer->Get_Info().fX - m_tInfo.fX;

	if (!pPlayer) return;

	if ((abs(pPlayer->Get_Info().fY - m_tInfo.fY) <= FLOAT_RANGE) ||
		(abs(m_tInfo.fY - pPlayer->Get_Info().fY) <= FLOAT_RANGE))
	{
		int iIndex = 0;
		if (iDir < 0)
		{
			m_eDir = RIGHT;
			iIndex = 3;
		}
		else
		{
			m_eDir = LEFT;
			iIndex = 1;
		}

		auto pDestTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
			m_tInfo.fX + (float)(iDx[iIndex] * TILECX),
			m_tInfo.fY + (float)(iDy[iIndex] * TILECY)));

		if (pDestTile->Get_Owner() == nullptr)
		{
			if (!m_bMove)
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;

				auto pCurTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this));
				pCurTile->Leave_Here();
				pDestTile->Set_Owner(this);
			}
		}

		if (m_iSummonCnt > 0)
			m_eCurState = SUMMON;
		else
			m_eCurState = IDLE;
	}
}

void CDeathMetal::Update_Phase()
{
	if ((m_ePrePhase != m_eCurPhase))
	{
		switch (m_eCurPhase)
		{
		case P_FIRST:
			m_iNeedToDoAct = 3;
			break;

		case P_SECOND:
			m_bOnShield = false;
			m_iNeedToDoAct = 2;
			m_iSoundMaxCnt = 7;
			m_iSummonMaxCnt = 3;
			m_pFrameKey = L"Death_Metal_Left";
			m_eDir = DOWN;
			m_ePreState = ST_DEAD;
			m_eCurState = SUMMON;
			break;

		case P_THIRD:
			m_bCanSummon = false;
			m_iSummonCnt = 0;
			m_iSummonMaxCnt = 6;
			m_iSoundCnt = 0;
			m_iSoundMaxCnt = 6;
			m_iNeedToDoAct = 2;
			m_pFrameKey = L"Death_Metal_Left";
			m_eDir = DOWN;
			m_ePreState = ST_DEAD;
			m_eCurState = SUMMON;
			break;

		case P_FOURTH:
			m_bCanSummon = false;
			m_iSummonCnt = 0;
			m_iSummonMaxCnt = 4;
			m_iSoundCnt = 0;
			m_iSoundMaxCnt = 4;
			m_iNeedToDoAct = 1;
			m_pFrameKey = L"Death_Metal_Left";
			m_eDir = DOWN;
			m_ePreState = ST_DEAD;
			m_eCurState = SUMMON;
			break;

		case P_END:
			break;
		}

		m_ePrePhase = m_eCurPhase;
	}
}

void CDeathMetal::Play_Sound()
{
	if (m_iSoundCnt >= m_iSoundMaxCnt)
	{
		switch (m_eCurPhase)
		{
		case P_FIRST:
			m_iSoundCnt = 0;
			break;

		case P_SECOND:
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_SUMMON);
			CSoundMgr::Get_Instance()->PlaySound(L"en_deathmetal_welcome.ogg", SOUND_BOSS_SUMMON, 0.5f);
			m_iSoundCnt = 0;
			break;

		case P_THIRD:
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_SUMMON);
			CSoundMgr::Get_Instance()->PlaySound(L"en_deathmetal_skels2.ogg", SOUND_BOSS_SUMMON, 0.5f);
			m_iSoundCnt = 0;
			break;

		case P_FOURTH:
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_SUMMON);
			CSoundMgr::Get_Instance()->PlaySound(L"en_deathmetal_fireball.ogg", SOUND_BOSS_SUMMON, 0.5f);
			m_iSoundCnt = 0;
			break;

		case P_END:
			break;
		}

		m_bCanSummon = true;
	}
}

void CDeathMetal::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = m_iStartFrame;
	}
}

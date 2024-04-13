#include "stdafx.h"
#include "Bat.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"

CBat::CBat()
{
}

CBat::~CBat()
{
	Release();
}

void CBat::Initialize()
{
	m_tInfo = {0, 0, 48.f, 48.f};
	m_tFrame = {0, 3, 0, (DWORD)m_iBpm, GetTickCount()};
	m_pFrameKey = L"Bat_Left";
	m_fSpeed = 3.0f;
	m_eDir = DOWN;
	m_eCurState = IDLE;
	m_iMaxHp = 1;
	m_iHp = 1;
	m_iAttack = 1; // 0.5임
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 3;
	m_iAlphaPosY = -12;
	m_pParticleKey = L"Particle_Purple";
	Set_Heart(m_iMaxHp);
}

int CBat::Update()
{
	if (m_bDead)
	{
		// 사운드 추가
		CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_DEAD);
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_bat_hit.ogg", SOUND_MONSTER_DEAD, 0.5f);
		Draw_Particle();
		Leave();
		return OBJ_DEAD;
	}

	if (m_bAttacked)
	{
		if (m_iBeatCount >= 1)
			--m_iBeatCount;
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

void CBat::Late_Update()
{
	Move();
	Motion_Change();
	__super::Move_Frame();
}

void CBat::Render(HDC hDC)
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

void CBat::Release()
{
}

void CBat::Do_Act()
{
	Patrol();
}

void CBat::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eDir)
		{
		case LEFT:
			m_pFrameKey = L"Bat_Left";
			m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
			break;

		case RIGHT:
			m_pFrameKey = L"Bat_Right";
			m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
			break;

		case DOWN:
			m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
			break;

		case UP:
			m_tFrame = FRAME{0, 3, 0, (unsigned long)m_iBpm, GetTickCount()};
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CBat::Patrol()
{
	auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this));

	assert(pTile);

	bool bSuccess = false;
	int iDir = rand() % 4;
	for (int i = 0; i < 4; ++i)
	{
		int iIndex = i + iDir;
		if (iIndex > 3)
			iIndex -= 4;

		// 상 좌 하 우
		int iX = iDx[iIndex];
		int iY = iDy[iIndex];

		auto pDestTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
			pTile->Get_Info().fX + (float)(iX * TILECX),
			pTile->Get_Info().fY + (float)(iY * TILECY)));

		if (pDestTile)
		{
			if (iIndex == 0)
				m_eDir = UP;
			else if (iIndex == 1)
				m_eDir = LEFT;
			else if (iIndex == 2)
				m_eDir = DOWN;
			else
				m_eDir = RIGHT;

			if (pDestTile->Get_Owner() == nullptr)
			{
				pTile->Leave_Here();
				pDestTile->Set_Owner(this);

				if (!m_bMove)
				{
					m_bMove = true;
					m_fPreYPos = m_tInfo.fY;
					m_eCurState = MOVE;
				}

				bSuccess = true;
				break;
			}

			// 새로 추가한 부분
			if (pDestTile->Get_Owner()->Get_ObjID() == OBJ_PLAYER)
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

				CSoundMgr::Get_Instance()->StopSound(SOUND_MONSTER_ATTACK);
				CSoundMgr::Get_Instance()->PlaySoundW(L"en_bat_attack.ogg", SOUND_MONSTER_ATTACK, 0.5f);
				bSuccess = true;
				break;
			}
		}
	}

	if (!bSuccess)
	{
		m_eDir = static_cast<DIRECTION>(iDir);
	}
}

void CBat::Move()
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

bool CBat::On_Range()
{
	for (int i = 0; i < 4; ++i)
	{
		auto pOwner = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
				m_tInfo.fX + (float)(iDx[i] * TILECX),
				m_tInfo.fY + (float)(iDy[i] * TILECY))
		)->Get_Owner();

		if (pOwner)
		{
			if (pOwner->Get_ObjID() == OBJ_PLAYER)
			{
				return true;
			}
		}
	}

	return false;
}

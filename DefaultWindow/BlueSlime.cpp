#include "stdafx.h"
#include "BlueSlime.h"

#include "BmpMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "AbstractFactory.h"

CBlueSlime::CBlueSlime()
{
}

CBlueSlime::~CBlueSlime()
{
	Release();
}

void CBlueSlime::Initialize()
{
	m_tInfo = {0, 0, 52.f, 54.f};
	m_pFrameKey = L"Slime_Blue_Left";
	m_fSpeed = 3.0f;
	m_eDir = UP;
	m_eCurState = IDLE;
	m_iMaxHp = 2;
	m_iHp = 2;
	m_iAttack = 2;
	m_eRenderID = RENDER_GAME_OBJECT;
	m_eObjID = OBJ_MONSTER;
	m_iNeedToDoAct = 2;
	m_iAlphaPosY = -18;
	Set_Heart(m_iMaxHp);
}

int CBlueSlime::Update()
{
	if (m_bDead)
	{
		Draw_Particle(L"Particle_Blue");
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

	__super::Update_Rect();
	Update_Heart();

	return OBJ_NOEVENT;
}

void CBlueSlime::Late_Update()
{
	Move();
	Motion_Change();
	__super::Move_Frame();
}

void CBlueSlime::Render(HDC hDC)
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

void CBlueSlime::Release()
{
}

void CBlueSlime::Do_Act()
{
	Patrol();
}

void CBlueSlime::Move()
{
	if (m_bMove)
	{
		m_fMoveTime += 0.5f;

		if (m_eDir == UP)
		{
			m_tInfo.fY -= 4.8f;
			if (m_fPreYPos - TILECY > m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos - TILECY;
				m_fMoveTime = 0.f;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(this));
				m_eDir = DOWN;
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
				m_eDir = UP;
				m_eCurState = IDLE;
			}
		}
	}
}

void CBlueSlime::Patrol()
{
	auto pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this));
	assert(pTile);

	const int iDir = (m_eDir == UP) ? 0 : 2;

	auto pDestTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
		m_tInfo.fX + (float)(iDx[iDir] * TILECX),
		m_tInfo.fY + (float)(iDy[iDir] * TILECY)));

	if (!pDestTile)
		return;

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

		return;
	}

	if (pDestTile->Get_Owner()->Get_ObjID() == OBJ_PLAYER)
	{
		CObj* pEffect = nullptr;
		TCHAR* pKey = L"";

		switch (m_eDir)
		{
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
		CSoundMgr::Get_Instance()->PlaySoundW(L"en_slime_attack.ogg", SOUND_MONSTER_ATTACK, 0.5f);

		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
		CObjMgr::Get_Instance()->Player_Damaged(m_iAttack);
	}
}

void CBlueSlime::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eDir)
		{
		case UP:
		case DOWN:
			m_pFrameKey = L"Slime_Blue_Left";
			m_tFrame = FRAME{0, 7, 0, (DWORD)m_iBpm / 4, GetTickCount()};
			break;
		}

		m_ePreState = m_eCurState;
	}
}

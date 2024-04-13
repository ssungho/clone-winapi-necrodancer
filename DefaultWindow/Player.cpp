#include "stdafx.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "Effect.h"
#include "Inven.h"
#include "Miss.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Tile.h"
#include "TileMgr.h"
#include "Trap.h"
#include "Wall.h"

CPlayer::CPlayer()
	: m_ePreState(ST_END), m_eCurState(IDLE),
	  m_iArmor(0), m_iWeapon(0), m_iCombo(0), m_eNextDir(DIR_END), m_pPreTile(nullptr),
	  m_fShadowPosY(0), m_bIsShake(false), m_dwShakeTime(0), m_iShakeCnt(0),
	  m_bPlayerDead(false), m_iAttackCombo(0)
{
	// 위치 초기화
	ZeroMemory(&m_tPos2D, sizeof(VEC2));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_eObjID = OBJ_PLAYER;
	m_tInfo = {WINCX / 2.f, WINCY / 2.f, 48.f, 48.f};
	m_fSpeed = 3.0f;
	m_iMaxHp = 6;
	m_iHp = m_iMaxHp;

	m_eCurState = IDLE;
	m_tFrame = {0, 3, 0, 522 / 4, GetTickCount()};

	m_pFrameKey = L"Player_Head";
	m_pBodyFrameKey = L"Player_Body";
	m_pShadowFrameKey = L"Standard_Shadow_Right";
	m_pDeathFrameKey = L"Player_Death";
	m_eRenderID = RENDER_GAME_OBJECT;
	m_fShadowPosY = (float)m_tRect.top;
	m_bPlayerDead = false;
}

int CPlayer::Update()
{
	if (m_bPlayerDead)
	{
		return OBJ_NOEVENT;
	}

	if (m_iHp <= 0)
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"vo_cad_death_03.ogg", SOUND_PLAYER_DEAD, 0.5f);
		m_bPlayerDead = true;
		m_eDeadPoint = CSceneMgr::Get_Instance()->Get_Current_SceneID();
		m_eObjID = OBJ_WALL;
		static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile_On_Player())->Leave_Here();
	}

	Key_Input();
	Offset();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	Move();
	Motion_Change();
	Shake();
	__super::Move_Frame();
}

void CPlayer::Render(HDC hDC)
{
	int iAlphaY = -18;

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bPlayerDead)
	{
		HDC hDeadDC = CBmpMgr::Get_Instance()->Find_Image(m_pDeathFrameKey);
		GdiTransparentBlt(hDC,
		                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		                  hDeadDC,
		                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		                  m_tFrame.iMotion * (int)m_tInfo.fCY,
		                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		                  MAGENTA);
		return;
	}


	HDC hHeadDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	HDC hShadowDC = CBmpMgr::Get_Instance()->Find_Image(m_pShadowFrameKey);
	HDC hBodyDC = CBmpMgr::Get_Instance()->Find_Image(m_pBodyFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_fShadowPosY + iScrollY + iAlphaY - 24,
	                  48, 52,
	                  hShadowDC,
	                  0, 0,
	                  48, 52,
	                  MAGENTA);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY + iAlphaY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hBodyDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  (m_tFrame.iMotion + m_iArmor) * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY + iAlphaY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hHeadDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	if (!CObjMgr::Get_Instance()->On_Beat())
	{
		// 빗나감 출력, 콤보 초기화 (뭔가 싱크가 안맞음)
		if (GET_SINGLE(CKeyMgr)->Get_Arrow_Input())
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_UI,
			                                    CAbstractFactory<CMiss>::Create(
				                                    (float)(WINCX * 0.5f), (float)(HEART_POSY), m_iBpm, true)
			);
			m_iCombo = 0;
		}
		return;
	}

	// 움직이지 않을 때만 입력 가능!
	if (!m_bMove)
	{
		if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::LEFT))
		{
			m_eDir = LEFT;

			if (!m_bMove && Check_Tile())
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			m_pFrameKey = L"Player_Head_Flip";
			m_pBodyFrameKey = L"Player_Body_Flip";
			m_pShadowFrameKey = L"Standard_Shadow_Left";

			++m_iCombo;
		}

		else if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::RIGHT))
		{
			m_eDir = RIGHT;

			if (!m_bMove && Check_Tile())
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			m_pFrameKey = L"Player_Head";
			m_pBodyFrameKey = L"Player_Body";
			m_pShadowFrameKey = L"Standard_Shadow_Right";


			++m_iCombo;
		}

		else if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::UP))
		{
			m_eDir = UP;

			if (!m_bMove && Check_Tile())
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			if (m_eDir == LEFT)
			{
				m_pFrameKey = L"Player_Head_Flip";
				m_pBodyFrameKey = L"Player_Body_Flip";
				m_pShadowFrameKey = L"Standard_Shadow_Left";
			}

			else if (m_eDir == RIGHT)
			{
				m_pFrameKey = L"Player_Head";
				m_pBodyFrameKey = L"Player_Body";
				m_pShadowFrameKey = L"Standard_Shadow_Right";
			}

			++m_iCombo;
		}

		else if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::DOWN))
		{
			m_eDir = DOWN;

			if (!m_bMove && Check_Tile())
			{
				m_bMove = true;
				m_fPreYPos = m_tInfo.fY;
			}

			if (m_eDir == LEFT)
			{
				m_pFrameKey = L"Player_Head_Flip";
				m_pBodyFrameKey = L"Player_Body_Flip";
				m_pShadowFrameKey = L"Standard_Shadow_Left";
			}

			else if (m_eDir == RIGHT)
			{
				m_pFrameKey = L"Player_Head";
				m_pBodyFrameKey = L"Player_Body";
				m_pShadowFrameKey = L"Standard_Shadow_Right";
			}

			++m_iCombo;
		}

		else if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::SPACE))
		{
			if (!m_bMove && CInven::Get_Instance()->Can_Spell())
			{
				Spell();
				++m_iCombo;
				CObjMgr::Get_Instance()->Hit_Beat();
			}
		}

		if (m_bMove)
		{
			CObjMgr::Get_Instance()->Hit_Beat();
		}
	}
}

void CPlayer::Offset()
{
	// TODO: OFFSET 수정 필요함

	int iOffSetminX = WINCX / 2 - 12;
	int iOffSetmaxX = WINCX / 2 + 12;

	int iOffSetminY = WINCY / 2 - 12;
	int iOffSetmaxY = WINCY / 2 + 12;

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	// 4.8이었음.

	if ((float)iOffSetminX > m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(/*m_fSpeed*/4.8f);

	if ((float)iOffSetmaxX < m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(/*-m_fSpeed*/-4.8f);

	if ((float)iOffSetminY > m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if ((float)iOffSetmaxY < m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);
}

void CPlayer::Motion_Change()
{
	if (m_bPlayerDead)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 0;
		m_tFrame.dwTime = GetTickCount();
		return;
	}

	if (m_ePreState != m_eCurState)
	{
		const int iSpeed = 100;

		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = iSpeed;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = iSpeed;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = iSpeed;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = iSpeed;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = iSpeed;
			m_tFrame.dwTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPlayer::Clothes_Change()
{
	// BUG: 누수포인트
	auto a = CInven::Get_Instance()->Get_Item(ITEM_ARMOR)->Get_FrameKey();

	if (!lstrcmp(L"Spiked_Leather", CInven::Get_Instance()->Get_Item(ITEM_ARMOR)->Get_FrameKey()))
		m_iArmor = 1;
	else if (!lstrcmp(L"Chain_Mail", CInven::Get_Instance()->Get_Item(ITEM_ARMOR)->Get_FrameKey()))
		m_iArmor = 2;
	else
		m_iArmor = 0;
}

void CPlayer::Shake()
{
	if (m_bIsShake)
	{
		if (m_dwShakeTime + 20 < GetTickCount())
		{
			if (m_iShakeCnt % 4 == 1)
				CScrollMgr::Get_Instance()->Set_ScrollX(-5);
			else if (m_iShakeCnt % 4 == 2)
				CScrollMgr::Get_Instance()->Set_ScrollX(+5);
			else if (m_iShakeCnt % 4 == 3)
				CScrollMgr::Get_Instance()->Set_ScrollY(-5);
			else if (m_iShakeCnt % 4 == 0)
				CScrollMgr::Get_Instance()->Set_ScrollY(+5);

			++m_iShakeCnt;
			m_dwShakeTime = GetTickCount();
		}

		if (m_iShakeCnt > 8)
		{
			m_bIsShake = false;
			m_iShakeCnt = 0;
		}
	}
}

void CPlayer::Move()
{
	if (m_bMove)
	{
		m_iAttackCombo = 0;
		m_fMoveTime += 0.5f;

		// 대충 MoveTime 5에 도착
		if (m_eDir == RIGHT)
		{
			m_tInfo.fX += 4.8f;
			m_tInfo.fY -= static_cast<float>(sin(45)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) *
				0.5f;
			m_fShadowPosY += (float)(sin(45)) * m_fSpeed * m_fMoveTime - 1.2f * (m_fMoveTime * m_fMoveTime) * 0.5f;

			if (m_fPreYPos < m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos;
				m_fShadowPosY = m_fPreYPos;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY));
				m_fMoveTime = 0.f;

				if (static_cast<CTile*>(CTileMgr::Get_Instance()->
						Get_Tile(m_tInfo.fX, m_tInfo.fY))->Is_Stair() &&
					CInven::Get_Instance()->HasKey())
				{
					if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_01)
						CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE_02);
					else if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_02)
						CSceneMgr::Get_Instance()->Scene_Change(SC_BOSS_STAGE);
				}
			}
		}

		else if (m_eDir == LEFT)
		{
			m_tInfo.fX -= 4.8f;
			m_tInfo.fY -= static_cast<float>(sin(45)) * m_fSpeed * m_fMoveTime - 1.47f * (m_fMoveTime * m_fMoveTime) *
				0.5f;
			m_fShadowPosY += (sin(45)) * m_fSpeed * m_fMoveTime - 1.2f * (m_fMoveTime * m_fMoveTime) * 0.5f;

			if (m_fPreYPos < m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos;
				m_fShadowPosY = m_fPreYPos;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY));
				m_fMoveTime = 0.f;


				if (static_cast<CTile*>(CTileMgr::Get_Instance()->
						Get_Tile(m_tInfo.fX, m_tInfo.fY))->Is_Stair() &&
					CInven::Get_Instance()->HasKey())
				{
					if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_01)
						CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE_02);
					else if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_02)
						CSceneMgr::Get_Instance()->Scene_Change(SC_BOSS_STAGE);
				}
			}
		}

		else if (m_eDir == UP)
		{
			m_tInfo.fY -= 4.8f;
			m_fShadowPosY -= 3.2f;
			if (m_fPreYPos - TILECY > m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos - TILECY;
				m_fShadowPosY = m_fPreYPos - TILECY;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY));
				m_fMoveTime = 0.f;


				if (static_cast<CTile*>(CTileMgr::Get_Instance()->
						Get_Tile(m_tInfo.fX, m_tInfo.fY))->Is_Stair() &&
					CInven::Get_Instance()->HasKey())
				{
					if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_01)
						CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE_02);
					else if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_02)
						CSceneMgr::Get_Instance()->Scene_Change(SC_BOSS_STAGE);
				}
			}
		}

		else if (m_eDir == DOWN)
		{
			m_tInfo.fY += 4.8f;
			m_fShadowPosY += 5.4f;
			if (m_fPreYPos + TILECY < m_tInfo.fY)
			{
				m_bMove = false;
				m_tInfo.fY = m_fPreYPos + TILECY;
				m_fShadowPosY = m_fPreYPos + TILECY;
				Set_Pos(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY));
				m_fMoveTime = 0.f;


				if (static_cast<CTile*>(CTileMgr::Get_Instance()->
						Get_Tile(m_tInfo.fX, m_tInfo.fY))->Is_Stair() &&
					CInven::Get_Instance()->HasKey())
				{
					if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_01)
						CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE_02);
					else if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_02)
						CSceneMgr::Get_Instance()->Scene_Change(SC_BOSS_STAGE);
				}
			}
		}

		// TODO : 트랩 테스트중
		if (!m_bMove && m_eNextDir != DIR_END)
		{
			m_eDir = m_eNextDir;
			m_eNextDir = DIR_END;

			float fDestPosX = m_tInfo.fX + (float)(iDx[(int)m_eDir] * TILECX);
			float fDestPosY = m_tInfo.fY + (float)(iDy[(int)m_eDir] * TILECY);

			auto pDestTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(fDestPosX, fDestPosY));
			assert(pDestTile);

			// 자리 교체
			static_cast<CTile*>(m_pPreTile)->Leave_Here();
			pDestTile->Set_Owner(this);
			m_bMove = true;
			m_fPreYPos = m_tInfo.fY;
		}
	}
}

void CPlayer::Attack()
{
	CObj* pEffect = CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir);

	auto pGetKey = CInven::Get_Instance()->Get_Item(ITEM_WEAPON)->Get_FrameKey();

	if (!lstrcmp(pGetKey, L"Dagger"))
	{
		switch (m_eDir)
		{
		case LEFT:
			pEffect->Set_FrameKey(L"Attack_Dagger_Left");
			pEffect->Set_PosX(-TILECX);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			break;

		case RIGHT:
			pEffect->Set_FrameKey(L"Attack_Dagger_Right");
			pEffect->Set_PosX(TILECX);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			break;

		case UP:
			pEffect->Set_FrameKey(L"Attack_Dagger_Up");
			pEffect->Set_PosY(-TILECY);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			break;

		case DOWN:
			pEffect->Set_FrameKey(L"Attack_Dagger_Down");
			pEffect->Set_PosY(TILECY);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			break;
		}

		pEffect->Set_Scale(48.f, 48.f);
		static_cast<CEffect*>(pEffect)->Set_EffectType(EFFECT_DAGGER);
	}
	else if (!lstrcmp(pGetKey, L"Broadsword"))
	{
		switch (m_eDir)
		{
		case LEFT:
			pEffect->Set_FrameKey(L"Attack_Sword_Left");
			pEffect->Set_PosX(-TILECX);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(48.f, 144.f);
			break;

		case RIGHT:
			pEffect->Set_FrameKey(L"Attack_Sword_Right");
			pEffect->Set_PosX(TILECX);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(48.f, 144.f);
			break;

		case UP:
			pEffect->Set_FrameKey(L"Attack_Sword_Up");
			pEffect->Set_PosY(-TILECY);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(144.f, 48.f);
			break;

		case DOWN:
			pEffect->Set_FrameKey(L"Attack_Sword_Down");
			pEffect->Set_PosY(TILECY);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(144.f, 48.f);
			break;
		}


		static_cast<CEffect*>(pEffect)->Set_EffectType(EFFECT_SWORD);
	}
	else if (!lstrcmp(pGetKey, L"Broadsword_T"))
	{
		switch (m_eDir)
		{
		case LEFT:
			pEffect->Set_FrameKey(L"Attack_Sword_T_Left");
			pEffect->Set_PosX(-TILECX);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(48.f, 144.f);
			break;

		case RIGHT:
			pEffect->Set_FrameKey(L"Attack_Sword_T_Right");
			pEffect->Set_PosX(TILECX);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(48.f, 144.f);
			break;

		case UP:
			pEffect->Set_FrameKey(L"Attack_Sword_T_Up");
			pEffect->Set_PosY(-TILECY);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(144.f, 48.f);
			break;

		case DOWN:
			pEffect->Set_FrameKey(L"Attack_Sword_T_Down");
			pEffect->Set_PosY(TILECY);
			pEffect->Set_Frame(FRAME{0, 2, 0, 50, GetTickCount()});
			pEffect->Set_Scale(144.f, 48.f);
			break;
		}


		static_cast<CEffect*>(pEffect)->Set_EffectType(EFFECT_SWORD);
	}

	if (m_iAttackCombo <= 1)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER_ATTACK1);
		CSoundMgr::Get_Instance()->PlaySoundW(L"vo_cad_ranged_01.ogg", SOUND_PLAYER_ATTACK1, 0.5f);
	}
	else if (m_iAttackCombo <= 2)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER_ATTACK1);
		CSoundMgr::Get_Instance()->PlaySoundW(L"vo_cad_melee_4_01.ogg", SOUND_PLAYER_ATTACK1, 0.5f);
	}
	else
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER_ATTACK1);
		CSoundMgr::Get_Instance()->PlaySoundW(L"vo_cad_ranged_04.ogg", SOUND_PLAYER_ATTACK1, 0.5f);
	}

	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
}

bool CPlayer::Check_Tile()
{
	// 방향에 맞게 초기화
	VEC2 vPos;
	switch (m_eDir)
	{
	case LEFT: vPos = {m_tInfo.fX - TILECX, m_tInfo.fY};
		break;
	case RIGHT: vPos = {m_tInfo.fX + TILECX, m_tInfo.fY};
		break;
	case UP: vPos = {m_tInfo.fX, m_tInfo.fY - TILECY};
		break;
	case DOWN: vPos = {m_tInfo.fX, m_tInfo.fY + TILECY};
		break;
	}

	auto pDestTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(vPos.x, vPos.y));

	if (pDestTile == nullptr)
		return false;

	auto pOwner = pDestTile->Get_Owner();
	auto pEquip = CInven::Get_Instance()->Get_Item(ITEM_WEAPON);
	vector<CTile*> vecSideTile;
	bool bSwordAttack = false;

	if (pEquip)
	{
		if (!lstrcmp(pEquip->Get_FrameKey(), L"Broadsword") ||
			!lstrcmp(pEquip->Get_FrameKey(), L"Broadsword_T"))
		{
			switch (m_eDir)
			{
			case LEFT:
			case RIGHT:
				vecSideTile.
					push_back(dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(vPos.x, vPos.y - TILECY)));
				vecSideTile.
					push_back(dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(vPos.x, vPos.y + TILECY)));
				break;
			case UP:
			case DOWN:
				vecSideTile.
					push_back(dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(vPos.x - TILECX, vPos.y)));
				vecSideTile.
					push_back(dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(vPos.x + TILECX, vPos.y)));
				break;
			}

			for (unsigned int i = 0; i < vecSideTile.size(); ++i)
			{
				if (vecSideTile[i] == nullptr)
					continue;

				if (vecSideTile[i]->Get_Owner() != nullptr &&
					vecSideTile[i]->Get_Owner()->Get_ObjID() == OBJ_MONSTER)
				{
					bSwordAttack = true;
					Attack();
					vecSideTile[i]->Get_Owner()->On_Damaged(CInven::Get_Instance()->Get_Attack());
					vecSideTile[i]->Get_Owner()->Set_IsAttacked();
					if (vecSideTile[i]->Get_Owner()->Get_Hp() <= 0)
						vecSideTile[i]->Leave_Here();

					if (!m_bIsShake)
					{
						m_bIsShake = true;
						m_dwShakeTime = GetTickCount();
					}
				}
			}

			if (pOwner)
			{
				if (pOwner->Get_ObjID() == OBJ_MONSTER)
				{
					bSwordAttack = true;
					Attack();
					pOwner->On_Damaged(CInven::Get_Instance()->Get_Attack());
					pOwner->Set_IsAttacked();
					if (pOwner->Get_Hp() <= 0)
						pDestTile->Leave_Here();

					if (!m_bIsShake)
					{
						m_bIsShake = true;
						m_dwShakeTime = GetTickCount();
					}
				}
			}

			if (bSwordAttack)
			{
				++m_iAttackCombo;
				++m_iCombo;
				return false;
			}
		}
	}

	if (pOwner)
	{
		auto eObjID = pOwner->Get_ObjID();

		if (eObjID == OBJ_TRAP)
		{
			auto eTrapID = static_cast<CTrap*>(pOwner)->Get_TrapID();
			if (eTrapID == TRAP_BOUNCE)
			{
				auto eDir = pOwner->Get_Dir();
				m_eNextDir = eDir;
				m_pPreTile = CTileMgr::Get_Instance()->Get_Tile_On_Player();
				return true;
			}
		}

		// 소유자가 벽일 경우
		if (eObjID == OBJ_WALL && CInven::Get_Instance()->Can_DestroyWall())
		{
			auto pEffect = CAbstractFactory<CEffect>::Create(pDestTile);

			static_cast<CEffect*>(pEffect)->Set_Effect(
				CInven::Get_Instance()->Get_Item(ITEM_SHOVEL)->Get_FrameKey(), EFFECT_TOOL, 48.f, 48.f, 250);

			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

			auto pWall = static_cast<CWall*>(pOwner);
			if (pWall->Get_WallID() == BOSS)
				return false;

			if (pWall->Get_WallID() == NORMAL ||
				pWall->Get_WallID() == DOOR_H ||
				pWall->Get_WallID() == DOOR_V)
			{
				//Draw_Particle(L"Particle_Dirt");
				pWall->Set_Dead();
				pDestTile->Leave_Here();
			}

			if (pWall->Get_WallID() == STONE ||
				pWall->Get_WallID() == SHOP)
			{
				if (!lstrcmp(L"Pickaxe", CInven::Get_Instance()->Get_Item(ITEM_SHOVEL)->Get_FrameKey()))
					pWall->Set_Hp(pWall->Get_Hp() - 1);
				if (pWall->Get_Hp() <= 0)
				{
					//Draw_Particle(L"Particle_Dirt");
					pWall->Set_Dead();
					pDestTile->Leave_Here();
				}
			}

			return false;
		}

		// 소유자가 몬스터일 경우
		if (eObjID == OBJ_MONSTER && CInven::Get_Instance()->Can_Attack())
		{
			Attack();
			pOwner->On_Damaged(CInven::Get_Instance()->Get_Attack());
			pOwner->Set_IsAttacked();

			if (pOwner->Get_Hp() <= 0)
				pDestTile->Leave_Here();

			if (!m_bIsShake)
			{
				m_bIsShake = true;
				m_dwShakeTime = GetTickCount();
			}

			++m_iAttackCombo;
			++m_iCombo;
			return false;
		}

		// 소유자가 아이템일 경우
		if (eObjID == OBJ_ITEM)
		{
			int iCoinPrice = pOwner->Get_Coin();
			int iDiamondPrice = pOwner->Get_Diamond();

			if (iCoinPrice > 0 || iDiamondPrice > 0)
			{
				if (!CInven::Get_Instance()->Buy(iCoinPrice, iDiamondPrice))
				{
					// UI생성 필요
					return false;
				}
			}

			static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this))->Leave_Here();
			CInven::Get_Instance()->Pickup_Item(pOwner);
			pDestTile->Set_Owner(this);
			return true;
		}
	}

	// 아무것도 없을 경우 그냥 이동만 하면 됨.
	else
	{
		static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this))->Leave_Here();
		pDestTile->Set_Owner(this);
	}

	return true;
}

void CPlayer::Spell()
{
	stack<CTile*> tileStack;

	int iDirX;
	int iDirY;
	TCHAR* pKey = L"";
	switch (m_eDir)
	{
	case UP: iDirX = 0;
		iDirY = -1;
		pKey = L"Fireball_V";
		break;
	case DOWN: iDirX = 0;
		iDirY = 1;
		pKey = L"Fireball_V";
		break;
	case LEFT: iDirX = -1;
		iDirY = 0;
		pKey = L"Fireball";
		break;
	case RIGHT: iDirX = 1;
		iDirY = 0;
		pKey = L"Fireball";
		break;
	}

	int iCnt = 1;

	while (true)
	{
		auto pObj = CTileMgr::Get_Instance()->Get_Tile(
			m_tInfo.fX + (float)(iDirX * iCnt * TILECX),
			m_tInfo.fY + (float)(iDirY * iCnt * TILECY));

		auto pTile = dynamic_cast<CTile*>(pObj);

		if (pObj == nullptr || pTile == nullptr)
			return;

		if (pTile->Get_Owner() != nullptr)
		{
			if (pTile->Get_Owner()->Get_ObjID() != OBJ_WALL)
			{
				tileStack.push(pTile);
			}
			else
			{
				break;
			}
		}
		else
		{
			tileStack.push(pTile);
		}
		++iCnt;
	}

	while (!tileStack.empty())
	{
		auto pDestTile = tileStack.top();
		CObj* pFireball = CAbstractFactory<CEffect>::Create(
			pKey, 48.f, 48.f, pDestTile, m_iBpm);

		static_cast<CEffect*>(pFireball)->Set_EffectType(EFFECT_FIREBALL);
		pFireball->Set_Frame({0, 6, 0, 20, GetTickCount()});
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pFireball);

		auto pOwner = pDestTile->Get_Owner();
		if (pOwner != nullptr)
		{
			auto eID = pOwner->Get_ObjID();
			if (eID == OBJ_MONSTER)
			{
				pOwner->On_Damaged(3);
			}
		}

		tileStack.pop();
	}
	CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER_SPELL);
	CSoundMgr::Get_Instance()->PlaySoundW(L"wep_spell_transmute.ogg", SOUND_PLAYER_SPELL, 0.5f);
}

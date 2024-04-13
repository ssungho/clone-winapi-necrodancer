#include "stdafx.h"
#include "BossStage.h"

#include "AbstractFactory.h"
#include "Beat.h"
#include "BeatHeart.h"
#include "BmpMgr.h"
#include "DeathMetal.h"
#include "Inven.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "WallMgr.h"

CBossStage::CBossStage()
	: m_bOnCutScene(false), m_bStop(false), m_dwCutSceneTimer(0), m_fMoveDist(0), m_dwTimer(0),
	  m_bCanInput(false), m_iStageBpm(0)
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	m_iStageBpm = 350;
	CTileMgr::Get_Instance()->Reset_RestoreTile();

	Load_Stage();

	auto pTile = CTileMgr::Get_Instance()->Get_Tile(705);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(pTile);

	auto pSpellTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(
		pTile->Get_Info().fX, pTile->Get_Info().fY - TILECY * 3));

	auto pSpell = CAbstractFactory<CItem>::Create_Item(ITEM_SPELL, L"Fire_Spell", pSpellTile);
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pSpell);

	// 리듬 관련 UI
	{
		CObj* pBeatHeart = CAbstractFactory<CBeatHeart>::Create();
		CObjMgr::Get_Instance()->Set_BeatHeart(pBeatHeart);
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pBeatHeart);
	}

	// 보스 몬스터 생성
	{
		pTile = CTileMgr::Get_Instance()->Get_Tile(225);
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER,
		                                    CAbstractFactory<CDeathMetal>::Create(
			                                    pTile->Get_Info().fX, pTile->Get_Info().fY, m_iStageBpm));
	}

	// 브금재생
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlayBGM(L"boss_stage.ogg", 0.5f);
	}

	CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_CUT_SCENE);
	CSoundMgr::Get_Instance()->PlaySoundW(L"vo_announcer_deathmetal.ogg", SOUND_BOSS_CUT_SCENE, 0.5f);

	m_bOnCutScene = true;
}

int CBossStage::Update()
{
	if (static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Player_Dead())
	{
		if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::ENTER))
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_LOADING);
		}
	}

	if (m_bOnCutScene && m_bStop)
	{
		if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::ENTER))
			m_bOnCutScene = false;
	}

	if (GET_SINGLE(CKeyMgr)->Get_Button_Down(KeyType::KEY_0))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_MENU);
		CSoundMgr::Get_Instance()->StopAll();
	}

	Create_Beat(m_iStageBpm);
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CInven::Get_Instance()->Update();

	return 0;
}

void CBossStage::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update_Boss_Stage();
	CObjMgr::Get_Instance()->Late_Update();
	CInven::Get_Instance()->Late_Update();
}

void CBossStage::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
	CInven::Get_Instance()->Render(hDC);

	if (static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Player_Dead())
	{
		Draw_Dead_Popup(hDC);
	}

	if (m_bOnCutScene)
	{
		Play_CutScene(hDC);
	}
}

void CBossStage::Release()
{
	CTileMgr::Get_Instance()->Release();
	CWallMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Delete_Scene_Obj();
}

void CBossStage::Load_Stage()
{
	CTileMgr::Get_Instance()->Load_Boss_Stage_Tile();
	CWallMgr::Get_Instance()->Set_Boss_Stage_Wall();
}

void CBossStage::Create_Beat(int _iBpm)
{
	if (m_dwTimer + _iBpm < GetTickCount())
	{
		CObj* pObj = CAbstractFactory<CBeat>::Create(0, BEAT_POSY, RIGHT);
		CObjMgr::Get_Instance()->Add_Object(OBJ_BEAT, pObj);

		pObj = CAbstractFactory<CBeat>::Create(WINCX, BEAT_POSY, LEFT);
		CObjMgr::Get_Instance()->Add_Object(OBJ_BEAT, pObj);

		m_dwTimer = GetTickCount();
	}
}

void CBossStage::Play_CutScene(HDC hDC)
{
	if (!m_bStop)
		m_fMoveDist += 12.f;

	HDC alphaDC = CBmpMgr::Get_Instance()->Find_Image(L"Alpha_Black");
	GdiAlphaBlend(
		hDC, 0, 0, 800, 600,
		alphaDC,
		0, 0, 800, 600,
		{AC_SRC_OVER, 0, 200, 0});

	HDC topDC = CBmpMgr::Get_Instance()->Find_Image(L"Cut_Top");
	HDC middleDC = CBmpMgr::Get_Instance()->Find_Image(L"Cut_Middle");
	HDC bossDC = CBmpMgr::Get_Instance()->Find_Image(L"Cut_Death_Metal");
	HDC bottomDC = CBmpMgr::Get_Instance()->Find_Image(L"Cut_Bottom");

	if (m_fMoveDist >= 600.f)
	{
		m_fMoveDist = 600.f;
		m_bStop = true;
	}

	if (m_dwCutSceneTimer + 10 < GetTickCount())
	{
		GdiTransparentBlt(
			hDC,
			0, 175,
			(int)m_fMoveDist, 68,
			topDC,
			602 - (int)m_fMoveDist, 0,
			(int)m_fMoveDist, 68,
			MAGENTA);

		GdiTransparentBlt(
			hDC,
			WINCX - (int)(m_fMoveDist * 1.33f), 250,
			(int)(m_fMoveDist * 1.33f), 200,
			middleDC,
			0, 0,
			(int)(m_fMoveDist * 1.33f), 200,
			MAGENTA);

		GdiTransparentBlt(
			hDC,
			WINCX - (int)(m_fMoveDist * 1.5f), 50,
			(int)(m_fMoveDist * 1.5f), 398,
			bossDC,
			0, 0,
			(int)(m_fMoveDist * 1.5f), 398,
			MAGENTA);

		GdiTransparentBlt(
			hDC,
			WINCX - (int)m_fMoveDist, WINCY - 150,
			(int)m_fMoveDist, 96,
			bottomDC,
			0, 0,
			(int)m_fMoveDist, 96,
			MAGENTA);
	}
}

void CBossStage::Draw_Dead_Popup(HDC hDC)
{
	HDC alphaDC = CBmpMgr::Get_Instance()->Find_Image(L"Alpha_Black");
	GdiAlphaBlend(
		hDC, 0, 0, 800, 600,
		alphaDC,
		0, 0, 800, 600,
		{AC_SRC_OVER, 0, 225, 0});

	HDC deadPopupDC = CBmpMgr::Get_Instance()->Find_Image(L"Dead_Popup");
	GdiTransparentBlt(
		hDC,
		0, 0,
		WINCX, WINCY,
		deadPopupDC,
		0, 0,
		WINCX, WINCY,
		MAGENTA);
}

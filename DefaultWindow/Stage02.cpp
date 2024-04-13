#include "stdafx.h"
#include "Stage02.h"

#include "AbstractFactory.h"
#include "Bat.h"
#include "Beat.h"
#include "BeatHeart.h"
#include "BmpMgr.h"
#include "GreenDragon.h"
#include "GreenSlime.h"
#include "Inven.h"
#include "KeyMgr.h"
#include "Minotaur.h"
#include "ObjMgr.h"
#include "OrangeSlime.h"
#include "Player.h"
#include "RedDragon.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "Trap.h"
#include "WallMgr.h"
#include "YellowSkeleton.h"
#include "Zombie.h"

CStage02::CStage02()
	: m_dwTimer(0), m_bCanInput(false), m_iStageBpm(0), m_bOnFade(false), m_byteFadeTime(0)
{
}

CStage02::~CStage02()
{
	Release();
}

void CStage02::Initialize()
{
	m_iStageBpm = 445;
	Load_Stage();
	m_bOnFade = true;
	m_byteFadeTime = 400;
}

int CStage02::Update()
{
	if (static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Player_Dead())
	{
		if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::ENTER))
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_LOADING);
		}
	}

	if (m_bOnFade)
	{
		--m_byteFadeTime;
		if (m_byteFadeTime < 1)
		{
			m_byteFadeTime = 0;
			m_bOnFade = false;
		}
	}

	Create_Beat(m_iStageBpm);
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CInven::Get_Instance()->Update();

	return 0;
}

void CStage02::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update_Stage02_Tile();
	CObjMgr::Get_Instance()->Late_Update();
	CInven::Get_Instance()->Late_Update();
}

void CStage02::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
	CInven::Get_Instance()->Render(hDC);

	if (m_bOnFade)
	{
		BYTE byteFade = 254;
		if (m_byteFadeTime < 255)
			byteFade = static_cast<BYTE>(m_byteFadeTime);

		HDC alphaDC = CBmpMgr::Get_Instance()->Find_Image(L"Alpha_Black");
		GdiAlphaBlend(
			hDC, 0, 0, 800, 600,
			alphaDC,
			0, 0, 800, 600,
			{AC_SRC_OVER, 0, byteFade, 0});
	}

	if (static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Player_Dead())
	{
		Draw_Dead_Popup(hDC);
	}
}

void CStage02::Release()
{
	CWallMgr::Get_Instance()->Release();
	CTileMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Delete_Scene_Obj();
}

void CStage02::Load_Stage()
{
	CTileMgr::Get_Instance()->Load_Stage02_Tile(m_iStageBpm);
	CWallMgr::Get_Instance()->Set_Stage02_Wall();

	CObj* pBeatHeart = CAbstractFactory<CBeatHeart>::Create();
	CObjMgr::Get_Instance()->Set_BeatHeart(pBeatHeart);
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pBeatHeart);
	// 여기 위는 수정 x

	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(366, 366);

	// TODO : 열쇠 생성, 몬스터 불러오기, 아이템 등등 추가해야됨
	{
		auto pItem = CAbstractFactory<CItem>::Create_Item(ITEM_ARMOR, L"Chain_Mail",
		                                                  CTileMgr::Get_Instance()->Get_Tile(1032, 312));
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);

		pItem = CAbstractFactory<CItem>::Create_Item(ITEM_WEAPON, L"Broadsword_T",
		                                             CTileMgr::Get_Instance()->Get_Tile(744, 1174));
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);
	}


	auto pZombile = CAbstractFactory<CZombie>::Create(504, 668, m_iStageBpm);
	pZombile->Set_Dir(RIGHT);
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pZombile);
	pZombile = CAbstractFactory<CZombie>::Create(1032, 1080, m_iStageBpm);
	pZombile->Set_Dir(LEFT);
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pZombile);
	pZombile = CAbstractFactory<CZombie>::Create(744, 552, m_iStageBpm);
	pZombile->Set_Dir(RIGHT);
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pZombile);
	pZombile = CAbstractFactory<CZombie>::Create(1080, 488, m_iStageBpm);
	pZombile->Set_Dir(LEFT);
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pZombile);


	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<COrangeSlime>::Create(306, 840, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<COrangeSlime>::Create(888, 840, m_iStageBpm));

	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CYellowSkeleton>::Create(840, 600, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CYellowSkeleton>::Create(888, 408, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CYellowSkeleton>::Create(1128, 504, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CYellowSkeleton>::Create(984, 840, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CYellowSkeleton>::Create(1080, 888, m_iStageBpm));

	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(1128, 984, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(888, 936, m_iStageBpm));

	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMinotaur>::Create(792, 456, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGreenDragon>::Create(456, 888, m_iStageBpm));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CRedDragon>::Create(1128, 840, m_iStageBpm));


	// 열쇠
	auto pItem = CAbstractFactory<CItem>::Create_Item(ITEM_KEY, L"Item_Key",
	                                                  CTileMgr::Get_Instance()->Get_Tile(696, 840));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);

	// 함정
	auto pTrapTile1 = CTileMgr::Get_Instance()->Get_Tile(648, 840);
	auto pTrap = CAbstractFactory<CTrap>::Create(pTrapTile1);
	pTrap->Set_Dir(LEFT);
	pTrap->Set_FrameKey(L"Bounce_Trap_Left");
	static_cast<CTile*>(pTrapTile1)->Set_Owner(pTrap);
	CObjMgr::Get_Instance()->Add_Object(OBJ_TRAP, pTrap);

	pTrapTile1 = CTileMgr::Get_Instance()->Get_Tile(648, 888);
	pTrap = CAbstractFactory<CTrap>::Create(pTrapTile1);
	pTrap->Set_Dir(LEFT);
	pTrap->Set_FrameKey(L"Bounce_Trap_Left");
	static_cast<CTile*>(pTrapTile1)->Set_Owner(pTrap);
	CObjMgr::Get_Instance()->Add_Object(OBJ_TRAP, pTrap);

	// 여기 아래는 수정 x

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"zone1_2.ogg", 0.5f);
}

void CStage02::Create_Beat(int _iBpm)
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

void CStage02::Draw_Dead_Popup(HDC hDC)
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

#include "stdafx.h"
#include "Stage01.h"
#include "AbstractFactory.h"
#include "Bat.h"
#include "Beat.h"
#include "BeatHeart.h"
#include "BlueSlime.h"
#include "BmpMgr.h"
#include "GreenSlime.h"
#include "Inven.h"
#include "KeyMgr.h"
#include "Minotaur.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "ShopKeeper.h"
#include "Skeleton.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "Trap.h"
#include "WallMgr.h"


CStage01::CStage01()
	: m_dwTimer(0), m_bCanInput(false), m_iStageBpm(0)
{
}

CStage01::~CStage01()
{
	Release();
}

void CStage01::Initialize()
{
	// 꼭 초기화 해야함
	m_iStageBpm = 500;

	Load_Stage();
	m_bOnFade = true;
	m_byteFadeTime = 400;
}

int CStage01::Update()
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

void CStage01::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update_Stage1();
	CObjMgr::Get_Instance()->Late_Update();
	CInven::Get_Instance()->Late_Update();
}

void CStage01::Render(HDC hDC)
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

void CStage01::Release()
{
	CWallMgr::Get_Instance()->Release();
	CTileMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Delete_Scene_Obj();
}

void CStage01::Draw_Dead_Popup(HDC hDC)
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

void CStage01::Load_Stage()
{
	CTileMgr::Get_Instance()->Load_Stage01_Tile(m_iStageBpm);
	CWallMgr::Get_Instance()->Set_Stage01_Wall();
	CInven::Get_Instance()->Initialize();

	CObj* pBeatHeart = CAbstractFactory<CBeatHeart>::Create();

	CObjMgr::Get_Instance()->Set_BeatHeart(pBeatHeart);
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pBeatHeart);

	// 플레이어 생성
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(
		                                    CTileMgr::Get_Instance()->Get_Tile(456, 1080), m_iStageBpm));

	// 함정
	{
		// UP 함정
		auto pTrapTile1 = CTileMgr::Get_Instance()->Get_Tile(456 + TILECX * 6, 1080);
		auto pTrap = CAbstractFactory<CTrap>::Create(pTrapTile1);
		pTrap->Set_Dir(LEFT);
		pTrap->Set_FrameKey(L"Bounce_Trap_Left");
		static_cast<CTile*>(pTrapTile1)->Set_Owner(pTrap);
		CObjMgr::Get_Instance()->Add_Object(OBJ_TRAP, pTrap);

		pTrapTile1 = CTileMgr::Get_Instance()->Get_Tile(696, 1128);
		pTrap = CAbstractFactory<CTrap>::Create(pTrapTile1);
		pTrap->Set_Dir(UP);
		pTrap->Set_FrameKey(L"Bounce_Trap_Up");
		static_cast<CTile*>(pTrapTile1)->Set_Owner(pTrap);
		CObjMgr::Get_Instance()->Add_Object(OBJ_TRAP, pTrap);
	}

	// 몬스터 불러오기
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(744, 306, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(792, 408, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(744, 456, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(456, 696, m_iStageBpm));

		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGreenSlime>::Create(984, 648, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGreenSlime>::Create(744, 696, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGreenSlime>::Create(1080, 504, m_iStageBpm));

		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBlueSlime>::Create(936, 600, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBlueSlime>::Create(936, 1128, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBlueSlime>::Create(888, 792, m_iStageBpm));

		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(1128, 1080, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(552, 696, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(456, 792, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(796, 648, m_iStageBpm));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBat>::Create(1080, 984, m_iStageBpm));

		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMinotaur>::Create(1080, 792, m_iStageBpm));

		// 열쇠 생성
		{
			auto pItem = CAbstractFactory<CItem>::Create_Item(ITEM_KEY, L"Item_Key",
			                                                  CTileMgr::Get_Instance()->Get_Tile(840, 984));
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);
		}
	}

	// 상점 주인
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CShopKeeper>::Create(456, 408, m_iStageBpm));
		CTileMgr::Get_Instance()->Get_Tile(408, 408)->Set_FrameKey(L"Shop_Tile");
		CTileMgr::Get_Instance()->Get_Tile(504, 408)->Set_FrameKey(L"Shop_Tile");
	}

	// 상점 아이템
	{
		auto pItem = CAbstractFactory<CItem>::Create_Item(ITEM_WEAPON, L"Broadsword",
		                                                  CTileMgr::Get_Instance()->Get_Tile(408, 456));
		pItem->Set_Coin(10);
		static_cast<CItem*>(pItem)->Set_Sell();
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);

		pItem = CAbstractFactory<CItem>::Create_Item(ITEM_ARMOR, L"Spiked_Leather",
		                                             CTileMgr::Get_Instance()->Get_Tile(456, 456));
		pItem->Set_Coin(20);
		static_cast<CItem*>(pItem)->Set_Sell();
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);

		pItem = CAbstractFactory<CItem>::Create_Item(ITEM_CAP, L"Head_Helmet",
		                                             CTileMgr::Get_Instance()->Get_Tile(504, 456));
		pItem->Set_Coin(30);
		static_cast<CItem*>(pItem)->Set_Sell();
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);


		// 곡갱이 드랍템
		pItem = CAbstractFactory<CItem>::Create_Item(ITEM_SHOVEL, L"Pickaxe",
		                                             CTileMgr::Get_Instance()->Get_Tile(936, 312));
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);
	}

	CSoundMgr::Get_Instance()->PlayBGM(L"zone1_1.ogg", 0.5f);
}

void CStage01::Create_Beat(int _iBpm)
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

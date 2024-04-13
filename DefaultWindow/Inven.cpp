#include "stdafx.h"
#include "Inven.h"
#include "AbstractFactory.h"
#include "Item.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Text.h"
#include "TileMgr.h"

CInven* CInven::m_pInstance = nullptr;

CInven::CInven()
	: m_pairMoneyUI({nullptr, nullptr}), m_iSlotCount(0)
{
	m_vecHeart.reserve(5);
}

CInven::~CInven()
{
	Release();
}

void CInven::Initialize()
{
	// 기본 아이템 제공
	{
		// 삽
		auto pItem = CAbstractFactory<CItem>::Create();
		pItem->Set_FrameKey(L"Shovel_Basic"); // Shovel_Basic
		pItem->Set_Scale(ITEMCX, ITEMCY);
		static_cast<CItem*>(pItem)->Set_ItemID(ITEM_SHOVEL);
		static_cast<CItem*>(pItem)->Set_Slot(true);
		m_mapItem.insert({ITEM_SHOVEL, pItem});

		// 삽 슬롯
		pItem = CAbstractFactory<CItem>::Create();
		pItem->Set_FrameKey(L"Item_Slot1");
		pItem->Set_Scale(SLOTCX, SLOTCY);
		static_cast<CItem*>(pItem)->Set_Slot(true);
		m_mapSlot.insert({ITEM_SHOVEL, pItem});

		// 무기
		pItem = CAbstractFactory<CItem>::Create();
		pItem->Set_FrameKey(L"Dagger");
		pItem->Set_Scale(ITEMCX, ITEMCY);
		pItem->Set_Attack(1);
		static_cast<CItem*>(pItem)->Set_ItemID(ITEM_WEAPON);
		static_cast<CItem*>(pItem)->Set_Slot(true);
		m_mapItem.insert({ITEM_WEAPON, pItem});

		// 무기 슬롯
		pItem = CAbstractFactory<CItem>::Create();
		pItem->Set_FrameKey(L"Item_Slot2");
		pItem->Set_Scale(SLOTCX, SLOTCY);
		static_cast<CItem*>(pItem)->Set_Slot(true);
		m_mapSlot.insert({ITEM_WEAPON, pItem});
	}

	// 체력 유아이 셋팅
	{
		for (int i = 1; i <= 3; ++i)
		{
			auto pItem = CAbstractFactory<CItem>::Create_UI_Item(ITEM_SPECIAL, L"Player_Heart", GUICX, GUICY);
			pItem->Set_Pos((float)P_LIFE_X - (float)(i * GUICX), P_LIFE_Y);
			m_vecHeart.push_back(pItem);
		}
	}

	// 재화 UI 셋팅
	{
		m_pairMoneyUI =
		{
			CAbstractFactory<CItem>::Create_UI_Item(ITEM_SPECIAL, L"UI_Coin", ITEMCX, ITEMCY),
			CAbstractFactory<CItem>::Create_UI_Item(ITEM_SPECIAL, L"UI_Diamond", ITEMCX, ITEMCY)
		};

		m_pairMoneyUI.first->Set_Pos(COINPOSX, COINPOSY);
		m_pairMoneyUI.second->Set_Pos(COINPOSX, COINPOSY + ITEMCY);
	}
}

void CInven::Update()
{
	for (auto& pairSlot : m_mapSlot)
	{
		// 결과값 없음
		pairSlot.second->Update();
	}


	for (auto& pairItem : m_mapItem)
	{
		// 결과값 없음
		pairItem.second->Update();
	}

	for (auto& pHeart : m_vecHeart)
	{
		pHeart->Update();
	}

	if (m_pairMoneyUI.first != nullptr)
	{
		m_pairMoneyUI.first->Update();
		m_pairMoneyUI.second->Update();
	}

	Update_Heart();
}

void CInven::Late_Update()
{
	// 아이템 슬롯 위치 지정
	if (m_iSlotCount != m_mapItem.size())
	{
		int iCnt = 0;
		for (auto& pairItem : m_mapItem)
		{
			pairItem.second->Set_Pos
			(
				(float)(SLOTPOSX + iCnt * (SLOTCX + SLOT_INTER)),
				(float)(SLOTPOSY)
			);
			++iCnt;
		}

		iCnt = 0;
		for (auto& pairSlot : m_mapSlot)
		{
			pairSlot.second->Set_Pos
			(
				(float)(SLOTPOSX + iCnt * (SLOTCX + SLOT_INTER)),
				(float)(SLOTPOSY)
			);
			++iCnt;
		}
	}

	m_pairMoneyUI.first->Late_Update();
	m_pairMoneyUI.second->Late_Update();
}

void CInven::Render(HDC hDC)
{
	for (auto& pairSlot : m_mapSlot)
	{
		pairSlot.second->Render(hDC);
	}

	for (auto& pairItem : m_mapItem)
	{
		pairItem.second->Render(hDC);
	}

	for (auto& pHeart : m_vecHeart)
	{
		pHeart->Render(hDC);
	}

	if (m_pairMoneyUI.first != nullptr)
		m_pairMoneyUI.first->Render(hDC);
	if (m_pairMoneyUI.second != nullptr)
		m_pairMoneyUI.second->Render(hDC);

	Render_Money(hDC);
}

void CInven::Release()
{
	for_each(m_mapItem.begin(), m_mapItem.end(), DeleteMap());
	m_mapItem.clear();

	for_each(m_mapSlot.begin(), m_mapSlot.end(), DeleteMap());
	m_mapSlot.clear();

	for_each(m_vecHeart.begin(), m_vecHeart.end(), DeleteObj());
	m_vecHeart.clear();

	Safe_Delete(m_pairMoneyUI.first);
	Safe_Delete(m_pairMoneyUI.first);
}

bool CInven::Can_DestroyWall()
{
	if (m_mapItem.find(ITEM_SHOVEL) != m_mapItem.end())
		return true;

	return false;
}

bool CInven::Can_Attack()
{
	if (m_mapItem.find(ITEM_WEAPON) != m_mapItem.end())
		return true;

	return false;
}

bool CInven::Can_Spell()
{
	if (m_mapItem.find(ITEM_SPELL) != m_mapItem.end())
		return true;

	return false;
}

void CInven::Pickup_Item(CObj* _pItem)
{
	// 우선 인벤에 넣을 아이템 형변환
	auto pItem = dynamic_cast<CItem*>(_pItem);

	// 성공하면
	if (pItem)
	{
		// 아이디를 가져오고
		auto eItemID = pItem->Get_ItemID();

		// 재화 처리 1
		if (eItemID == ITEM_COIN)
		{
			m_iCoin += pItem->Get_Coin() * 8;
			pItem->Set_Dead();
			return;
		}

		// 재화 처리 2
		if (eItemID == ITEM_DIAMOND)
		{
			m_iCoin += pItem->Get_Diamond() * 2;
			pItem->Set_Dead();
			return;
		}

		// 같은 종류의 아이템이 있는지 확인하고 (가지고있는 같은 종류의 아이템)
		auto pairItem = m_mapItem.find(eItemID);

		// 있으면 아이템만 지워주고, 없으면 슬롯 생성
		if (pairItem != m_mapItem.end())
		{
			Drop_Item(pairItem->second, eItemID);
			Delete_Item(eItemID);
		}
		else
		{
			Make_Slot(eItemID);
		}

		Make_Item(eItemID, pItem);

		if (eItemID == ITEM_ARMOR)
		{
			static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Clothes_Change();
		}
		else if (eItemID == ITEM_KEY)
		{
			if (CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_01 ||
				CSceneMgr::Get_Instance()->Get_Current_SceneID() == SC_STAGE_02)
			{
				CTileMgr::Get_Instance()->Get_Tile(1176, 312)->Set_FrameKey(L"Stair_Tile");
			}
		}
	}

	// 필드에 남은 아이템 제거
	pItem->Set_Dead();
}

void CInven::Drop_Item(CObj* _pItem, ITEMID _eItemID)
{
	CObj* pTile = CTileMgr::Get_Instance()->Get_Tile_On_Player();

	CObjMgr::Get_Instance()->Add_Object(
		OBJ_ITEM, CAbstractFactory<CItem>::Create_Item(
			_eItemID, _pItem->Get_CopyKey(), pTile));
}

bool CInven::Buy(int _iCoin, int _iDiamond)
{
	bool bResult = false;

	if (_iCoin > 0)
	{
		if (_iCoin <= m_iCoin)
		{
			m_iCoin -= _iCoin;
			bResult = true;
		}
	}
	else if (_iDiamond > 0)
	{
		if (_iDiamond <= m_iCoin)
		{
			m_iDiamond -= _iDiamond;
			bResult = true;
		}
	}

	return bResult;
}

void CInven::Make_Item(ITEMID _eItemID, CObj* _pItem)
{
	m_mapItem.insert({
		_eItemID, CAbstractFactory<CItem>::Create_UI_Item(
			_eItemID, _pItem->Get_CopyKey(), ITEMCX, ITEMCY)
	});

	auto pName = _pItem->Get_FrameKey();
	auto tInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	auto pText = CAbstractFactory<CText>::Create(tInfo.fX, tInfo.fY, 300, true);
	TCHAR* pInsertName = L"";
	if (!lstrcmp(pName, L"Shovel_Basic"))
	{
		pInsertName = L"   일반 삽";
	}
	else if (!lstrcmp(pName, L"Pickaxe"))
	{
		pInsertName = L"    곡괭이";
	}
	else if (!lstrcmp(pName, L"Dagger"))
	{
		pInsertName = L" 일반 단검";
	}
	else if (!lstrcmp(pName, L"Broadsword"))
	{
		pInsertName = L" 브로드소드";
	}
	else if (!lstrcmp(pName, L"Broadsword_T"))
	{
		pInsertName = L"티타늄 브로드소드";
	}
	else if (!lstrcmp(pName, L"Spiked_Leather"))
	{
		pInsertName = L"  가죽 갑옷";
	}
	else if (!lstrcmp(pName, L"Chain_Mail"))
	{
		pInsertName = L"  사슬 갑옷";
	}
	else if (!lstrcmp(pName, L"Head_Helmet"))
	{
		pInsertName = L"    투구";
	}
	else if (!lstrcmp(pName, L"Item_Key"))
	{
		pInsertName = L"    열쇠";
	}
	else if (!lstrcmp(pName, L"Fire_Spell"))
	{
		pInsertName = L"  마법 주문";
	}

	pText->Set_FrameKey(pInsertName);
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pText);
}

void CInven::Make_Slot(ITEMID _eItemID)
{
	auto pItem = CAbstractFactory<CItem>::Create();

	switch (_eItemID)
	{
	case ITEM_SHOVEL:
		pItem->Set_FrameKey(L"Item_Slot1");
		break;

	case ITEM_WEAPON:
		pItem->Set_FrameKey(L"Item_Slot2");
		break;

	case ITEM_ARMOR:
		pItem->Set_FrameKey(L"Item_Slot3");
		break;

	case ITEM_CAP:
		pItem->Set_FrameKey(L"Item_Slot4");
		break;

	case ITEM_KEY:
		pItem->Set_FrameKey(L"Item_Slot5");
		break;

	case ITEM_SPELL:
		pItem->Set_FrameKey(L"Spell_Slot");
		break;
	}

	pItem->Set_Scale(SLOTCX, SLOTCY);
	static_cast<CItem*>(pItem)->Set_Slot(true);
	m_mapSlot.insert({_eItemID, pItem});
}

void CInven::Delete_Item(ITEMID _eItemID)
{
	auto pairItem = m_mapItem.find(_eItemID);

	if (pairItem != m_mapItem.end())
	{
		delete pairItem->second;
		pairItem->second = nullptr;

		pairItem = m_mapItem.erase(pairItem);
	}
}

void CInven::Delete_Slot(ITEMID _eItemID)
{
	auto pairSlot = m_mapSlot.find(_eItemID);

	if (pairSlot != m_mapSlot.end())
	{
		delete pairSlot->second;
		pairSlot->second = nullptr;

		pairSlot = m_mapSlot.erase(pairSlot);
	}
}

void CInven::Update_Heart()
{
	int iHp = CObjMgr::Get_Instance()->Get_Player()->Get_Hp();
	int iMaxHp = CObjMgr::Get_Instance()->Get_Player()->Get_MaxHp();

	int iHeart = iHp / 2;
	int iMaxHeart = iMaxHp / 2;

	if (iHp < 0)
		return;

	for (int i = iMaxHeart - 1; i >= 0; --i)
	{
		m_vecHeart[i]->Set_FrameKey(L"Player_Heart");
	}

	if (iMaxHp > iHp)
	{
		for (int i = iMaxHeart - 1; i >= iHeart; --i)
		{
			if (i == iHeart && iHp % 2 == 1)
			{
				m_vecHeart[i]->Set_FrameKey(L"Player_Heart_Half");
			}
			else
			{
				m_vecHeart[i]->Set_FrameKey(L"Player_Heart_Empty");
			}
		}
	}
}

void CInven::Update_Wallet(ITEMID _eItemID)
{
	// TODO : 재화 초기화
}

void CInven::Render_Money(HDC hDC)
{
	TCHAR str[100] = L"";
	HFONT font = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
	                        VARIABLE_PITCH | FF_ROMAN, L"휴먼둥근헤드라인");

	swprintf_s(str, L"%d", m_iCoin);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));

	SelectObject(hDC, font);
	TextOut(hDC, COINPOSX + 30, COINPOSY - 10, str, lstrlen(str));

	swprintf_s(str, L"%d", m_iDiamond);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	TextOut(hDC, COINPOSX + 30, COINPOSY + 40, str, lstrlen(str));
}

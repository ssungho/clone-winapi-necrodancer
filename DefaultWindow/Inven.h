#pragma once
#include "Obj.h"
#include "SceneMgr.h"

class CInven
{
private:
	CInven();
	~CInven();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	static CInven* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CInven;

		return m_pInstance;
	}

	static void Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	bool Can_DestroyWall();
	bool Can_Attack();
	bool Can_Spell();

public:
	void Pickup_Item(CObj* _pItem);
	void Drop_Item(CObj* _pItem, ITEMID _eItemID);
	void Update_Heart();
	void Update_Wallet(ITEMID _eItemID);

public:
	CObj* Get_Item(ITEMID _eItemID)
	{
		return m_mapItem[_eItemID];
	}

	const int Get_Defense()
	{
		int iDefense = 0;
		for (auto& pairItem : m_mapItem)
		{
			auto pKey = pairItem.second->Get_FrameKey();
			if (!lstrcmp(L"Spiked_Leather", pKey))
				iDefense += 1;
			else if (!lstrcmp(L"Chain_Mail", pKey))
				iDefense += 2;
		}

		return iDefense;
	}

	const int Get_Attack()
	{
		int iAttack = 0;
		for (auto& pairItem : m_mapItem)
		{
			auto pKey = pairItem.second->Get_FrameKey();
			if (!lstrcmp(L"Dagger", pKey))
				iAttack = 1;
			else if (!lstrcmp(L"Broadsword", pKey))
				iAttack = 1;
			else if (!lstrcmp(L"Broadsword_T", pKey))
				iAttack = 2;
		}

		return iAttack;
	}

	bool HasKey()
	{
		auto iterKey = m_mapItem.find(ITEM_KEY);

		if (iterKey == m_mapItem.end())
			return false;

		Delete_Item(ITEM_KEY);
		Delete_Slot(ITEM_KEY);

		return true;
	}

	const int Get_Remain_Coin() const { return m_iCoin; }
	const int Get_Remain_Diamond() const { return m_iDiamond; }
	bool Buy(int _iCoin, int _iDiamond);

private:
	void Make_Item(ITEMID _eItemID, CObj* _pItem);
	void Make_Slot(ITEMID _eItemID);
	void Delete_Item(ITEMID _eItemID);
	void Delete_Slot(ITEMID _eItemID);
	void Render_Money(HDC hDC);

private:
	static CInven* m_pInstance;

	map<ITEMID, CObj*> m_mapItem; // æ∆¿Ã≈€ ∏ 
	map<ITEMID, CObj*> m_mapSlot; // ΩΩ∑‘	∏ 

	vector<CObj*> m_vecHeart; // ∏Òº˚
	pair<CObj*, CObj*> m_pairMoneyUI; // ¿Á»≠ UI

	unsigned int m_iSlotCount; // µÓ∑œµ» æ∆¿Ã≈€ ∞≥ºˆ
	int m_iCoin = 100;
	int m_iDiamond = 100;
};

#pragma once
#include "Obj.h"

class CItem : public CObj
{
public:
	CItem();
	~CItem() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	ITEMID Get_ItemID() const { return m_eID; }

	void Set_ItemID(ITEMID _eID) { m_eID = _eID; }
	void Set_Slot(bool _bOn) { m_bOnSLot = _bOn; }
	void Set_Sell() { m_bIsSelling = true; };

protected:
	ITEMID m_eID;
	bool m_bOnSLot;
	bool m_bIsSelling;
};

#pragma once
#include "Obj.h"

class CTile : public CObj
{
public:
	CTile();
	~CTile() override;

public: // Getter & Setter
	int Get_DrawID() const { return m_iDrawID; }
	int Get_Option() const { return m_iOption; }
	CObj* Get_Owner() const { return m_pOwnerObj; }
	bool Is_Stair() const { return m_bIsStair; }

	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void Set_Option(int _iOption) { m_iOption = _iOption; }
	void Set_CoinX(COIN_X _eCoinX) { m_eCurCoinX = _eCoinX; }

	void Set_Owner(CObj* _pObj)
	{
		m_pOwnerObj = _pObj;
		m_bOnHere = true;
	}

	void Leave_Here()
	{
		m_pOwnerObj = nullptr;
		m_bOnHere = false;
	}

	void Set_Flag() { m_bFlag = true; }
	void Set_Stair() { m_bIsStair = true; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void Move_Tile_Frame();

private:
	int m_iDrawID; // 아직 필요 없음.
	int m_iOption; // 옵션에 따라 타일 결정
	int m_iShiftValue; // BMP의 몇 칸씩 이동할지 (애니메이션)
	bool m_bFlag; // 격자로 타일 설정을 위한 플래그

	COIN_X m_ePreCoinX;
	COIN_X m_eCurCoinX;

	bool m_bOnHere; // 뭔가 있는지
	CObj* m_pOwnerObj; // 소유자

	bool m_bIsStair; // 계단인지..
};

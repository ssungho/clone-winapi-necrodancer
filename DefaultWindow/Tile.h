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
	int m_iDrawID; // ���� �ʿ� ����.
	int m_iOption; // �ɼǿ� ���� Ÿ�� ����
	int m_iShiftValue; // BMP�� �� ĭ�� �̵����� (�ִϸ��̼�)
	bool m_bFlag; // ���ڷ� Ÿ�� ������ ���� �÷���

	COIN_X m_ePreCoinX;
	COIN_X m_eCurCoinX;

	bool m_bOnHere; // ���� �ִ���
	CObj* m_pOwnerObj; // ������

	bool m_bIsStair; // �������..
};

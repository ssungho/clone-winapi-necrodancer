#pragma once
#include "Obj.h"
#include "Item.h"

class CPlayer : public CObj
{
public:
	enum STATE { IDLE, MOVE, ATTACK, HIT, DEAD, ST_END };

public:
	CPlayer();
	~CPlayer() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	bool Get_Player_Dead() const { return m_bPlayerDead; }
	int Get_Combo() const { return m_iCombo; }
	bool Get_Move() const { return m_bMove; }

	void Set_Combo(int _iCombo) { m_iCombo = _iCombo; }
	void Clothes_Change();
	void Shake();

private:
	void Key_Input();
	void Offset();
	void Motion_Change();

	//	진행 방향 타일 검사
	bool Check_Tile();

	//	행동들
	void Move();
	void Attack();
	void Spell();

private:
	STATE m_ePreState;
	STATE m_eCurState;

	int m_iArmor;
	int m_iWeapon;
	int m_iCombo;

	const TCHAR* m_pBodyFrameKey = L"";
	const TCHAR* m_pShadowFrameKey = L"";
	const TCHAR* m_pDeathFrameKey = L"";
	VEC2 m_tPos2D;

	DIRECTION m_eNextDir; // 트랩) 점프 뛸 곳
	CObj* m_pPreTile; // 트랩) 점프 뛰기 전 타일

	float m_fShadowPosY;
	bool m_bIsShake;
	DWORD m_dwShakeTime;
	UINT m_iShakeCnt;

	bool m_bPlayerDead;
	UINT m_iAttackCombo;
};

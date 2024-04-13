#pragma once
#include "Monster.h"

class CMinotaur : public CMonster
{
public:
	CMinotaur();
	~CMinotaur() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Do_Act() override;

protected:
	void Motion_Change() override;
	void Move_Frame();

private:
	void Move();
	void Charge();
	void Chase();
	void Stun();
	void Detect();
	void Attack();

private:
	bool m_bCharge;
	bool m_bCollision;

	int m_iFrameStart;
	UINT m_iStunCnt;
};

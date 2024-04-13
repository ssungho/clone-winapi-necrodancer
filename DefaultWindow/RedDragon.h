#pragma once
#include "Monster.h"

class CRedDragon : public CMonster
{
public:
	CRedDragon();
	~CRedDragon() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

protected:
	void Motion_Change() override;
	void Do_Act() override;
	void Move();
	void Breath();

protected:
	void Chase();
	void Detect();

private:
	UINT m_iBreathCnt;
	DIRECTION m_eBreathDir;
};

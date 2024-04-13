#pragma once
#include "Monster.h"

class CBlueSlime : public CMonster
{
public:
	CBlueSlime();
	~CBlueSlime() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Do_Act() override;
	void Move();
	void Patrol();

protected:
	void Motion_Change() override;
};

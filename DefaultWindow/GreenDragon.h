#pragma once
#include "Monster.h"

class CGreenDragon : public CMonster
{
public:
	CGreenDragon();
	~CGreenDragon() override;

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

protected:
	void Chase();
};

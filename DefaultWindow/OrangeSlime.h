#pragma once
#include "Monster.h"

class COrangeSlime : public CMonster
{
public:
	COrangeSlime();
	~COrangeSlime() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Do_Act() override;

	void Move();
	void Patrol();

protected:
	void Motion_Change() override;
};

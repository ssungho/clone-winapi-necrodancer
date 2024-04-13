#pragma once
#include "Monster.h"

class CBat : public CMonster
{
public:
	CBat();
	~CBat() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Do_Act() override;

protected:
	void Motion_Change() override;
	void Patrol();
	void Move();
	bool On_Range();
};

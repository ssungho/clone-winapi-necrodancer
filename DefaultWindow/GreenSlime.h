#pragma once
#include "Monster.h"

class CGreenSlime : public CMonster
{
public:
	CGreenSlime();
	~CGreenSlime() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Do_Act() override;

protected:
	void Motion_Change() override;

private:
	void Move();
};

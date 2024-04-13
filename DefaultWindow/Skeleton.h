#pragma once
#include "Monster.h"

class CSkeleton : public CMonster
{
public:
	CSkeleton();
	~CSkeleton() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Do_Act() override;
	void Move();
	void Chase(bool _bCanAttack);

protected:
	void Motion_Change() override;
};

#pragma once
#include "Skeleton.h"

class CYellowSkeleton : public CSkeleton
{
public:
	CYellowSkeleton();
	~CYellowSkeleton() override;

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

private:
	void Escape();
	void Move_Frame();
	void Bounce();

private:
	int m_iFrameStart;
};

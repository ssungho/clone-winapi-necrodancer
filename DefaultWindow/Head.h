#pragma once
#include "Obj.h"

class CHead : public CObj
{
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void Move();
};

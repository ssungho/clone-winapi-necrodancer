#pragma once
#include "Obj.h"

class CHeart : public CObj
{
public:
	CHeart();
	~CHeart() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

#pragma once
#include "Obj.h"

class CMiss : public CObj
{
public:
	CMiss();
	~CMiss() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	DWORD m_dwTime;
};

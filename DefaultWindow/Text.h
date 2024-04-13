#pragma once
#include "Obj.h"

class CText : public CObj
{
public:
	CText();
	~CText() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	DWORD m_dwTime;

private:
};

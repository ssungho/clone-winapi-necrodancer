#pragma once
#include "Obj.h"

class CBeatHeart : public CObj
{
public:
	CBeatHeart();
	~CBeatHeart() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Heart_Beat();

private:
	UINT m_iBeatCnt;
	bool m_bOnFrame;
	DWORD m_dwBeatTimer;
};

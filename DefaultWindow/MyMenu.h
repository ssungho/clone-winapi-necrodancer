#pragma once

#include "Scene.h"

class CMyMenu : public CScene
{
public:
	CMyMenu();
	~CMyMenu() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	bool m_bInput = false;
};

#pragma once

#include "Scene.h"

class CMyEdit : public CScene
{
public:
	CMyEdit();
	~CMyEdit() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Set_Option(int _iOption) { m_iOption = _iOption; }

private:
	void Key_Input();
	void Change_Option();

private:
	int m_iDrawID;
	int m_iOption;
	int m_iMode;
	WALLID m_eWallID;
};

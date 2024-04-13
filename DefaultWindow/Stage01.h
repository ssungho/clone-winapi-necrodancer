#pragma once
#include "Scene.h"

class CStage01 : public CScene
{
public:
	CStage01();
	~CStage01() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Set_CanInput(bool _bCan) { m_bCanInput = _bCan; }
	bool Get_CanInput() const { return m_bCanInput; }
	void Draw_Dead_Popup(HDC hDC);

public:
	void Load_Stage();
	void Create_Beat(int _iBpm);

private:
	DWORD m_dwTimer;
	bool m_bCanInput;
	int m_iStageBpm;

	bool m_bOnFade;
	UINT m_byteFadeTime;
};

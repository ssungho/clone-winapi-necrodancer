#pragma once
#include "Scene.h"

class CStage02 : public CScene
{
public:
	CStage02();
	~CStage02() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Set_CanInput(bool _bCan) { m_bCanInput = _bCan; }
	bool Get_CanInput() const { return m_bCanInput; }

public:
	void Load_Stage();
	void Create_Beat(int _iBpm);
	void Draw_Dead_Popup(HDC hDC);

private:
	DWORD m_dwTimer;
	bool m_bCanInput;
	int m_iStageBpm;

	bool m_bOnFade;
	UINT m_byteFadeTime;
};

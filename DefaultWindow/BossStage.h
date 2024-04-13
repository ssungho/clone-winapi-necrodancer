#pragma once
#include "Scene.h"

class CBossStage : public CScene
{
public:
	CBossStage();
	~CBossStage() override;

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
	void Play_CutScene(HDC hDC);
	void Draw_Dead_Popup(HDC hDC);

private:
	bool m_bOnCutScene;
	bool m_bStop;
	DWORD m_dwCutSceneTimer;
	float m_fMoveDist;

	DWORD m_dwTimer;
	bool m_bCanInput;
	int m_iStageBpm;
};

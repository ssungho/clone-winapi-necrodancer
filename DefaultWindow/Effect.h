#pragma once
#include "Obj.h"

class CEffect : public CObj
{
public:
	CEffect();
	~CEffect() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	EFFECTID Get_EffectType() const { return m_eEffectType; }
	void Set_EffectType(EFFECTID _eEffectType) { m_eEffectType = _eEffectType; }
	void Set_Life_Time(int _iLifeTime) { m_iLifeTime = _iLifeTime; }
	void Set_Effect(const TCHAR* _pFrameKey, EFFECTID _eEffectType, float _fX, float _fY, int _iLifeTime);

private:
	int Move_Effect_Frame();

private:
	EFFECTID m_eEffectType;
	DWORD m_dwTime;
	int m_iLifeTime;
};

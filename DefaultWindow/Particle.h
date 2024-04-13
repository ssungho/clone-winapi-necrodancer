#pragma once
#include "Obj.h"

class CParticle : public CObj
{
public:
	CParticle();
	~CParticle() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Speed(float _fSpeed) { m_fSpeed = _fSpeed; }

private:
	float m_fAngle;
	float m_fMoveDist;
};

#pragma once
#include "Obj.h"

class CTrap : public CObj
{
public:
	CTrap();
	~CTrap() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Play_Trap_Anim();

public:
	void Set_TrapOn(bool _bOn) { m_bOnTrap = _bOn; }

	TRAPID Get_TrapID() const { return m_eTrapID; }
	bool Is_TrapOn() const { return m_bOnTrap; }

private:
	TRAPID m_eTrapID;
	bool m_bOnTrap;
};

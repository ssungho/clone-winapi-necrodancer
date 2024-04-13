#pragma once
#include "Obj.h"

class CWall : public CObj
{
public:
	CWall();
	~CWall() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Motion_Change();
	void Create_Particle();

	WALLID Get_WallID() const { return m_eWallID; }
	void Set_WallID(WALLID _eWallID) { m_eWallID = _eWallID; }

private:
	WALLID m_eWallID;
};

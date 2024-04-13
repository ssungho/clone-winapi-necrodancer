#pragma once
#include "Obj.h"

class CWallMgr
{
private:
	CWallMgr();
	~CWallMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	void Create_Wall(POINT ptMouse);
	void Create_Wall(POINT ptMouse, WALLID _eWallID);
	void Delete_Wall(POINT ptMouse);

public:
	void Save_Wall();
	void Load_Wall();

	void Set_Boss_Stage_Wall();
	void Set_Stage01_Wall();
	void Set_Stage02_Wall();

	// ∞≥¬º∏Æ
	void Set_Stage1_Wall(); // owner º≥¡§«ÿ¡‹.

public:
	static CWallMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CWallMgr;

		return m_pInstance;
	}

	static void Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CWallMgr* m_pInstance;
	vector<CObj*> m_vecWall;

	// ¿”Ω√..
	vector<int> m_vecIndexes;
};

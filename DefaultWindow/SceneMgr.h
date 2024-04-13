#pragma once

#include "Define.h"
#include "MyEdit.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

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

public:
	void Scene_Change(SCENEID eID);
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	SCENEID Get_Current_SceneID() const { return m_eCurScene; }

private:
	static CSceneMgr* m_pInstance;

	CScene* m_pScene;

	SCENEID m_ePreScene;
	SCENEID m_eCurScene;
};

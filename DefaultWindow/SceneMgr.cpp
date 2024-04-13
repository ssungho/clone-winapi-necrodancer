#include "stdafx.h"
#include "SceneMgr.h"
#include "BossStage.h"
#include "LoadingScene.h"
#include "MyMenu.h"
#include "ObjMgr.h"
#include "Stage01.h"
#include "Stage02.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_ePreScene(SC_END), m_eCurScene(SC_MENU)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_MENU:
			m_pScene = new CMyMenu;
			break;

		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;

		case SC_STAGE_01:
			m_pScene = new CStage01;
			break;

		case SC_STAGE_02:
			m_pScene = new CStage02;
			break;

		case SC_BOSS_STAGE:
			m_pScene = new CBossStage;
			break;

		case SC_LOADING:
			m_pScene = new CLoadingScene;
			break;
		}

		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}

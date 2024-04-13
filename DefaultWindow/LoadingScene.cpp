#include "stdafx.h"
#include "LoadingScene.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"

CLoadingScene::CLoadingScene()
{
}

CLoadingScene::~CLoadingScene()
{
}

void CLoadingScene::Initialize()
{
	CSoundMgr::Get_Instance()->StopAll();
	m_eDeadPoint = CObjMgr::Get_Instance()->Get_Player()->Get_DeadPoint();
	CObjMgr::Get_Instance()->Get_Player()->Initialize();
}

int CLoadingScene::Update()
{
	CSceneMgr::Get_Instance()->Scene_Change(m_eDeadPoint);
	return 0;
}

void CLoadingScene::Late_Update()
{
}

void CLoadingScene::Render(HDC hDC)
{
}

void CLoadingScene::Release()
{
}

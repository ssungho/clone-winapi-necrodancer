#include "stdafx.h"
#include "MainGame.h"

#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "Inven.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "WallMgr.h"

CMainGame::CMainGame() : m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	GET_SINGLE(CKeyMgr)->Initialize();

	CSoundMgr::Get_Instance()->Initialize(); // 사운드 매니저 초기화
	CBmpMgr::Get_Instance()->Load_AllBmp(); // 모든 이미지 로드
	CSceneMgr::Get_Instance()->Scene_Change(SC_MENU);
}

void CMainGame::Update()
{
	GET_SINGLE(CKeyMgr)->Update();

	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CScrollMgr::Get_Instance()->Scroll_Lock();

	CSceneMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render()
{
	// FPS 출력

	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");

	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	CSoundMgr::Destroy_Instance();
	CInven::Destroy();
	CWallMgr::Destroy();
	CTileMgr::Destroy();
	CBmpMgr::Destroy();
	CScrollMgr::Destroy();
	CSceneMgr::Destroy();
	CObjMgr::Destroy();
	ReleaseDC(g_hWnd, m_hDC);
}

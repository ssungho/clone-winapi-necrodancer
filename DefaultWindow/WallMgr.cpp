#include "stdafx.h"
#include "WallMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "Wall.h"

CWallMgr* CWallMgr::m_pInstance = nullptr;

CWallMgr::CWallMgr()
{
	m_vecWall.reserve(TILEX * TILEY);
}

CWallMgr::~CWallMgr()
{
	Release();
}

void CWallMgr::Initialize()
{
	// 일반 이니셜라이즈
	//for (int i = 0; i < TILEY; ++i)
	//{
	//	for (int j = 0; j < TILEX; ++j)
	//	{
	//		float fX = TILECX / 2.f + (TILECX * j) - 2;		// 타일과 사이즈 동기화
	//		float fY = TILECY / 2.f + (TILECY * i) - 22;	// 타일과 사이즈 동기화

	//		CObj* pObj = CAbstractFactory<CWall>::Create(fX, fY);

	//		pObj->Set_FrameKey(L"Dirt_Crypt");

	//		m_vecWall.push_back(pObj);
	//	}
	//}

	// 13 * 11
	// 30 * 20


	//for (int i = 0; i < TILEY; ++i)
	//{
	//	for (int j = 0; j < TILEX; ++j)
	//	{
	//		int yCenter = TILEY / 2;
	//		int xCenter = TILEX / 2;
	//		if (((i == yCenter - 10)) && ((j == xCenter - 6) || (j == xCenter + 6)))
	//		{

	//			float fX = TILECX / 2.f + (TILECX * j)/* - 2*/;		// 타일과 사이즈 동기화
	//			float fY = TILECY / 2.f + (TILECY * i)/* - 22*/;	// 타일과 사이즈 동기화

	//			CObj* pObj = CAbstractFactory<CWall>::Create(fX, fY);

	//			pObj->Set_FrameKey(L"Boss_Wall");
	//			static_cast<CWall*>(pObj)->Set_WallID(BOSS);

	//			m_vecWall.push_back(pObj);
	//		}
	//	}
	//}


	/*for (int i = 4; i < 26; ++i)
	{
		for (int j = 4; j < 26; ++j)
		{
			float fX = TILECX / 2.f + TILECX * j;
			float fY = TILECY / 2.f + TILECY * i;

	

			CObj* pObj = CAbstractFactory<CWall>::Create(fX, fY);

			pObj->Set_FrameKey(L"Boss_Wall");
			static_cast<CWall*>(pObj)->Set_WallID(BOSS);

			m_vecWall.push_back(pObj);
			
		}
	}*/
}

void CWallMgr::Update()
{
	for (auto& pWall : m_vecWall)
	{
		int iResult = pWall->Update();

		if (OBJ_DEAD == iResult)
			Safe_Delete<CObj*>(pWall);
	}
}

void CWallMgr::Late_Update()
{
	for (auto& pWall : m_vecWall)
		pWall->Late_Update();
}

void CWallMgr::Render(HDC hDC)
{
	for (auto& pWall : m_vecWall)
	{
		pWall->Render(hDC);
	}
}

void CWallMgr::Release()
{
	for_each(m_vecWall.begin(), m_vecWall.end(), DeleteObj());
	m_vecWall.clear();
	m_vecWall.shrink_to_fit();
}

void CWallMgr::Create_Wall(POINT ptMouse)
{
	const int x = ptMouse.x / TILECX;
	const int y = ptMouse.y / TILECY;
	const int iIndex = y * TILEX + x;

	const auto tInfo = CTileMgr::Get_Instance()->Get_Tile(iIndex)->Get_Info();

	CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
	m_vecWall.push_back(pObj);
}

void CWallMgr::Create_Wall(POINT ptMouse, WALLID _eWallID)
{
	const int x = ptMouse.x / TILECX;
	const int y = ptMouse.y / TILECY;
	const int iIndex = y * TILEX + x;

	if (find(m_vecIndexes.begin(), m_vecIndexes.end(), iIndex)
		!= m_vecIndexes.end())
		return;

	const auto tInfo = CTileMgr::Get_Instance()->Get_Tile(iIndex)->Get_Info();

	CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
	static_cast<CWall*>(pObj)->Set_WallID(_eWallID); // 0328 추가

	if (_eWallID == NORMAL)
		pObj->Set_FrameKey(L"Dirt_Crypt");

	if (_eWallID == STONE)
		pObj->Set_FrameKey(L"Stone_Wall");

	if (_eWallID == BOSS)
		pObj->Set_FrameKey(L"Boss_Wall");

	if (_eWallID == DOOR_V)
		pObj->Set_FrameKey(L"Door_Side");

	if (_eWallID == DOOR_H)
		pObj->Set_FrameKey(L"Door_Front");

	if (_eWallID == SHOP)
		pObj->Set_FrameKey(L"Shop_Wall");

	if (_eWallID == LEVEL)
		pObj->Set_FrameKey(L"Level_Wall");

	m_vecWall.push_back(pObj);

	m_vecIndexes.push_back(iIndex);

	sort(m_vecWall.begin(), m_vecWall.end(), [&](auto& a, auto& b) -> bool
	{
		if (a->Get_Info().fY < b->Get_Info().fY)
			return true;
		return false;
	});
}

void CWallMgr::Delete_Wall(POINT ptMouse)
{
	const int x = ptMouse.x / TILECX;
	const int y = ptMouse.y / TILECY;
	const int iIndex = y * TILEX + x;

	const auto tInfo = CTileMgr::Get_Instance()->Get_Tile(iIndex)->Get_Info();

	auto iter = find_if(m_vecWall.begin(), m_vecWall.end(), [&](auto a)-> bool
	{
		if (abs(a->Get_Info().fX - tInfo.fX) <= 0.001f && abs(a->Get_Info().fY - tInfo.fY) <= 0.001f)
			return true;
		return false;
	});

	if (iter != m_vecWall.end())
		iter = m_vecWall.erase(iter);

	auto newIter = find(m_vecIndexes.begin(), m_vecIndexes.end(), iIndex);
	if (newIter != m_vecIndexes.end())
		newIter = m_vecIndexes.erase(newIter);
}

void CWallMgr::Save_Wall()
{
	HANDLE hFile = CreateFile(
		L"../Data/Stage02_Wall.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수

	// 저장할 것들
	WALLID eWallID;

	for (auto& pTile : m_vecWall)
	{
		WriteFile(hFile, &(pTile->Get_Info()), sizeof(INFO), &dwByte, nullptr);


		eWallID = static_cast<CWall*>(pTile)->Get_WallID();
		WriteFile(hFile, &eWallID, sizeof(WALLID), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CWallMgr::Load_Wall()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Wall.dat", // 파일 경로(이름을 포함)
	                          GENERIC_READ, // 파일 접근 모드(GENERIC_READ : 읽기 전용)
	                          NULL, // 공유 방식, 파일이 열려 있는 상태에서 다른 프로세서에서 오픈하고자 할 때 허용할 지에 대한 여부(NULL인 경우 공유하지 않음)
	                          nullptr, // 보안 모드(NULL인 경우 기본 보안 상태)
	                          OPEN_EXISTING,
	                          // 생성 방식(CREATE_ALWAYS : 파일이 없으면 생성, 있으면 덮어쓰기), (OPEN_EXISTING : 파일이 있을 때만 열기)
	                          FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 특수 속성이 없는 파일 생성)
	                          nullptr); // 생성될 파일의 속성을 제공할 템플릿 파일(우리는 사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0);
	INFO tInfo{};
	WALLID eWallID;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &eWallID, sizeof(WALLID), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
		static_cast<CWall*>(pObj)->Set_WallID(eWallID);

		if (eWallID == NORMAL)
			pObj->Set_FrameKey(L"Dirt_Crypt");

		if (eWallID == STONE)
			pObj->Set_FrameKey(L"Stone_Wall");

		if (eWallID == BOSS)
			pObj->Set_FrameKey(L"Boss_Wall");

		if (eWallID == DOOR_V)
			pObj->Set_FrameKey(L"Door_Side");

		if (eWallID == DOOR_H)
			pObj->Set_FrameKey(L"Door_Front");

		if (eWallID == SHOP)
			pObj->Set_FrameKey(L"Shop_Wall");

		if (eWallID == LEVEL)
			pObj->Set_FrameKey(L"Level_Wall");

		m_vecWall.push_back(pObj);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CWallMgr::Set_Stage1_Wall()
{
	HANDLE hFile = CreateFile(L"../Data/Stage1_Wall.dat",
	                          GENERIC_READ,
	                          NULL,
	                          nullptr,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0);
	INFO tInfo{};

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(tInfo.fX, tInfo.fY))->Set_Owner(pObj);
		CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj); // 0323 수정 여기로 넣음
		// 원래는 벡터에 넣었었음
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

// 이게 진짜 스테이지 01
void CWallMgr::Set_Stage01_Wall()
{
	HANDLE hFile = CreateFile(L"../Data/Stage01_Wall.dat",
	                          GENERIC_READ,
	                          NULL,
	                          nullptr,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0);
	INFO tInfo{};
	WALLID eWallID;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &eWallID, sizeof(WALLID), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(tInfo.fX, tInfo.fY))->Set_Owner(pObj);
		static_cast<CWall*>(pObj)->Set_WallID(eWallID);

		if (eWallID == NORMAL)
		{
			pObj->Set_FrameKey(L"Dirt_Crypt");

			int iRand = rand() % 15;
			pObj->Set_Frame({iRand, iRand, 0, 0, 0});

			pObj->Set_MaxHp(1);
			pObj->Set_Hp(1);
		}

		if (eWallID == STONE)
		{
			pObj->Set_FrameKey(L"Stone_Wall");
			pObj->Set_MaxHp(2);
			pObj->Set_Hp(2);
		}

		if (eWallID == BOSS)
			pObj->Set_FrameKey(L"Boss_Wall");

		if (eWallID == DOOR_V)
			pObj->Set_FrameKey(L"Door_Side");

		if (eWallID == DOOR_H)
			pObj->Set_FrameKey(L"Door_Front");

		if (eWallID == SHOP)
		{
			pObj->Set_FrameKey(L"Shop_Wall");
			pObj->Set_MaxHp(4);
			pObj->Set_Hp(4);
		}

		if (eWallID == LEVEL)
			pObj->Set_FrameKey(L"Level_Wall");

		CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
	}

	CloseHandle(hFile);

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}


void CWallMgr::Set_Stage02_Wall()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Wall.dat",
	                          GENERIC_READ,
	                          NULL,
	                          nullptr,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0);
	INFO tInfo{};
	WALLID eWallID;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &eWallID, sizeof(WALLID), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(tInfo.fX, tInfo.fY))->Set_Owner(pObj);
		static_cast<CWall*>(pObj)->Set_WallID(eWallID);

		if (eWallID == NORMAL)
		{
			pObj->Set_FrameKey(L"Dirt_Crypt");

			int iRand = rand() % 15;
			pObj->Set_Frame({iRand, iRand, 0, 0, 0});

			pObj->Set_MaxHp(1);
			pObj->Set_Hp(1);
		}

		if (eWallID == STONE)
		{
			pObj->Set_FrameKey(L"Stone_Wall");
			pObj->Set_MaxHp(2);
			pObj->Set_Hp(2);
		}

		if (eWallID == BOSS)
			pObj->Set_FrameKey(L"Boss_Wall");

		if (eWallID == DOOR_V)
			pObj->Set_FrameKey(L"Door_Side");

		if (eWallID == DOOR_H)
			pObj->Set_FrameKey(L"Door_Front");

		if (eWallID == SHOP)
		{
			pObj->Set_FrameKey(L"Shop_Wall");
			pObj->Set_MaxHp(4);
			pObj->Set_Hp(4);
		}

		if (eWallID == LEVEL)
			pObj->Set_FrameKey(L"Level_Wall");

		CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
	}

	CloseHandle(hFile);

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CWallMgr::Set_Boss_Stage_Wall()
{
	HANDLE hFile = CreateFile(L"../Data/Boss_Stage_Wall.dat",
	                          GENERIC_READ,
	                          NULL,
	                          nullptr,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0);
	INFO tInfo{};
	WALLID eWallID;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &eWallID, sizeof(WALLID), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CWall>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(tInfo.fX, tInfo.fY))->Set_Owner(pObj);
		static_cast<CWall*>(pObj)->Set_WallID(eWallID);

		if (eWallID == NORMAL)
			pObj->Set_FrameKey(L"Dirt_Crypt");

		if (eWallID == BOSS || eWallID == STONE)
		{
			pObj->Set_FrameKey(L"Boss_Wall");
			pObj->Set_MaxHp(100);
			pObj->Set_Hp(100);
		}

		if (eWallID == DOOR_V)
			pObj->Set_FrameKey(L"Door_Side");

		if (eWallID == DOOR_H)
			pObj->Set_FrameKey(L"Door_Front");

		if (eWallID == LEVEL)
			pObj->Set_FrameKey(L"Level_Wall");

		CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj); // 0323 수정 여기로 넣음
		// 원래는 벡터에 넣었었음
	}

	CloseHandle(hFile);

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

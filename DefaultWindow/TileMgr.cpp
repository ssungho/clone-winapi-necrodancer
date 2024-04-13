#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "Tile.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr() : m_iDrawID(0), m_iOption(0)
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	/*for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = TILECX / 2.f + (TILECX * j);
			float fY = TILECY / 2.f + (TILECY * i);

			CObj* pObj = CAbstractFactory<CTile>::Create(fX, fY);
			pObj->Set_FrameKey(L"Dirt1_Floor");
			

			m_vecTile.push_back(pObj);
		}
	}*/

	// 보스방

	//// 스테이지2
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = TILECX / 2.f + (TILECX * j);
			float fY = TILECY / 2.f + (TILECY * i);

			CObj* pObj = CAbstractFactory<CTile>::Create(fX, fY);

			if (i <= 3 || i >= 27 || j <= 3 || j >= 27)
			{
				pObj->Set_FrameKey(L"Black_Tile");
				static_cast<CTile*>(pObj)->Set_DrawID(2);
			}
			else
			{
				pObj->Set_FrameKey(L"Dirt2_Floor");
			}

			m_vecTile.push_back(pObj);
		}
	}
}

void CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	int iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX / TILECX + 2;
	int iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			static_cast<CTile*>(m_vecTile[iIndex])->Render(hDC);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
	m_vecTile.reserve(TILEX * TILEY);
}

void CTileMgr::Reset_RestoreTile()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
	m_vecTile.reserve(TILEX * TILEY);
}

CObj* CTileMgr::Get_Tile(CObj* _pObj) const
{
	const auto tInfo = _pObj->Get_Info();
	return Get_Tile(tInfo.fX, tInfo.fY);
}

CObj* CTileMgr::Get_Tile_On_Player() const
{
	auto tInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	return Get_Tile(tInfo.fX, tInfo.fY);
}

#pragma region 스테이지 1 관련

void CTileMgr::Load_Stage01_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage01_Tile.dat",
	                          GENERIC_READ,
	                          NULL,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수
	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		if (iDrawID == 0)
		{
			pObj->Set_FrameKey(L"Dirt1_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	static_cast<CTile*>(Get_Tile(1176, 312))->Set_Stair();
	Get_Tile(1176, 312)->Set_FrameKey(L"Stair_Locked_Tile");

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CTileMgr::Load_Stage01_Tile(int _iBpm)
{
	HANDLE hFile = CreateFile(L"../Data/Stage01_Tile.dat",
	                          GENERIC_READ,
	                          NULL,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수
	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		if (iDrawID == 0)
		{
			pObj->Set_FrameKey(L"Dirt1_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		pObj->Set_Bpm(_iBpm);
		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	static_cast<CTile*>(Get_Tile(1176, 312))->Set_Stair();
	Get_Tile(1176, 312)->Set_FrameKey(L"Stair_Locked_Tile");

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CTileMgr::Late_Update_Stage01_Tile()
{
	const int iCombo = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Combo();

	if (iCombo < 5)
	{
		Set_Coin_Mode(COIN_X_1);
	}
	else if (iCombo < 10)
	{
		Set_Coin_Mode(COIN_X_2);
	}

	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

#pragma endregion

#pragma region 스테이지 2 관련

void CTileMgr::Load_Stage02_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Tile.dat",
	                          GENERIC_READ,
	                          NULL,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수
	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		if (iDrawID == 0)
		{
			pObj->Set_FrameKey(L"Dirt1_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	static_cast<CTile*>(Get_Tile(1176, 312))->Set_Stair();
	Get_Tile(1176, 312)->Set_FrameKey(L"Stair_Locked_Tile");

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CTileMgr::Load_Stage02_Tile(int _iBpm)
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Tile.dat",
	                          GENERIC_READ,
	                          NULL,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수
	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		if (iDrawID == 0)
		{
			pObj->Set_FrameKey(L"Dirt1_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		pObj->Set_Bpm(_iBpm);
		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	static_cast<CTile*>(Get_Tile(1176, 312))->Set_Stair();
	Get_Tile(1176, 312)->Set_FrameKey(L"Stair_Locked_Tile");
}

void CTileMgr::Late_Update_Stage02_Tile()
{
	const int iCombo = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Combo();

	if (iCombo < 5)
	{
		Set_Coin_Mode(COIN_X_1);
	}
	else if (iCombo < 10)
	{
		Set_Coin_Mode(COIN_X_2);
	}

	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

#pragma endregion

#pragma region 보스 스테이지 관련

void CTileMgr::Load_Boss_Stage_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Boss_Stage_Tile.dat", // 파일 경로(이름을 포함)
	                          GENERIC_READ, // 파일 접근 모드(GENERIC_READ : 읽기 전용)
	                          NULL, // 공유 방식, 파일이 열려 있는 상태에서 다른 프로세서에서 오픈하고자 할 때 허용할 지에 대한 여부(NULL인 경우 공유하지 않음)
	                          NULL, // 보안 모드(NULL인 경우 기본 보안 상태)
	                          OPEN_EXISTING,
	                          // 생성 방식(CREATE_ALWAYS : 파일이 없으면 생성, 있으면 덮어쓰기), (OPEN_EXISTING : 파일이 있을 때만 열기)
	                          FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 특수 속성이 없는 파일 생성)
	                          NULL); // 생성될 파일의 속성을 제공할 템플릿 파일(우리는 사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수

	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		if (iDrawID == 0)
		{
			pObj->Set_FrameKey(L"Boss_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CTileMgr::Load_Boss_Stage_Tile(int _iBpm)
{
	HANDLE hFile = CreateFile(L"../Data/Boss_Stage_Tile.dat", // 파일 경로(이름을 포함)
	                          GENERIC_READ, // 파일 접근 모드(GENERIC_READ : 읽기 전용)
	                          NULL, // 공유 방식, 파일이 열려 있는 상태에서 다른 프로세서에서 오픈하고자 할 때 허용할 지에 대한 여부(NULL인 경우 공유하지 않음)
	                          NULL, // 보안 모드(NULL인 경우 기본 보안 상태)
	                          OPEN_EXISTING,
	                          // 생성 방식(CREATE_ALWAYS : 파일이 없으면 생성, 있으면 덮어쓰기), (OPEN_EXISTING : 파일이 있을 때만 열기)
	                          FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 특수 속성이 없는 파일 생성)
	                          NULL); // 생성될 파일의 속성을 제공할 템플릿 파일(우리는 사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수

	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		if (iDrawID == 0)
		{
			pObj->Set_FrameKey(L"Boss_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		pObj->Set_Bpm(_iBpm);
		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);
}

void CTileMgr::Late_Update_Boss_Stage()
{
	const int iCombo = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Combo();

	if (iCombo < 5)
	{
		Set_Coin_Mode(COIN_X_1);
	}
	else if (iCombo < 10)
	{
		Set_Coin_Mode(COIN_X_2);
	}

	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

#pragma endregion

#pragma region 안쓰는거 (혹시몰라서 일단 냅두자.)

void CTileMgr::Load_Stage1_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage1_Tile.dat",
	                          GENERIC_READ,
	                          NULL,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수

	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	int iRow = 0;
	int iCol = 0;

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);

		// 03/22 수정
		switch (iOption)
		{
		case 0:
			pObj->Set_FrameKey(L"Dirt1_Floor");
			pObj->Set_Frame(FRAME{0, 1, 0, 500, GetTickCount()});
			break;

		case 1:
			pObj->Set_FrameKey(L"Dirt2_Floor");
			pObj->Set_Frame(FRAME{0, 1, 0, 500, GetTickCount()});
			break;

		case 2:
			pObj->Set_FrameKey(L"Boss_Floor");
			break;

		default:
			break;;
		}

		// 격자 생성기
		if ((iRow + iCol) % 2 == 1)
		{
			static_cast<CTile*>(pObj)->Set_Flag();
		}

		++iRow;

		if (iRow >= TILEX)
		{
			iRow %= TILEX;
			++iCol;
		}

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CTileMgr::Late_Update_Stage1()
{
	const int iCombo = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Combo();

	if (iCombo < 5)
	{
		Set_Coin_Mode(COIN_X_1);
	}
	else if (iCombo < 10)
	{
		Set_Coin_Mode(COIN_X_2);
	}

	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

#pragma endregion

#pragma region 에디터 관련된 기능

void CTileMgr::Picking_Tile(POINT ptMouse, int iDrawID, int iOption)
{
	int x = ptMouse.x / TILECX;
	int y = ptMouse.y / TILECY;

	int iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	static_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(iDrawID);
	static_cast<CTile*>(m_vecTile[iIndex])->Set_Option(iOption);
	// m_vecTile[iIndex]->Set_Dead();
}

// TODO : 스테이지2로 바꿔둠
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Tile.dat", // 파일 경로(이름을 포함)
	                          GENERIC_WRITE, // 파일 접근 모드(GENERIC_READ : 읽기 전용)
	                          NULL, // 공유 방식, 파일이 열려 있는 상태에서 다른 프로세서에서 오픈하고자 할 때 허용할 지에 대한 여부(NULL인 경우 공유하지 않음)
	                          NULL, // 보안 모드(NULL인 경우 기본 보안 상태)
	                          CREATE_ALWAYS,
	                          // 생성 방식(CREATE_ALWAYS : 파일이 없으면 생성, 있으면 덮어쓰기), (OPEN_EXISTING : 파일이 있을 때만 열기)
	                          FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 특수 속성이 없는 파일 생성)
	                          NULL); // 생성될 파일의 속성을 제공할 템플릿 파일(우리는 사용 안하기 때문에 NULL)


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수

	// 저장할 것들
	int iDrawID(0);
	int iOption(0);

	for (auto& pTile : m_vecTile)
	{
		iDrawID = static_cast<CTile*>(pTile)->Get_DrawID();
		iOption = static_cast<CTile*>(pTile)->Get_Option();

		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(pTile->Get_Info()), sizeof(INFO), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

// TODO : 스테이지2로 바꿔둠
void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Tile.dat", // 파일 경로(이름을 포함)
	                          GENERIC_READ, // 파일 접근 모드(GENERIC_READ : 읽기 전용)
	                          NULL, // 공유 방식, 파일이 열려 있는 상태에서 다른 프로세서에서 오픈하고자 할 때 허용할 지에 대한 여부(NULL인 경우 공유하지 않음)
	                          NULL, // 보안 모드(NULL인 경우 기본 보안 상태)
	                          OPEN_EXISTING,
	                          // 생성 방식(CREATE_ALWAYS : 파일이 없으면 생성, 있으면 덮어쓰기), (OPEN_EXISTING : 파일이 있을 때만 열기)
	                          FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 특수 속성이 없는 파일 생성)
	                          NULL); // 생성될 파일의 속성을 제공할 템플릿 파일(우리는 사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // 예외 처리 변수

	int iDrawID(0);
	int iOption(0);
	INFO tInfo{};

	Release();

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pObj)->Set_Option(iOption);


		if (iDrawID == 0 || iDrawID == 1)
		{
			pObj->Set_FrameKey(L"Dirt2_Floor");
		}
		else if (iDrawID == 2)
		{
			pObj->Set_FrameKey(L"Black_Tile");
		}

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	// MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

#pragma endregion

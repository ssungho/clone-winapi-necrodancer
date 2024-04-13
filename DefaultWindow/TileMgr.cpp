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

	// ������

	//// ��������2
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

#pragma region �������� 1 ����

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

	DWORD dwByte(0); // ���� ó�� ����
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

	// MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
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

	DWORD dwByte(0); // ���� ó�� ����
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

	// MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
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

#pragma region �������� 2 ����

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

	DWORD dwByte(0); // ���� ó�� ����
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

	// MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
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

	DWORD dwByte(0); // ���� ó�� ����
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

#pragma region ���� �������� ����

void CTileMgr::Load_Boss_Stage_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Boss_Stage_Tile.dat", // ���� ���(�̸��� ����)
	                          GENERIC_READ, // ���� ���� ���(GENERIC_READ : �б� ����)
	                          NULL, // ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ������� �����ϰ��� �� �� ����� ���� ���� ����(NULL�� ��� �������� ����)
	                          NULL, // ���� ���(NULL�� ��� �⺻ ���� ����)
	                          OPEN_EXISTING,
	                          // ���� ���(CREATE_ALWAYS : ������ ������ ����, ������ �����), (OPEN_EXISTING : ������ ���� ���� ����)
	                          FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�ƹ��� Ư�� �Ӽ��� ���� ���� ����)
	                          NULL); // ������ ������ �Ӽ��� ������ ���ø� ����(�츮�� ��� ���ϱ� ������ NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // ���� ó�� ����

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

	// MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

void CTileMgr::Load_Boss_Stage_Tile(int _iBpm)
{
	HANDLE hFile = CreateFile(L"../Data/Boss_Stage_Tile.dat", // ���� ���(�̸��� ����)
	                          GENERIC_READ, // ���� ���� ���(GENERIC_READ : �б� ����)
	                          NULL, // ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ������� �����ϰ��� �� �� ����� ���� ���� ����(NULL�� ��� �������� ����)
	                          NULL, // ���� ���(NULL�� ��� �⺻ ���� ����)
	                          OPEN_EXISTING,
	                          // ���� ���(CREATE_ALWAYS : ������ ������ ����, ������ �����), (OPEN_EXISTING : ������ ���� ���� ����)
	                          FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�ƹ��� Ư�� �Ӽ��� ���� ���� ����)
	                          NULL); // ������ ������ �Ӽ��� ������ ���ø� ����(�츮�� ��� ���ϱ� ������ NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // ���� ó�� ����

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

#pragma region �Ⱦ��°� (Ȥ�ø��� �ϴ� ������.)

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

	DWORD dwByte(0); // ���� ó�� ����

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

		// 03/22 ����
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

		// ���� ������
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

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
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

#pragma region ������ ���õ� ���

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

// TODO : ��������2�� �ٲ��
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Tile.dat", // ���� ���(�̸��� ����)
	                          GENERIC_WRITE, // ���� ���� ���(GENERIC_READ : �б� ����)
	                          NULL, // ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ������� �����ϰ��� �� �� ����� ���� ���� ����(NULL�� ��� �������� ����)
	                          NULL, // ���� ���(NULL�� ��� �⺻ ���� ����)
	                          CREATE_ALWAYS,
	                          // ���� ���(CREATE_ALWAYS : ������ ������ ����, ������ �����), (OPEN_EXISTING : ������ ���� ���� ����)
	                          FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�ƹ��� Ư�� �Ӽ��� ���� ���� ����)
	                          NULL); // ������ ������ �Ӽ��� ������ ���ø� ����(�츮�� ��� ���ϱ� ������ NULL)


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // ���� ó�� ����

	// ������ �͵�
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

	MessageBox(g_hWnd, _T("Save �Ϸ�"), L"����", MB_OK);
}

// TODO : ��������2�� �ٲ��
void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Stage02_Tile.dat", // ���� ���(�̸��� ����)
	                          GENERIC_READ, // ���� ���� ���(GENERIC_READ : �б� ����)
	                          NULL, // ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ������� �����ϰ��� �� �� ����� ���� ���� ����(NULL�� ��� �������� ����)
	                          NULL, // ���� ���(NULL�� ��� �⺻ ���� ����)
	                          OPEN_EXISTING,
	                          // ���� ���(CREATE_ALWAYS : ������ ������ ����, ������ �����), (OPEN_EXISTING : ������ ���� ���� ����)
	                          FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�ƹ��� Ư�� �Ӽ��� ���� ���� ����)
	                          NULL); // ������ ������ �Ӽ��� ������ ���ø� ����(�츮�� ��� ���ϱ� ������ NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwByte(0); // ���� ó�� ����

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

	// MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

#pragma endregion

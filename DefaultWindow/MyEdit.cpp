#include "stdafx.h"
#include "MyEdit.h"

#include "AbstractFactory.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "WallMgr.h"
#include "MyUI.h"
#include "SceneMgr.h"

CMyEdit::CMyEdit()
	: m_iDrawID(0), m_iOption(0), m_iMode(0), m_eWallID(NORMAL)
{
	// �ʱⰪ 0, 0, ��
	// ���̵�� ���� �ǹ� ����
	// �ɼ��� �߿���
	// DRAWID 0�� �Ϲ� ��
	// DRAWID 2�� �����ٴ�..
}

CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	// CTileMgr::Get_Instance()->Initialize();
	// CTileMgr::Get_Instance()->Load_Tile();
	// CWallMgr::Get_Instance()->Set_Boss_Stage_Wall();
	// CWallMgr::Get_Instance()->Initialize();
}

int CMyEdit::Update()
{
	Key_Input();

	CTileMgr::Get_Instance()->Update();
	CWallMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();

	return 0;
}

void CMyEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CWallMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyEdit::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CWallMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
}

void CMyEdit::Release()
{
}

void CMyEdit::Key_Input()
{
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_0))
	{
		if (m_iMode == 0)
			m_iMode = 1;
		else
			m_iMode = 0;
	}

	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);

	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	// ���ɱ�
	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::LEFT_MOUSE))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= static_cast<long>(CScrollMgr::Get_Instance()->Get_ScrollX());
		pt.y -= static_cast<long>(CScrollMgr::Get_Instance()->Get_ScrollY());

		if (m_iMode == 0)
			CTileMgr::Get_Instance()->Picking_Tile(pt, 2, m_iOption); // ��������
		else
			CWallMgr::Get_Instance()->Create_Wall(pt, m_eWallID);
	}

	// �� �����
	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::RIGHT_MOUSE))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= static_cast<long>(CScrollMgr::Get_Instance()->Get_ScrollX());
		pt.y -= static_cast<long>(CScrollMgr::Get_Instance()->Get_ScrollY());

		CWallMgr::Get_Instance()->Delete_Wall(pt);
	}

	// ����
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::F1))
	{
		if (m_iMode == 0)
		{
			CTileMgr::Get_Instance()->Save_Tile();
		}
		else
		{
			CWallMgr::Get_Instance()->Save_Wall();
		}
	}

	// �ҷ�����
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::F2))
	{
		if (m_iMode == 0)
		{
			CTileMgr::Get_Instance()->Load_Tile();
		}
		else
		{
			CWallMgr::Get_Instance()->Load_Wall();
		}
	}

	// �ɼ� �ٲٱ�
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_1))
		Change_Option();

	// �� �ɼ� �ٲٱ� �⺻ 0��, 0���� �Ϲ� ��
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_2))
	{
		m_eWallID = NORMAL;
	}

	// �� �ɼ� �ٲٱ� 1�� �� ��
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_3))
	{
		m_eWallID = STONE;
	}

	// �� �ɼ� �ٲٱ� 2�� ���� ��
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_4))
	{
		m_eWallID = DOOR_V;
	}

	// �� �ɼ� �ٲٱ� 3�� ���� ��
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_5))
	{
		m_eWallID = DOOR_H;
	}

	// ���� ��
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_6))
	{
		m_eWallID = SHOP;
	}

	// ���� ���
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_7))
	{
		m_eWallID = LEVEL;
	}


	// ������
	if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::KEY_9))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_MENU);
	}
}

void CMyEdit::Change_Option()
{
	++m_iOption %= 3;
}

#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "MyButton.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CMyMenu::CMyMenu()
{
}

CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"main_menu.ogg", 0.5f);
	CObj* pObj = CAbstractFactory<CMyButton>::Create(400.f, 400.f);
	pObj->Set_FrameKey(L"Edit");

	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
}

int CMyMenu::Update()
{
	CObjMgr::Get_Instance()->Update();

	return 0;
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

	if (m_bInput)
	{
		if (GET_SINGLE(CKeyMgr)->Get_Button_Up(KeyType::ENTER))
			CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE_01);
	}

	if (GET_SINGLE(CKeyMgr)->Get_Button(KeyType::SPACE)) // 스페이스 누르면 계속하기
	{
		m_bInput = true;
	}
}

void CMyMenu::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	// TODO 잠깐
	if (m_bInput)
	{
		HDC tempDC = CBmpMgr::Get_Instance()->Find_Image(L"Continue");
		GdiTransparentBlt(hDC,
		                  WINCX / 2 - 220, WINCY / 2 + 190,
		                  440, 36,
		                  tempDC,
		                  0,
		                  0,
		                  440, 36,
		                  MAGENTA);
	}

	CObjMgr::Get_Instance()->Render(hDC);
}

void CMyMenu::Release()
{
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_UI);
}

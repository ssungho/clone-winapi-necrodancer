#include "stdafx.h"
#include "Monster.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "Head.h"
#include "Heart.h"
#include "ObjMgr.h"
#include "Particle.h"
#include "ScrollMgr.h"
#include "TileMgr.h"

CMonster::CMonster()
	: m_ePreState(ST_DEAD), m_eCurState(IDLE), m_iAlphaPosY(0), m_iNeedToDoAct(0)
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_eObjID = OBJ_MONSTER;
	m_eRenderID = RENDER_GAME_OBJECT;
}

int CMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
	Motion_Change();
	__super::Move_Frame();
}

void CMonster::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * (int)m_tInfo.fCX,
	                  m_tFrame.iMotion * (int)m_tInfo.fCY,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CMonster::Release()
{
	for_each(m_vecHeart.begin(), m_vecHeart.end(), DeleteObj());
	m_vecHeart.clear();
}

void CMonster::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			break;

		case MOVE:
			break;

		case ATTACK:
			break;

		case ST_DEAD:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CMonster::Drop_Item()
{
	int iCoin = rand() % 30;
	auto pTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(m_tInfo.fX, m_tInfo.fY));
	auto pItem = CAbstractFactory<CItem>::Create_Item(ITEM_COIN, L"Item_Coin3", pTile);
	pTile->Set_Owner(pItem);
	pItem->Set_Coin(iCoin);
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pItem);
}

void CMonster::Leave()
{
	auto pTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(this));
	if (pTile->Get_Owner() == this)
		pTile->Leave_Here();
}

void CMonster::Do_Act()
{
}

void CMonster::Render_Heart(HDC hDC)
{
	if (m_iHp >= m_iMaxHp)
	{
		return;
	}

	float iCenterX = m_tInfo.fX;
	if (m_iMaxHp % 2 == 0) // Â¦¼ö
	{
		iCenterX -= float(12 * (m_iMaxHp - 1));
	}
	else
	{
		iCenterX -= float(12 * m_iMaxHp);
	}

	for (int i = 0; i < m_iMaxHp - m_iHp; ++i)
	{
		if (m_iMaxHp - m_iHp >= m_vecHeart.size())
			break;
		m_vecHeart[i]->Set_FrameKey(L"UI_Empty_Small_Heart");
	}
	for (int i = 0; i < m_iMaxHp; ++i)
	{
		m_vecHeart[i]->Set_Pos(iCenterX + (float)(i * 24), m_tRect.top + m_iAlphaPosY);
		m_vecHeart[i]->Render(hDC);
	}
}

void CMonster::Render_Heart_Boss(HDC hDC, int _iAlphaX, int _iAlphaY)
{
	float fCenterX = m_tInfo.fX;

	if (m_iMaxHp % 2 == 0) // Â¦¼ö
	{
		fCenterX -= float(12 * (m_iMaxHp - 1));
	}
	else
	{
		fCenterX -= float(12 * m_iMaxHp);
	}

	for (int i = 0; i < m_iMaxHp - m_iHp; ++i)
	{
		if (m_iMaxHp - m_iHp >= m_vecHeart.size())
			break;
		m_vecHeart[i]->Set_FrameKey(L"UI_Empty_Small_Heart");
	}

	for (int i = 0; i < m_iMaxHp; ++i)
	{
		m_vecHeart[i]->Set_Pos(
			fCenterX + (float)(i * 24) + (float)_iAlphaX,
			(float)m_tRect.top + (float)m_iAlphaPosY + (float)_iAlphaY);
		m_vecHeart[i]->Render(hDC);
	}
}

void CMonster::Update_Heart()
{
	for (int i = 0; i < m_iMaxHp; ++i)
	{
		m_vecHeart[i]->Update();
	}
}

void CMonster::Set_Heart(int _iHeart)
{
	m_vecHeart.reserve(_iHeart);

	float iCenterX = m_tInfo.fX;
	if (_iHeart % 2 == 0) // Â¦¼ö
	{
		iCenterX -= 12;
		iCenterX -= (24 * (_iHeart / 2));
	}
	else
	{
		iCenterX -= 24 * (_iHeart / 2);
	}

	for (int i = 0; i < _iHeart; ++i)
	{
		auto pHeart = CAbstractFactory<CHeart>::Create();
		pHeart->Set_Pos(iCenterX, 12 + m_tRect.top);
		m_vecHeart.push_back(pHeart);
	}
}

void CMonster::Throw(TCHAR* _pKey, int _iCX, int _iCY)
{
	auto pObj = CAbstractFactory<CHead>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_FrameKey(_pKey);
	pObj->Set_Pos(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Scale(_iCX, _iCY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pObj);
}

#include "stdafx.h"
#include "Item.h"

#include "BmpMgr.h"
#include "ScrollMgr.h"

CItem::CItem()
	: m_eID(ITEM_MAX), m_bOnSLot(false), m_bIsSelling(false)
{
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_tInfo.fX = -300;
	m_tInfo.fY = -300;
	m_eObjID = OBJ_ITEM;
	m_eRenderID = RENDER_GAME_OBJECT;
}

int CItem::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CItem::Late_Update()
{
	// ππ æ¯¿Ω.
}

void CItem::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bOnSLot)
	{
		iScrollX = 0;
		iScrollY = 0;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  (int)(m_tInfo.fCX), (int)(m_tInfo.fCY),
	                  hMemDC,
	                  0, 0,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);

	if (m_bIsSelling)
	{
		HFONT font = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		                        VARIABLE_PITCH | FF_ROMAN, L"»ﬁ∏’µ’±Ÿ«ÏµÂ∂Û¿Œ");
		SelectObject(hDC, font);
		SetBkMode(hDC, TRANSPARENT);

		TCHAR str[128];
		wsprintf(str, TEXT("%d"), m_iCoin);
		SetTextColor(hDC, RGB(255, 255, 255));
		TextOut(hDC, (int)m_tInfo.fX + iScrollX - 5, (int)m_tInfo.fY + iScrollY + 20, str, lstrlen(str));
	}
}

void CItem::Release()
{
}

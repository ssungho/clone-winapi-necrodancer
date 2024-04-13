#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"


CTile::CTile()
	: m_iDrawID(0), m_iOption(0), m_iShiftValue(0), m_bFlag(false),
	  m_ePreCoinX(COIN_END), m_eCurCoinX(COIN_END),
	  m_bOnHere(false), m_pOwnerObj(nullptr), m_bIsStair(false)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
	m_tFrame = {0, 2, 0, 522, GetTickCount()};
	m_pFrameKey = L"Boss_Floor";
	m_eRenderID = RENDER_BACKGROUND; // 상관없긴함

	m_eCurCoinX = COIN_X_1;
	m_iShiftValue = 1;
}

int CTile::Update()
{
	__super::Update_Rect();
	return 0;
}

void CTile::Late_Update()
{
	Move_Tile_Frame();
}

void CTile::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
	                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
	                  TILECX, TILECY,
	                  hMemDC,
	                  m_tFrame.iFrameStart * 52 + 2,
	                  m_tFrame.iMotion * 52 + 2,
	                  TILECX, TILECY,
	                  MAGENTA);
}

void CTile::Release()
{
}

void CTile::Move_Tile_Frame()
{
	if (m_ePreCoinX != m_eCurCoinX)
	{
		switch (m_eCurCoinX)
		{
		case COIN_X_1:
			if (m_bFlag)
			{
				m_tFrame.iMotion = 0;
				m_iShiftValue = 1;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iFrameStart = 0;
			}
			else
			{
				m_tFrame.iMotion = 0;
				m_iShiftValue = 2;
				m_tFrame.iFrameEnd = 2;
				m_tFrame.iFrameStart = 2;
			}
			break;

		case COIN_X_2:
			if (m_bFlag)
			{
				m_tFrame.iMotion = 1;
				m_iShiftValue = 1;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iFrameStart = 0;
			}
			else
			{
				m_tFrame.iMotion = 1;
				m_iShiftValue = 2;
				m_tFrame.iFrameEnd = 2;
				m_tFrame.iFrameStart = 2;
			}
			break;
		}

		m_ePreCoinX = m_eCurCoinX;
	}

	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		m_tFrame.iFrameStart += m_iShiftValue;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}

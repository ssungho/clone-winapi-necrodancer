#include "stdafx.h"
#include "Effect.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CEffect::CEffect()
	: m_eEffectType(EFFECT_END), m_dwTime(GetTickCount()), m_iLifeTime(0)
{
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	m_eObjID = OBJ_EFFECT;
	m_eRenderID = RENDER_EFFECT;
}

int CEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	if (m_eEffectType == EFFECT_TOOL)
	{
		if (m_dwTime + m_iLifeTime < GetTickCount())
			m_bDead = true;
	}
	else
	{
		m_bDead = Move_Effect_Frame();
	}
}

void CEffect::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_eEffectType == EFFECT_DAGGER ||
		m_eEffectType == EFFECT_MONSTER_ATTACK ||
		m_eEffectType == EFFECT_SWORD)
	{
		if (m_eDir == LEFT || m_eDir == RIGHT)
		{
			GdiTransparentBlt(hDC,
			                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
			                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
			                  hMemDC,
			                  m_tFrame.iFrameStart * static_cast<int>(m_tInfo.fCX),
			                  m_tFrame.iMotion * static_cast<int>(m_tInfo.fCY),
			                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
			                  MAGENTA);
		}
		else if (m_eDir == UP || m_eDir == DOWN)
		{
			GdiTransparentBlt(hDC,
			                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
			                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
			                  hMemDC,
			                  m_tFrame.iMotion * static_cast<int>(m_tInfo.fCX),
			                  m_tFrame.iFrameStart * static_cast<int>(m_tInfo.fCY),
			                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
			                  MAGENTA);
		}
	}
	else if (m_eEffectType == EFFECT_TOOL)
	{
		GdiTransparentBlt(hDC,
		                  m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
		                  hMemDC,
		                  0, 0,
		                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
		                  MAGENTA);
	}
	else if (m_eEffectType == EFFECT_FIREBALL)
	{
		GdiTransparentBlt(hDC,
		                  m_tRect.left + iScrollX, m_tRect.top + iScrollY - 12, // ��ġ������ 12.f
		                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
		                  hMemDC,
		                  m_tFrame.iFrameStart * static_cast<int>(m_tInfo.fCX),
		                  m_tFrame.iMotion * static_cast<int>(m_tInfo.fCY),
		                  static_cast<int>(m_tInfo.fCX), static_cast<int>(m_tInfo.fCY),
		                  MAGENTA);
	}
}

void CEffect::Release()
{
}

void CEffect::Set_Effect(const TCHAR* _pFrameKey, EFFECTID _eEffectType, float _fX, float _fY, int _iLifeTime)
{
	m_pFrameKey = _pFrameKey;
	m_eEffectType = _eEffectType;
	m_tInfo.fCX = _fX;
	m_tInfo.fCY = _fY;
	m_iLifeTime = _iLifeTime;
}

int CEffect::Move_Effect_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

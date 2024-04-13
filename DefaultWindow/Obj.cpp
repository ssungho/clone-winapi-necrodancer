#include "stdafx.h"
#include "Obj.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "Particle.h"

CObj::CObj()
	: m_eDir(DIR_END), m_eRenderID(RENDER_END), m_fPreYPos(0), m_bMove(false), m_fMoveTime(0), m_fSpeed(0.f),
	  m_bDead(false), m_iMaxHp(0), m_iHp(0), m_iAttack(0), m_iDefense(0), m_iCoin(0), m_iDiamond(0),
	  m_bDetected(false), m_bAttacked(false), m_iBpm(0), m_iBeatCount(0), m_bIsFlip(false), m_eObjID(OBJ_END),
	  m_pTarget(nullptr), m_eDeadPoint(SC_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj()
{
}

void CObj::Draw_Particle()
{
	Draw_Particle(m_pParticleKey);
}

void CObj::Draw_Particle(TCHAR* pKey)
{
	auto eDir = CObjMgr::Get_Instance()->Get_Player()->Get_Dir();

	int iDir;
	if (eDir == UP)
	{
		iDir = 0;
	}
	else if (eDir == LEFT)
	{
		iDir = 1;
	}
	else if (eDir == DOWN)
	{
		iDir = 2;
	}
	else
	{
		iDir = 3;
	}

	float fX = m_tInfo.fX + (float)(iDx[iDir] * TILECX) * 0.5f;
	float fY = m_tInfo.fY + (float)(iDy[iDir] * TILECY) * 0.5f;

	if (eDir == LEFT || eDir == RIGHT)
	{
		for (int i = 0; i < 12; i++)
		{
			auto pParticle = CAbstractFactory<CParticle>::Create(m_tInfo.fX, m_tInfo.fY);
			pParticle->Set_FrameKey(pKey);
			pParticle->Set_Pos(m_tInfo.fX, fY - 10.f - (float)i);
			static_cast<CParticle*>(pParticle)->Set_Angle(5.f + 5.f * (float)i);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pParticle);
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			auto pParticle = CAbstractFactory<CParticle>::Create(m_tInfo.fX, m_tInfo.fY);
			pParticle->Set_FrameKey(pKey);
			pParticle->Set_Pos(m_tInfo.fX, fY);
			static_cast<CParticle*>(pParticle)->Set_Angle(40.f + 0.2f * (float)i);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pParticle);
		}
	}
}

void CObj::Update_Rect()
{
	m_tRect.left = static_cast<long>(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = static_cast<long>(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = static_cast<long>(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = static_cast<long>(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}

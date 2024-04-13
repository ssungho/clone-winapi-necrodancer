#include "stdafx.h"
#include "ObjMgr.h"
#include "Beat.h"
#include "BeatHeart.h"
#include "CollisionMgr.h"
#include "Inven.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
	: m_pBeatHeart(nullptr), m_llCurBeatCount(0)
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	if (m_llCurBeatCount > 1)
	{
		static_cast<CBeatHeart*>(m_pBeatHeart)->Heart_Beat();

		for (auto& pMonster : m_ObjList[OBJ_MONSTER])
		{
			pMonster->Add_Beat_Count();
		}

		m_llCurBeatCount = 0;
	}

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID eID = iter->Get_GroupID();
			if (eID == RENDER_END)
				continue;
			m_RenderList[eID].push_back(iter);
		}
	}

	if (m_pBeatHeart)
	{
		CCollisionMgr::Collision_Beat(m_pBeatHeart, m_ObjList[OBJ_BEAT], m_llCurBeatCount);
	}
}

void CObjMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)-> bool
		{
			return pDst->Get_Info().fY < pSrc->Get_Info().fY;
		});

		for (auto& iter : m_RenderList[i])
		{
			iter->Render(hDC);
		}

		m_RenderList[i].clear();
	}
}

void CObjMgr::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

void CObjMgr::Delete_Scene_Obj()
{
	Delete_ID(OBJ_TRAP);
	Delete_ID(OBJ_ITEM);
	Delete_ID(OBJ_MONSTER);
	Delete_ID(OBJ_EFFECT);
	Delete_ID(OBJ_WALL);
	Delete_ID(OBJ_BEAT);
	Delete_ID(OBJ_UI);
	Delete_ID(OBJ_TRAP);
	Clear_Render_List();
}

void CObjMgr::Clear_Render_List()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].clear();
	}
}

bool CObjMgr::On_Beat()
{
	if (!m_ObjList[OBJ_BEAT].empty())
	{
		for (auto& pBeat : m_ObjList[OBJ_BEAT])
		{
			if (static_cast<CBeat*>(pBeat)->Get_OnHeart())
				return true;
		}
	}

	return false;
}

bool CObjMgr::Hit_Beat()
{
	int iCnt = 0;

	if (!m_ObjList[OBJ_BEAT].empty())
	{
		for (auto& pBeat : m_ObjList[OBJ_BEAT])
		{
			if (static_cast<CBeat*>(pBeat)->Get_OnHeart())
			{
				pBeat->Set_Dead();
				++iCnt;
			}
		}
	}

	// 비트가 안맞았다면
	if (iCnt == 0)
	{
		return false;
	}

	// 박자 카운트 증가
	m_llCurBeatCount += 2;

	return true;
}

void CObjMgr::Player_Damaged(int _iDamage)
{
	int iDefense = CInven::Get_Instance()->Get_Defense();

	int iRealDamage = _iDamage - iDefense;

	if (iRealDamage < 1)
		iRealDamage = 1;

	m_ObjList[OBJ_PLAYER].front()->On_Damaged(iRealDamage);
}

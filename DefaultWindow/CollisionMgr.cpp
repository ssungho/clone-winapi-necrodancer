#include "stdafx.h"
#include "CollisionMgr.h"
#include "Beat.h"
#include "ObjMgr.h"

CCollisionMgr::CCollisionMgr() = default;

CCollisionMgr::~CCollisionMgr() = default;

#pragma region ���� �Ⱦ����ִ� �浹ó��

void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rcRect{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rcRect, &(Dst->Get_Rect()), &(Src->Get_Rect())))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
{
	float fX(0.f), fY(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY) // ���� �浹
				{
					// �� �浹
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
					{
						Dst->Set_PosY(-fY);
					}
					// �� �浹
					else
					{
						Dst->Set_PosY(fY);
					}
				}

				else // �¿� �浹
				{
					// �� �浹
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
					{
						Dst->Set_PosX(-fX);
					}
					// �� �浹
					else
					{
						Dst->Set_PosX(fX);
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY)
{
	float fWidth = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float fRadiusX = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	float fRadiusY = (pDst->Get_Info().fCY + pSrc->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
		*pX = fRadiusX - fWidth;
		*pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj* pDst, CObj* pSrc)
{
	float fWidth = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float fDiagonal = sqrt(fWidth * fWidth + fHeight * fHeight);

	float fRadius = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;

	return fRadius >= fDiagonal;
}

#pragma endregion

// ��Ʈ �浹
void CCollisionMgr::Collision_Beat(CObj* _pHeart, list<CObj*> _BeatList, UINT& _pCnt)
{
	for (auto& pBeat : _BeatList)
	{
		if (On_Range_Beat(_pHeart, pBeat))
		{
			Cross_Heart(_pHeart, pBeat);

			if (Escape_Range_Beat(_pHeart, pBeat))
			{
				pBeat->Set_Dead();
				++_pCnt;
			}
		}
	}
}

bool CCollisionMgr::On_Range_Beat(CObj* _pHeart, CObj* _pBeat)
{
	auto pBeat = static_cast<CBeat*>(_pBeat);

	if (pBeat->Get_OnHeart())
		return true;

	if (pBeat->Get_Dir() == RIGHT) // ���ʿ��� ���� ���
	{
		if (pBeat->Get_Rect().right >= _pHeart->Get_Rect().left - HEART_EXTRA_SIZE + 5)
		{
			pBeat->Set_OnHeart();
			return true;
		}
	}
	else // �����ʿ��� ���� ���
	{
		if (pBeat->Get_Rect().left <= _pHeart->Get_Rect().right + HEART_EXTRA_SIZE - 5)
		{
			pBeat->Set_OnHeart();
			return true;
		}
	}

	return false;
}

bool CCollisionMgr::Escape_Range_Beat(CObj* _pHeart, CObj* _pBeat)
{
	auto pBeat = dynamic_cast<CBeat*>(_pBeat);

	// pBeat�� nullptr�̸� ũ����
	assert(pBeat);

	if (pBeat->Get_OnHeart())
	{
		if (pBeat->Get_Dir() == RIGHT) // ���ʿ��� ���� ���
		{
			if (pBeat->Get_Info().fX >= _pHeart->Get_Info().fX + HEART_EXTRA_SIZE + 5)
			{
				return true;
			}
		}
		else // �����ʿ��� ���� ���
		{
			if (pBeat->Get_Info().fX <= _pHeart->Get_Info().fX - HEART_EXTRA_SIZE - 5)
			{
				return true;
			}
		}
	}

	return false;
}

void CCollisionMgr::Cross_Heart(CObj* _pHeart, CObj* _pBeat)
{
	if (_pBeat->Get_Dir() == RIGHT)
	{
		if (_pHeart->Get_Info().fX <= _pBeat->Get_Info().fX)
			static_cast<CBeat*>(_pBeat)->Set_Off();
	}
	else
	{
		if (_pHeart->Get_Info().fX >= _pBeat->Get_Info().fX)
			static_cast<CBeat*>(_pBeat)->Set_Off();
	}
}

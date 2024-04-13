#include "stdafx.h"
#include "CollisionMgr.h"
#include "Beat.h"
#include "ObjMgr.h"

CCollisionMgr::CCollisionMgr() = default;

CCollisionMgr::~CCollisionMgr() = default;

#pragma region 현재 안쓰고있는 충돌처리

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
				if (fX > fY) // 상하 충돌
				{
					// 상 충돌
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
					{
						Dst->Set_PosY(-fY);
					}
					// 하 충돌
					else
					{
						Dst->Set_PosY(fY);
					}
				}

				else // 좌우 충돌
				{
					// 좌 충돌
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
					{
						Dst->Set_PosX(-fX);
					}
					// 우 충돌
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

// 비트 충돌
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

	if (pBeat->Get_Dir() == RIGHT) // 왼쪽에서 오는 경우
	{
		if (pBeat->Get_Rect().right >= _pHeart->Get_Rect().left - HEART_EXTRA_SIZE + 5)
		{
			pBeat->Set_OnHeart();
			return true;
		}
	}
	else // 오른쪽에서 오는 경우
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

	// pBeat가 nullptr이면 크래시
	assert(pBeat);

	if (pBeat->Get_OnHeart())
	{
		if (pBeat->Get_Dir() == RIGHT) // 왼쪽에서 오는 경우
		{
			if (pBeat->Get_Info().fX >= _pHeart->Get_Info().fX + HEART_EXTRA_SIZE + 5)
			{
				return true;
			}
		}
		else // 오른쪽에서 오는 경우
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

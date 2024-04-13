#pragma once

#include "Obj.h"

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);

	static void Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);
	static bool Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY);

	static void Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src);
	static bool Check_Sphere(CObj* pDst, CObj* pSrc);

	// 비트 충돌 관련 함수들
	static void Collision_Beat(CObj* _pHeart, list<CObj*> _BeatList, UINT& _pCnt);
	static bool On_Range_Beat(CObj* _pHeart, CObj* _pBeat);
	static bool Escape_Range_Beat(CObj* _pHeart, CObj* _pBeat);
	static void Cross_Heart(CObj* _pHeart, CObj* _pBeat);
};

#pragma once

#include "Effect.h"
#include "Obj.h"
#include "Item.h"
#include "TileMgr.h"

template <typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create(float _fX, float _fY)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		return pObj;
	}

	// Bpm 필요한 UI
	static CObj* Create(float _fX, float _fY, int _iBpm, bool _isUI)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Bpm(_iBpm);

		return pObj;
	}

	static CObj* Create(float _fX, float _fY, DIRECTION _eDir)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Dir(_eDir);

		return pObj;
	}

	// 타일 위에 생성
	static CObj* Create(CObj* _pTile)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_pTile->Get_Info().fX, _pTile->Get_Info().fY);

		return pObj;
	}

	// 키, 크기, 위치, 비피엠을 받는 생성
	static CObj* Create(TCHAR* pFrameKey, float _fCX, float _fCY, float _fPosX, float _fPosY, int iBpm)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_FrameKey(pFrameKey);
		pObj->Set_Scale(_fCX, _fCY);
		pObj->Set_Pos(_fPosX, _fPosY);
		pObj->Set_Bpm(iBpm);

		return pObj;
	}

	// 키, 크기, 위치(타일), 비피엠을 받는 생성
	static CObj* Create(TCHAR* pFrameKey, float _fCX, float _fCY, CTile* _pTile, int iBpm)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_FrameKey(pFrameKey);
		pObj->Set_Scale(_fCX, _fCY);
		pObj->Set_Pos(_pTile);
		pObj->Set_Bpm(iBpm);

		return pObj;
	}

	// UI에 보여지는 아이템 생성(장비류)
	static CObj* Create_UI_Item(ITEMID _eItemID, TCHAR* pFrameKey, float _fCX, float _fCY)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_FrameKey(pFrameKey);
		pObj->Set_Scale(_fCX, _fCY);

		static_cast<T*>(pObj)->Set_ItemID(_eItemID);
		static_cast<T*>(pObj)->Set_Slot(true);

		return pObj;
	}

	// 필드 위 아이템 생성(장비류)
	static CObj* Create_Item(ITEMID _eItemID, TCHAR* pFrameKey, CObj* _pTile)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(_pTile->Get_Info().fX, _pTile->Get_Info().fY);
		pObj->Set_FrameKey(pFrameKey);
		pObj->Set_Scale(ITEMCX, ITEMCY);

		static_cast<T*>(pObj)->Set_ItemID(_eItemID);
		static_cast<T*>(pObj)->Set_Slot(false);
		static_cast<CTile*>(_pTile)->Set_Owner(pObj);

		return pObj;
	}

	// 타일 위에 생성 (소유자 지정 x)
	static CObj* Create(CObj* _pTile, int _iBpm)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_pTile->Get_Info().fX, _pTile->Get_Info().fY);
		pObj->Set_Bpm(_iBpm);

		return pObj;
	}

	// 스테이지에서 사용 얘도 소유자 지정해줌
	static CObj* Create(int _iTileIndex, int _iBpm)
	{
		CObj* pObj = new T;
		pObj->Initialize();

		auto pTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(_iTileIndex));
		pTile->Set_Owner(pObj);

		pObj->Set_Pos(pTile->Get_Info().fX, pTile->Get_Info().fY);
		pObj->Set_Bpm(_iBpm);

		return pObj;
	}

	// 소유자를 지정해주는 생성.
	static CObj* Create(float _fX, float _fY, int _iBpm)
	{
		CObj* pObj = new T;
		pObj->Initialize();

		auto pTile = static_cast<CTile*>(CTileMgr::Get_Instance()->Get_Tile(_fX, _fY));
		pTile->Set_Owner(pObj);
		pObj->Set_Pos(pTile->Get_Info().fX, pTile->Get_Info().fY);
		pObj->Set_Bpm(_iBpm);

		return pObj;
	}

public:
	CAbstractFactory()
	{
	}

	~CAbstractFactory()
	{
	}
};

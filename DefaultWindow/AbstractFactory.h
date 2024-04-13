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

	// Bpm �ʿ��� UI
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

	// Ÿ�� ���� ����
	static CObj* Create(CObj* _pTile)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_pTile->Get_Info().fX, _pTile->Get_Info().fY);

		return pObj;
	}

	// Ű, ũ��, ��ġ, ���ǿ��� �޴� ����
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

	// Ű, ũ��, ��ġ(Ÿ��), ���ǿ��� �޴� ����
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

	// UI�� �������� ������ ����(����)
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

	// �ʵ� �� ������ ����(����)
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

	// Ÿ�� ���� ���� (������ ���� x)
	static CObj* Create(CObj* _pTile, int _iBpm)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_pTile->Get_Info().fX, _pTile->Get_Info().fY);
		pObj->Set_Bpm(_iBpm);

		return pObj;
	}

	// ������������ ��� �굵 ������ ��������
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

	// �����ڸ� �������ִ� ����.
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

#pragma once
#include "Obj.h"
#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	void Picking_Tile(POINT ptMouse, int iDrawID, int iOption);
	void Set_DrawID(int _iDrawId) { m_iDrawID = _iDrawId; }

public:
	void Save_Tile();
	void Load_Tile();

	// 지금 만들고있는 스테이지임.
	void Load_Stage01_Tile();
	void Load_Stage01_Tile(int _iBpm);
	void Late_Update_Stage01_Tile();

	void Load_Stage02_Tile();
	void Load_Stage02_Tile(int _iBpm);
	void Late_Update_Stage02_Tile();

	void Load_Boss_Stage_Tile();
	void Load_Boss_Stage_Tile(int _iBpm);
	void Late_Update_Boss_Stage();

	void Reset_RestoreTile();

	// 개쩌리
	void Late_Update_Stage1();
	void Load_Stage1_Tile();

public:
	CObj* Get_Tile(float _fX, float _fY) const
	{
		int iCol = static_cast<int>(_fX) / TILECX;
		int iRow = static_cast<int>(_fY) / TILECY;
		unsigned int iIndex = iRow * TILEX + iCol;

		if (iIndex >= m_vecTile.size())
			return nullptr;

		return m_vecTile[iIndex];
	}

	CObj* Get_Tile(int _iX, int _iY) const
	{
		int iCol = _iX / TILECX;
		int iRow = _iY / TILECY;
		unsigned int iIndex = iRow * TILEX + iCol;

		if (iIndex >= m_vecTile.size())
			return nullptr;

		return m_vecTile[iIndex];
	}

	CObj* Get_Tile(int _iIndex) const
	{
		if (static_cast<unsigned int>(_iIndex) >= m_vecTile.size())
			return nullptr;
		return m_vecTile[_iIndex];
	}

	CObj* Get_Tile(CObj* _pObj) const;
	CObj* Get_Tile_On_Player() const;

	int Get_TIle_Index(float _fX, float _fY) const
	{
		int iCol = static_cast<int>(_fX) / TILECX;
		int iRow = static_cast<int>(_fY) / TILECY;
		unsigned int iIndex = iRow * TILEX + iCol;

		if (iIndex >= m_vecTile.size())
			return -1;

		return iIndex;
	}

	void Set_Coin_Mode(COIN_X _eCoinX) const
	{
		for (auto& pTile : m_vecTile)
			static_cast<CTile*>(pTile)->Set_CoinX(_eCoinX);
	}

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

		return m_pInstance;
	}

	static void Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTileMgr* m_pInstance;
	vector<CObj*> m_vecTile;

	int m_iDrawID;
	int m_iOption;
};

// DrawID 2 == 배경타일!!

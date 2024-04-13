#pragma once

#include "MyBmp.h"

class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC Find_Image(const TCHAR* pImgKey);
	void Release();
	void Load_AllBmp();

private:
	void Load_PlayerBmp();
	void Load_MonsterBmp();
	void Load_EnvBmp();
	void Load_EffectBmp();
	void Load_MenuBmp();
	void Load_GUIBmp();
	void Load_ItemBmp();

public:
	static CBmpMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CBmpMgr;

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
	static CBmpMgr* m_pInstance;
	map<const TCHAR*, CMyBmp*> m_mapBit;
};

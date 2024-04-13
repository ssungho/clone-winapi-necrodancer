#pragma once

#include "Obj.h"
#include "Define.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	CObj* Get_Player()
	{
		if (m_ObjList[OBJ_PLAYER].empty())
			return nullptr;
		return m_ObjList[OBJ_PLAYER].front();
	}

	void Set_BeatHeart(CObj* _pBeatHeart)
	{
		m_pBeatHeart = _pBeatHeart;
	}

public:
	void Add_Object(OBJID eID, CObj* pObj);
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
	void Delete_ID(OBJID eID);

	void Delete_Scene_Obj();
	void Clear_Render_List();

public:
	bool On_Beat(); // ��Ʈ �Է�
	bool Hit_Beat(); // ��Ʈ ����
	void Reset_Beat()
	{
		m_llCurBeatCount = 0;
	} // �ֱ������� ��Ʈ�� �������ִ°� ����?

public:
	void Player_Damaged(int _iDamage);

private:
	list<CObj*> m_ObjList[OBJ_END];
	list<CObj*> m_RenderList[RENDER_END];

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

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
	static CObjMgr* m_pInstance;
	CObj* m_pBeatHeart;

	UINT m_llCurBeatCount;
};

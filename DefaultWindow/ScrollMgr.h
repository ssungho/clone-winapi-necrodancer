#pragma once

#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	float Get_ScrollX() const { return m_fScrollX; }
	float Get_ScrollY() const { return m_fScrollY; }

	void Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void Set_ScrollY(float _fY) { m_fScrollY += _fY; }

	void Scroll_Lock();
	void Scroll_Shake();

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;

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
	static CScrollMgr* m_pInstance;

	float m_fScrollX;
	float m_fScrollY;
};

#include "stdafx.h"
#include "Mouse.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
}

int CMouse::Update()
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;

	__super::Update_Rect();

	ShowCursor(false);

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	/*GdiTransparentBlt(hDC,
	                  m_tRect.left, m_tRect.top,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  hDC,0,0,
	                  (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	                  MAGENTA);*/
}

void CMouse::Release()
{
}

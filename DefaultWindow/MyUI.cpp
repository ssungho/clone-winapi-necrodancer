#include "stdafx.h"
#include "MyUI.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CMyUI::CMyUI(): m_iOptionNumber(0)
{
}

CMyUI::~CMyUI()
{
	Release();
}

void CMyUI::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
}

int CMyUI::Update()
{
	__super::Update_Rect();
	return 0;
}

void CMyUI::Late_Update()
{
}

void CMyUI::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	// auto iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	// auto iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	TCHAR strNum[32];
	swprintf_s(strNum, L"Option : %d", m_iOptionNumber);

	// TextOut(hDC, iScrollX + m_tInfo.fX - m_tInfo.fCX * 0.5f, iScrollY + m_tInfo.fY - m_tInfo.fCY * 0.5f, strNum, lstrlen(strNum));
	TextOut(hDC, m_tInfo.fX - m_tInfo.fCX * 0.5f, m_tInfo.fY - m_tInfo.fCY * 0.5f, strNum, lstrlen(strNum));

	GdiTransparentBlt(hDC,
	                  m_tRect.left,
	                  m_tRect.top,
	                  (int)m_tInfo.fCX,
	                  (int)m_tInfo.fCY,
	                  hMemDC,
	                  0,
	                  0,
	                  (int)m_tInfo.fCX,
	                  (int)m_tInfo.fCY,
	                  MAGENTA);
}

void CMyUI::Release()
{
}

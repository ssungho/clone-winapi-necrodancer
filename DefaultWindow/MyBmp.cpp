#include "stdafx.h"
#include "MyBmp.h"


CMyBmp::CMyBmp()
{
}


CMyBmp::~CMyBmp()
{
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC hDC = GetDC(g_hWnd);

	// ȭ��DC�� ȣȯ�Ǵ� DC�� �Ҵ�
	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL, // ���α׷� �ν��Ͻ� �ڵ�
	                               pFilePath, // �̹��� ������ ���
	                               IMAGE_BITMAP, // � Ÿ���� �о���� ����
	                               0, // �̹����� ����, ���� ũ��
	                               0,
	                               LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//LR_LOADFROMFILE : ���Ϸκ��� �̹����� �о���δٴ� �ɼ�
	// LR_CREATEDIBSECTION : �о�� ������ DIB ���·� ��ȯ

	if (m_hBitmap == NULL)
		MessageBox(g_hWnd, pFilePath, L"Image Load Failed!", NULL);


	// SelectObject : �غ��� DC�� GDI ������Ʈ�� �ҷ� ��Ʈ���� �����ϰ� ����� �Լ�
	// ���� �� �ڵ�� GDI ������Ʈ�� �����ϱ� �� ������ ������ �ִ� ������Ʈ�� ��ȯ�ϰ� �����.
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}

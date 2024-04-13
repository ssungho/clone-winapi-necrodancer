// DefaultWindow.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "DefaultWindow.h"

#include "Define.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst; // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING]; // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING]; // �⺻ â Ŭ���� �̸��Դϴ�.
HWND g_hWnd;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // HINSTANCE : ���� ���α׷��� ���� �ĺ� ��ȣ (Instance : �޸𸮿� �Ҵ�Ǵ� ��ü)
                      _In_opt_ HINSTANCE hPrevInstance, // ���� ����Ǿ��� �ν��Ͻ��� �ڵ�(���� ��� NULL)
                      _In_ LPWSTR lpCmdLine, // LPWSTR : �����ڵ� ��� ���ڿ� ������
                      _In_ int nCmdShow) // ������ â�� ��Ÿ���� �����ϴ� flag �� (��� / �ּ�ȭ, �ִ�ȭ ��� ����� �ɼ�)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

	MSG msg;
	msg.message = WM_NULL;

	CMainGame MainGame;
	MainGame.Initialize();

	DWORD dwTime = GetTickCount();

	while (true)
	{
		// PM_REMOVE		: �޼����� �о�Ȱ� ���ÿ� �޼��� ť���� ����
		// PM_NOREMOVE		: �޼��� ť�� �޼����� �����ϴ����� �ľ�, ���� �޼����� �о������ GetMessage�� �ٽ� ȣ���ؾ� ��
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{
			if (dwTime + 10 < GetTickCount())
			{
				MainGame.Update();
				MainGame.Late_Update();
				MainGame.Render();

				dwTime = GetTickCount();
			}
		}
	}


	return (int)msg.wParam;
}


//
//  �Լ�: MyRegisterClass()
//
//  ����: ���� �����츦 ����� ���� �����쿡 ���� ������ ���
//	� ������ � �Ӽ��� �������� �����ϴ� �Լ�

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//WNDCLASSEXW : �����츦 ���� �� â�� ���� ������ �����ϴ� �뵵�� ����ü

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); // �ڱ� �ڽ��� ����� ������ ����

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	// ������ â�� ��Ÿ���� ����, �ʱ�ȭ �Ǵ� ���� �ǹ��ϴ� ���� ������ â�� ����, �Ǵ� ������ ũ�Ⱑ ���� ��� �����츦 �ٽ� �׸��ٴ� �ǹ�

	// CS_HREDRAW : ���θ� �ٽ� �׸���.
	// CS_VREDRAW : ���θ� �ٽ� �׸���.

	wcex.lpfnWndProc = WndProc; // �޼��� ó���ϴ� �Լ�, �����ص� �Լ��� ȣ��Ǿ� �޼��� ó���� ����

	wcex.cbClsExtra = 0; // �����찡 Ư���� �������� ����ϴ� ������ ����    
	wcex.cbWndExtra = 0; // ������ ���� �������� 0�� �����ϸ� ���� ������ ������� �ʰڴٴ� ��

	wcex.hInstance = hInstance; //���� �Լ����� �Ű� ������ ���޵� ��, ���α׷��� ��ȣ�� ����

	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
	// ������ â�� ����� �������� ����(���� ǥ����)

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	// ������ â���� ����� ���콺 Ŀ���� ����

	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// ������ �۾� ������ ��� ���� ����

	wcex.lpszMenuName = NULL;
	// â ��ܿ� �ִ� �޴� ����

	wcex.lpszClassName = szWindowClass;
	// ���� ������ �̸��� ����(�������̸� ������Ʈ �̸��� ��ġ��Ŵ, �ǵ帮����)

	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	// â ��ܿ� ����� ���� ������ ��� ����

	return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	// CreateWindowW : ����� ���� �������� ���� â�� ����� �Լ�

	RECT rcWnd{0, 0, WINCX, WINCY};

	//  rcWnd = rcWnd + �⺻ â ��Ÿ�� + �޴� �� ������ ���
	AdjustWindowRect(&rcWnd, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hWnd = CreateWindowW(szWindowClass, // �ռ� ������ �������� ������ �̸�
	                          szTitle, // â ��ܿ� ����� ���ڿ�
	                          WS_OVERLAPPEDWINDOW, // â ��Ÿ�� ���� �ɼ�(�⺻ ������ ����ϴ� ���)
	                          CW_USEDEFAULT, 0, // â�� ����� X, Y ��ǥ
	                          rcWnd.right - rcWnd.left,
	                          rcWnd.bottom - rcWnd.top, // â�� ����, ���� ������
	                          nullptr, // �θ� �����찡 �ִٸ� �θ� �������� �ڵ��� ����	
	                          nullptr, // �����쿡�� ����� �޴��� �ڵ��� ����, �⺻�� ������ NULL
	                          hInstance, // �����츦 ����� ��ü
	                          nullptr); // � ü���� Ư���� �������� ���

	if (!hWnd)
	{
		return FALSE;
	}

	g_hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//

// �⺻���� ���̺귯��
// 1. KERNEL : �޸𸮸� �����ϰ� ���α׷��� ����
// 2. USER : ���� �������̽��� ������(â)�� ����
// 3. GDI : ȭ�� ó���� �׷����� ���

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// �޴� ������ ���� �м��մϴ�.
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			//PostQuitMessage(0);
			DestroyWindow(hWnd);
			break;
		}

		break;


	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


// �Լ� ȣ�� �Ծ�

// : �Լ��� ������ ����Ǹ� �Ҵ� �Ǿ��� ����(���� ������)�� �����ؾ� �ϴµ� �Լ��� ȣ���� ȣ���(caller)���� ������ ������
// �ƴϸ� ȣ����� ��ȣ����(callee)���� �� ���ΰ��� ���� ���

// __cdecl, __stdcall, __fastcall, __thiscall

// __cdecl    : c/c++ ����� �⺻ ȣ�� �Ծ�, ȣ����� stack�� ����, � �Ű� ������ �󸶳� ������ �𸣴� ����(���� ���� �Լ�) ������ �Լ��� ȣ���� ȣ����� �޸𸮸� ����Ͽ� ��ȯ
// __stdcall  : win32 api �⺻ ȣ�� �Ծ�, ��ȣ���ڰ� stack�� ����, api���� �����ϴ� ��κ��� �Լ����� ���� ���� �Լ�, �Ű� ������ �����Ǿ� �־� ��ȣ���ڰ� � ���� �޸� �� ��ϵ��� �˰� �־ ������ stack�� ������ �� ����
// __fastcall : �Լ� ȣ���� ������ ó���ϱ� ���� ȣ�� �Ծ�, �������͸� ����Ͽ� �Լ� ȣ���� ������
// __thiscall : this �����͸� �Ű� ������ �Ѱ� �޾��� ��(��� �Լ����� ����)

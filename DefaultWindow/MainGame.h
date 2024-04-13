#pragma once

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update(); // 실시간으로(매 프레임마다) 변경되는 값을 만드는 함수
	void Late_Update();
	void Render(); // 실시간으로(매 프레임마다) 출력하는 함수
	void Release();

private:
	HDC m_hDC;

	DWORD m_dwTime;
	int m_iFPS;
	TCHAR m_szFPS[32];
};

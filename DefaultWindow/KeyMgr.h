#pragma once

#include "Define.h"

enum class KeyType
{
	LEFT_MOUSE = VK_LBUTTON,
	RIGHT_MOUSE = VK_RBUTTON,

	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,
	SPACE = VK_SPACE,

	ENTER = VK_RETURN,
	F1 = VK_F1,
	F2 = VK_F2,

	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	Q = 'Q',
	E = 'E'
};

enum class KeyState
{
	None,
	Press,
	Down,
	Up,
	End
};

enum
{
	KEY_TYPE_COUNT = static_cast<int>(UINT8_MAX) + 1,
	KEY_STATE_COUNT = static_cast<int>(KeyState::End)
};

class CKeyMgr
{
	DECLARE_SINGLE(CKeyMgr);

public:
	void Initialize();
	void Update();

public:
	//누르고 있을 때
	bool Get_Button(KeyType key) { return Get_State(key) == KeyState::Press; }

	//맨 처음 눌렀을 때
	bool Get_Button_Down(KeyType key) { return Get_State(key) == KeyState::Down; }

	//맨 처음 눌렀다가 땔 때
	bool Get_Button_Up(KeyType key) { return Get_State(key) == KeyState::Up; }

	bool Get_Arrow_Input();

	POINT Get_Mouse_Pos() { return m_mousePos; }

private:
	KeyState Get_State(KeyType Key) { return m_states[static_cast<unsigned int>(Key)]; }

private:
	HWND m_hWND = NULL;
	vector<KeyState> m_states;
	POINT m_mousePos;
};

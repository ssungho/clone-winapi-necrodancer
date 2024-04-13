#include "stdafx.h"
#include "KeyMgr.h"

void CKeyMgr::Initialize()
{
	m_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void CKeyMgr::Update()
{
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	//Ű���� 256���� ���¸� ������ �ܾ��
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (unsigned int key = 0; key < KEY_TYPE_COUNT; key++)
	{
		//Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = m_states[key];

			//���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KeyState::Press || state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			else
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = m_states[key];

			//���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else
				state = KeyState::None;
		}
	}
	// Mouse
	::GetCursorPos(&m_mousePos);
	::ScreenToClient(g_hWnd, &m_mousePos);
}

bool CKeyMgr::Get_Arrow_Input()
{
	if (Get_Button_Down(KeyType::UP) || Get_Button_Down(KeyType::DOWN) ||
		Get_Button_Down(KeyType::RIGHT) || Get_Button_Down(KeyType::LEFT))
	{
		return true;
	}

	return false;
}

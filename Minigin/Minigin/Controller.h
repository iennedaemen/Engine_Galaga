#pragma once
#include <XInput.h>
#include <minwinbase.h>
#include <winerror.h>
#include "structs.h"

class Controller
{
public:
	Controller(int playerNr)
		: m_ContollerNr{ playerNr - 1 }
		, m_ControllerState{NULL}
	{}
	
	XINPUT_STATE GetState()
	{
		ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
		XInputGetState(m_ContollerNr, &m_ControllerState);

		return m_ControllerState;
	}

	bool IsConnected()
	{
		ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
		DWORD Result = XInputGetState(m_ContollerNr, &m_ControllerState);

		if (Result == ERROR_SUCCESS)
			return true;
		else
			return false;
	
	}

	bool IsPressed(ControllerButton button) const
	{
		if (m_ContollerNr == 0)
		{
			switch (button)
			{
			case ControllerButton::ButtonLeft:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) || GetAsyncKeyState(VK_LEFT);
			case ControllerButton::ButtonRight:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) || GetAsyncKeyState(VK_RIGHT);
			case ControllerButton::ButtonUp:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) || GetAsyncKeyState(VK_UP);
			case ControllerButton::ButtonDown:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) || GetAsyncKeyState(VK_DOWN);
			case ControllerButton::ButtonA:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) || GetAsyncKeyState(VK_SPACE);
			default: return false;
			}
		}

		else
		{
			switch (button)
			{
			case ControllerButton::ButtonLeft:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) || GetAsyncKeyState('A');
			case ControllerButton::ButtonRight:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) || GetAsyncKeyState('D');
			case ControllerButton::ButtonUp:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) || GetAsyncKeyState('W');
			case ControllerButton::ButtonDown:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) || GetAsyncKeyState('S');
			case ControllerButton::ButtonA:
				return (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) || GetAsyncKeyState('E');
			default: return false;
			}
		}
	}

private:
	int m_ContollerNr;
	XINPUT_STATE m_ControllerState;
	
};


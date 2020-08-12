#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

bool InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case ControllerButton::ButtonLeft:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
	case ControllerButton::ButtonRight:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
	case ControllerButton::ButtonUp:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
	case ControllerButton::ButtonDown:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
	case ControllerButton::ButtonA:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	default: return false;
	}
}

std::shared_ptr<Command> InputManager::HandleInput()
{
	if (IsPressed(ControllerButton::ButtonLeft)) return m_pButtonLeft;
	if (IsPressed(ControllerButton::ButtonRight)) return m_pButtonRight;
	if (IsPressed(ControllerButton::ButtonUp)) return m_pButtonUp;
	if (IsPressed(ControllerButton::ButtonDown)) return m_pButtonDown;
	if (IsPressed(ControllerButton::ButtonDown)) return m_pButtonA;

	return nullptr;
}

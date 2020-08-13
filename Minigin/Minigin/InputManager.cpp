#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool InputManager::ProcessInput()
{
	m_Contoller1->GetState();
	m_Contoller2->GetState();
	
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

bool InputManager::IsPressed(ControllerButton button, int playerNr) const
{
	if(playerNr == 1)
		return m_Contoller1->IsPressed(button);
	
	if (playerNr == 2)
		return m_Contoller2->IsPressed(button);

	return false;
}

std::shared_ptr<Command> InputManager::HandleInput(int playerNr)
{
	if (IsPressed(ControllerButton::ButtonLeft, playerNr)) return m_pButtonLeft;
	if (IsPressed(ControllerButton::ButtonRight, playerNr)) return m_pButtonRight;
	if (IsPressed(ControllerButton::ButtonUp, playerNr)) return m_pButtonUp;
	if (IsPressed(ControllerButton::ButtonDown, playerNr)) return m_pButtonDown;
	if (IsPressed(ControllerButton::ButtonA, playerNr)) return m_pButtonA;

	return nullptr;
}

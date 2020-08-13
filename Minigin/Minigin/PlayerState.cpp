#include "MiniginPCH.h"
#include "PlayerState.h"
#include <SDL.h>
#include "InputManager.h"
#include "SpriteComponent.h"

std::shared_ptr<PlayerState> WalkingState::handleInput(Player& player, Uint8 input)
{
    UNREFERENCED_PARAMETER(player);
    UNREFERENCED_PARAMETER(input);
	
    if (!InputManager::GetInstance().IsPressed(ControllerButton::ButtonLeft, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonRight, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonUp, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonDown, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonA, player.getPlayerNr()))
    {
        std::shared_ptr<IdleState> ptr1 = std::make_shared<IdleState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        player.GetComponent<SpriteComponent>()->SetCurrentFrame(0);

        return ptr2;
    }

    if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonA, player.getPlayerNr()))
    {
        std::shared_ptr<ShootingState> ptr1 = std::make_shared<ShootingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);
    	
        player.GetComponent<SpriteComponent>()->SetCurrentFrame(2);

        return ptr2;
    }
    return nullptr;
}

std::shared_ptr<PlayerState> ShootingState::handleInput(Player& player, Uint8 input)
{
    UNREFERENCED_PARAMETER(player);
    UNREFERENCED_PARAMETER(input);

    if (!InputManager::GetInstance().IsPressed(ControllerButton::ButtonLeft, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonRight, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonUp, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonDown, player.getPlayerNr())
        && !InputManager::GetInstance().IsPressed(ControllerButton::ButtonA, player.getPlayerNr()))
    {
        std::shared_ptr<IdleState> ptr1 = std::make_shared<IdleState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        player.GetComponent<SpriteComponent>()->SetCurrentFrame(0);
    	
        return ptr2;
    }
	
    if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonLeft, player.getPlayerNr())
        || InputManager::GetInstance().IsPressed(ControllerButton::ButtonRight, player.getPlayerNr())
        || InputManager::GetInstance().IsPressed(ControllerButton::ButtonUp, player.getPlayerNr())
        || InputManager::GetInstance().IsPressed(ControllerButton::ButtonDown, player.getPlayerNr()))
    { 
        std::shared_ptr<WalkingState> ptr1 = std::make_shared<WalkingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        player.GetComponent<SpriteComponent>()->SetCurrentFrame(1);
    	
        return ptr2;
    }
    return nullptr;
}

std::shared_ptr<PlayerState> IdleState::handleInput(Player& player, Uint8 input)
{
    UNREFERENCED_PARAMETER(player);
    UNREFERENCED_PARAMETER(input);

    if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonA, player.getPlayerNr()))
    {
        std::shared_ptr<ShootingState> ptr1 = std::make_shared<ShootingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        player.GetComponent<SpriteComponent>()->SetCurrentFrame(2);

        return ptr2;
    }
	
    if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonLeft, player.getPlayerNr())
        || InputManager::GetInstance().IsPressed(ControllerButton::ButtonRight, player.getPlayerNr())
        || InputManager::GetInstance().IsPressed(ControllerButton::ButtonUp, player.getPlayerNr())
        || InputManager::GetInstance().IsPressed(ControllerButton::ButtonDown, player.getPlayerNr()))
    {
        std::shared_ptr<WalkingState> ptr1 = std::make_shared<WalkingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        player.GetComponent<SpriteComponent>()->SetCurrentFrame(1);
    	
        return ptr2;
    }
    return nullptr;
}
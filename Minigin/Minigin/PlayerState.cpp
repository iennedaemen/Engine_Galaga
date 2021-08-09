#include "MiniginPCH.h"
#include "PlayerState.h"
#include <SDL.h>
#include "InputManager.h"
#include "SpriteComponent.h"


std::shared_ptr<PlayerState> IdleState::handleInput(Player& player, Uint8 input)
{
    UNREFERENCED_PARAMETER(player);
    UNREFERENCED_PARAMETER(input);
	/*
    if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonUp, player.getPlayerNr()))
    {
        std::shared_ptr<WalkingState> ptr1 = std::make_shared<WalkingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        if (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 0
            || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 2))
            player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0,0);

        if (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 1
            || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 3))
            player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0, 1);
    	
        return ptr2;
    }

    if(InputManager::GetInstance().IsPressed(ControllerButton::ButtonDown, player.getPlayerNr()))
    {
        std::shared_ptr<WalkingState> ptr1 = std::make_shared<WalkingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);


        if (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 0
            || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 2))
            player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0, 2);

        if (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 1
            || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 3))
            player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0, 3);

        return ptr2;
    }


        if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonLeft, player.getPlayerNr()))
        {
            std::shared_ptr<IdleState> ptr1 = std::make_shared<IdleState>();
            std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

            if ((player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 0)
                || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 1))
                player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(1, 1);

            if ((player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 2)
                || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 3))
                player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(1, 3);

            return ptr2;
        }
    


        if (InputManager::GetInstance().IsPressed(ControllerButton::ButtonRight, player.getPlayerNr()))
        {
            std::shared_ptr<IdleState> ptr1 = std::make_shared<IdleState>();
            std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

            if ((player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 0)
                || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 1))
                player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(1, 0);

            if ((player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 2)
                || (player.GetComponent<SpriteComponent>()->GetSpriteSheetTopLeft().y / player.GetRect().h == 3))
                player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(1, 2);

            return ptr2;
        }*/
    

    return nullptr;
}
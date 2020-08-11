#include "MiniginPCH.h"
#include "PlayerState.h"
#include <SDL.h>

std::shared_ptr<PlayerState> WalkingState::handleInput(Player& player, Uint8 input)
{
    UNREFERENCED_PARAMETER(player);
    if (input == SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE])
    {
        // std::shared_ptr<PlayerState> ptr2 = std::make_shared<ShootingState>();
        std::shared_ptr<ShootingState> ptr1 = std::make_shared<ShootingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        return ptr2;

        //  return nullptr;
          // Change to standing state...
          // player.setGraphics(IMAGE_STAND);
    }
    return nullptr;
}

std::shared_ptr<PlayerState> ShootingState::handleInput(Player& player, Uint8 input)
{
    UNREFERENCED_PARAMETER(player);
    if (input == SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
    { 
        // std::shared_ptr<PlayerState> ptr2 = std::make_shared<ShootingState>();
        std::shared_ptr<WalkingState> ptr1 = std::make_shared<WalkingState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);

        return ptr2;

        //  return nullptr;
          // Change to standing state...
          // player.setGraphics(IMAGE_STAND);
    }
    return nullptr;
}
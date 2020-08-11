#pragma once
#include "structs.h"
#include <SDL_hints.h>
class Player;

class PlayerState
{
public:
   // static WalkingState walking;
   // static ShootingState shooting;
	
    virtual ~PlayerState() {}
    virtual std::shared_ptr<PlayerState> handleInput(Player& player, Uint8 input)
    {
        UNREFERENCED_PARAMETER(player);
        UNREFERENCED_PARAMETER(input);
        return nullptr;
    };
    virtual void update(Player& player)
    {
        UNREFERENCED_PARAMETER(player);
    }

};

//class WalkingState : PlayerState
//{
//public:
//    WalkingState(){}
//
//   PlayerState* handleInput(Player& player, Input input) {
//        if (input == SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE])
//        {
//            return new ShootingState();
//                	
//           return nullptr;
//           // Change to standing state...
//           // player.setGraphics(IMAGE_STAND);
//        }
//    }
//
//    virtual void update(Player& player)
//	{
//
//    }
//
//};
//
//class ShootingState : PlayerState
//{
//public:
//    ShootingState() {}
//
//    std::shared_ptr<PlayerState> handleInput(Player& player, Input input) {
//        if (input == SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE])
//        {
//            player.SetState(PlayerState::shooting);
//            return nullptr;
//            // Change to standing state...
//           // player.setGraphics(IMAGE_STAND);
//        }
//    }
//
//    virtual void update(Player& player)
//    {
//
//    }
//
//};

class WalkingState : public PlayerState
{
public:
    WalkingState() {}

    virtual std::shared_ptr<PlayerState> handleInput(Player& player, Uint8 input) override;

};

class ShootingState : public PlayerState
{
public:
    ShootingState() {}

    virtual std::shared_ptr<PlayerState> handleInput(Player& player, Uint8 input) override;

};
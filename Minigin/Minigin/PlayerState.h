#pragma once
#include "structs.h"
#include <SDL_hints.h>
class Player;

class PlayerState
{
public:
	
    virtual ~PlayerState() {}
    virtual std::shared_ptr<PlayerState> handleInput(Player& player)
    {
        UNREFERENCED_PARAMETER(player);
        return nullptr;
    };
    virtual void update(Player& player)
    {
        UNREFERENCED_PARAMETER(player);
    }

};

class IdleState : public PlayerState
{
public:
    IdleState() {}

    virtual std::shared_ptr<PlayerState> handleInput(Player& player) override;

};

class ExplodeState : public PlayerState
{
public:
    ExplodeState() {}

    virtual std::shared_ptr<PlayerState> handleInput(Player& player) override;

};
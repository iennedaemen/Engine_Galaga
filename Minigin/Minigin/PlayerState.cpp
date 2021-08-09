#include "MiniginPCH.h"
#include "PlayerState.h"
#include <SDL.h>
#include "InputManager.h"
#include "SpriteComponent.h"


std::shared_ptr<PlayerState> IdleState::handleInput(Player& player)
{

    if (player.GetIsHit())
    {
        player.SetIsHit(false);
        player.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        player.GetComponent<SpriteComponent>()->IsStatic(false);
        player.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        player.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);


        std::shared_ptr<ExplodeState> ptr1 = std::make_shared<ExplodeState>();
        std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

std::shared_ptr<PlayerState> ExplodeState::handleInput(Player& player)
{
    if (player.GetComponent<SpriteComponent>()->IsAnimPlayed())
    {
        player.GetComponent<SpriteComponent>()->SetIsAnimPlayed(false);

        player.SetLives(player.GetLives() - 1);

        if(player.GetLives() <= 0)
            player.SetIsDead(true);
        else
        {
            player.GetComponent<SpriteComponent>()->SetTexture("Player1.png", 60.0f, 64.0f, 1, 1);
            player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0, 0);
            player.GetComponent<SpriteComponent>()->IsStatic(true);
            player.GetComponent<SpriteComponent>()->SetCurrentFrame(0);
            std::shared_ptr<IdleState> ptr1 = std::make_shared<IdleState>();
            std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);
            return ptr2;
        }
    }

    return nullptr;
}
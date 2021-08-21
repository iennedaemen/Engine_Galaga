#include "MiniginPCH.h"
#include "PlayerState.h"
#include <SDL.h>
#include "InputManager.h"
#include "SpriteComponent.h"
#include "GameInfo.h"

std::shared_ptr<PlayerState> IdleState::handleInput(Player& player)
{

    if (player.m_IsHit)
    {
        player.m_Exploding = true;
        player.m_IsHit = false;
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

        if (player.GetPlayerNr() == 1)
        {
           // GameInfo::GetInstance().player1Lives -= 1;
            if (GameInfo::GetInstance().player1Lives <= 0)
                player.m_IsDead = true;
        }
        else if (player.GetPlayerNr() == 2)
        {
            //GameInfo::GetInstance().player2Lives -= 1;
            if (GameInfo::GetInstance().player2Lives <= 0)
                player.m_IsDead = true;
        }

        if(!player.m_IsDead)
        {
            player.m_Exploding = false;
            player.GetComponent<SpriteComponent>()->SetTexture("Player" + std::to_string(player.GetPlayerNr()) +".png", 60.0f, 64.0f, 1, 1);
            player.GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0, 0);
            player.GetComponent<SpriteComponent>()->IsStatic(true);
            player.GetComponent<SpriteComponent>()->SetCurrentFrame(0);
            player.SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2 - player.m_Rect.w / 2), float(ScreenInfo::GetInstance().screenheigth - 75));
            std::shared_ptr<IdleState> ptr1 = std::make_shared<IdleState>();
            std::shared_ptr<PlayerState> ptr2 = std::static_pointer_cast<PlayerState>(ptr1);
            return ptr2;
        }
    }

    return nullptr;
}
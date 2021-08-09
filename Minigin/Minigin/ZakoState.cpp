#include "MiniginPCH.h"
#include "ZakoState.h"
#include "Zako.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "ScreenInfo.h"

std::shared_ptr<ZakoState> IdleStateZako::handleInput(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);

    if (zako.GetIsHit())
    {
        zako.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        zako.GetComponent<SpriteComponent>()->IsStatic(false);
        zako.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        zako.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateZako> ptr1 = std::make_shared<ExplodeStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

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

std::shared_ptr<ZakoState> SpawnStateZako::handleInput(Zako& zako)
{
    if (zako.GetIsHit())
    {
        zako.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        zako.GetComponent<SpriteComponent>()->IsStatic(false);
        zako.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        zako.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateZako> ptr1 = std::make_shared<ExplodeStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (zako.m_Rect.x == zako.GetIdlePos().x && zako.m_Rect.y == zako.GetIdlePos().y)
    {
        std::shared_ptr<IdleStateZako> ptr1 = std::make_shared<IdleStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void SpawnStateZako::update(Zako& zako)
{
    glm::vec2 newPosCoord1 = { ScreenInfo::GetInstance().screenwidth/2 - zako.m_Rect.w/2, ScreenInfo::GetInstance().screenheigth / 2};

    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    if (!m_ReachedPosX1 || !m_ReachedPosY1)
    {
        if (zako.SpawnedLeft())
        {
            if (zako.m_Rect.x < newPosCoord1.x)
                zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosX1 = true;
        }
        else
        {
            if (zako.m_Rect.x > newPosCoord1.x)
                zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosX1 = true;
        }
        if (zako.m_Rect.y > newPosCoord1.y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y - velocity);
        else m_ReachedPosY1 = true;
    }

    if (m_ReachedPosX1 && m_ReachedPosY1)
    {
        if (zako.GetIdlePos().x < ScreenInfo::GetInstance().screenwidth / 2)
        {
            if (zako.m_Rect.x > zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
        }
        else
        {
            if (zako.m_Rect.x < zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
        }
        if (zako.m_Rect.y > zako.GetIdlePos().y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y - velocity);
    }

}


std::shared_ptr<ZakoState> ExplodeStateZako::handleInput(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);
    
    if (zako.GetComponent<SpriteComponent>()->IsAnimPlayed())
    {
        zako.SetIsDead(true);
    }

    return nullptr;
}

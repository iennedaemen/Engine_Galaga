#include "MiniginPCH.h"
#include "ZakoState.h"
#include "Zako.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "ScreenInfo.h"
#include "GameInfo.h"

std::shared_ptr<ZakoState> IdleStateZako::handleInput(Zako& zako)
{
    if (zako.m_IsHit)
    {
        m_EnumState = State::Dead;
        zako.SetIsIdle(false);
        zako.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        zako.GetComponent<SpriteComponent>()->IsStatic(false);
        zako.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        zako.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateZako> ptr1 = std::make_shared<ExplodeStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (zako.m_DoShootRun)
    {
        m_EnumState = State::Attacking;
        zako.SetIsIdle(false);
        std::shared_ptr<ShootingRunStateZako> ptr1 = std::make_shared<ShootingRunStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (zako.DoCrashRun())
    {
        m_EnumState = State::Attacking;
        zako.SetIsIdle(false);
        std::shared_ptr<CrashRunStateZako> ptr1 = std::make_shared<CrashRunStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }


    return nullptr;
}

std::shared_ptr<ZakoState> SpawnStateZako::handleInput(Zako& zako)
{
    if (zako.m_IsHit)
    {
        m_EnumState = State::Dead;
        zako.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        zako.GetComponent<SpriteComponent>()->IsStatic(false);
        zako.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        zako.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateZako> ptr1 = std::make_shared<ExplodeStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        m_EnumState = State::Idle;
        zako.SetIsIdle(true);
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
        if (zako.m_SpawnedLeft)
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
            else m_ReachedPosXIdle = true;
        }
        else
        {
            if (zako.m_Rect.x < zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        if (zako.m_Rect.y > zako.GetIdlePos().y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }

}

std::shared_ptr<ZakoState> ShootingRunStateZako::handleInput(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);

    if (zako.m_IsHit)
    {
        m_EnumState = State::Dead;
        zako.SetIsIdle(false);
        zako.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        zako.GetComponent<SpriteComponent>()->IsStatic(false);
        zako.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        zako.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateZako> ptr1 = std::make_shared<ExplodeStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        m_EnumState = State::Idle;
        zako.m_DoShootRun = false;
        zako.SetIsIdle(true);
        std::shared_ptr<IdleStateZako> ptr1 = std::make_shared<IdleStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void ShootingRunStateZako::update(Zako& zako)
{
    glm::vec2 newPosCoord1 = { zako.m_Rect.x , ScreenInfo::GetInstance().screenheigth - 275 };
    glm::vec2 newPosCoord2;
    glm::vec2 newPosCoord3;

    if (zako.m_SpawnedLeft)
    {
        newPosCoord2 = { 50, ScreenInfo::GetInstance().screenheigth - 175 };
        newPosCoord3 = { ScreenInfo::GetInstance().screenwidth - 50, zako.m_Rect.y };
    }
    else 
    {
        newPosCoord2 = { ScreenInfo::GetInstance().screenwidth - 50, ScreenInfo::GetInstance().screenheigth - 175 };
        newPosCoord3 = { 50, zako.m_Rect.y };
    }


    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    // POSITION 1
    if (!m_ReachedPos1)
    {
        if (zako.m_Rect.y < newPosCoord1.y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y + velocity);
        else m_ReachedPos1 = true;
    }

    // POSITION 2
    if (m_ReachedPos1)
    {
        if (!m_ReachedPosX2 || !m_ReachedPosY2)
        {
            if (zako.m_SpawnedLeft)
            {
                if (zako.m_Rect.x > newPosCoord2.x)
                    zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
                else m_ReachedPosX2 = true;
            }
            else
            {
                if (zako.m_Rect.x < newPosCoord2.x)
                    zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
                else m_ReachedPosX2 = true;
            }
            if (zako.m_Rect.y < newPosCoord2.y)
                zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y + velocity);
            else m_ReachedPosY2 = true;
        }
    }

    // POSITION 3
    if (m_ReachedPosX2 && m_ReachedPosY2)
    {
        if (!m_ReachedPos3)
        {
            if (zako.m_SpawnedLeft)
            {
                if (zako.m_Rect.x < newPosCoord3.x)
                    zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
                else
                {
                    m_ReachedPos3 = true;
                    m_LastPosX = zako.GetTransform().GetPosition().x;
                }
            }
            else
            {
                if (zako.m_Rect.x > newPosCoord3.x)
                    zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
                else
                {
                    m_ReachedPos3 = true;
                    m_LastPosX = zako.GetTransform().GetPosition().x;
                }
            }
        }
    }

    // POSITION IDLE
    if (m_ReachedPos3)
    {
        if (zako.GetIdlePos().x < m_LastPosX)
        {
            if (zako.m_Rect.x > zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        else
        {
            if (zako.m_Rect.x < zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        if (zako.m_Rect.y > zako.GetIdlePos().y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }

    //SHOOT
    if (zako.m_PlayerPos.x > zako.m_Rect.x + 5
        && zako.m_PlayerPos.x - 5 < zako.m_Rect.x + zako.m_Rect.w / 2)
    {
        zako.ShootLaser(std::make_shared<Zako>(zako));
    }

    if (GameInfo::GetInstance().player2Active)
    {
        if (zako.m_Player2Pos.x > zako.m_Rect.x + 5
            && zako.m_Player2Pos.x - 5 < zako.m_Rect.x + zako.m_Rect.w / 2)
        {
            zako.ShootLaser(std::make_shared<Zako>(zako));
        }
    }
}

std::shared_ptr<ZakoState> CrashRunStateZako::handleInput(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);

    if (zako.m_IsHit)
    {
        m_EnumState = State::Dead;
        zako.SetIsIdle(false);
        zako.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        zako.GetComponent<SpriteComponent>()->IsStatic(false);
        zako.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        zako.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateZako> ptr1 = std::make_shared<ExplodeStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        m_EnumState = State::Idle;
        zako.SetCrashRun(false);
        zako.SetIsIdle(true);
        std::shared_ptr<IdleStateZako> ptr1 = std::make_shared<IdleStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void CrashRunStateZako::update(Zako& zako)
{
    glm::vec2 newPosCoord1 = { 0, 0 };
    if (GameInfo::GetInstance().player2Active)
    {
        int r = std::rand() % 2;
        if( r == 0)
            newPosCoord1 = { zako.m_PlayerPos.x , ScreenInfo::GetInstance().screenheigth - 275 };
        else newPosCoord1 = { zako.m_Player2Pos.x , ScreenInfo::GetInstance().screenheigth - 275 };
    }
    else newPosCoord1 = { zako.m_PlayerPos.x , ScreenInfo::GetInstance().screenheigth - 275 };

    glm::vec2 newPosCoord2 = { zako.m_Rect.x , ScreenInfo::GetInstance().screenheigth - 10};
 
    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    // POSITION 1
    if (!m_ReachedPosX1 || !m_ReachedPosY1)
    {

        if (zako.m_Rect.x > newPosCoord1.x + 10)
            zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
        else if(zako.m_Rect.x < newPosCoord2.x - 10)
            zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
        else
        {
            m_LastPosX = zako.GetTransform().GetPosition().x;
            m_ReachedPosX1 = true;
        }

        if (zako.m_Rect.y < newPosCoord2.y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y + velocity);
        else m_ReachedPosY1 = true;
    }

    // POSITION 2
    if (m_ReachedPosX1 && m_ReachedPosY1)
    {
        if (!m_ReachedPos2)
        {
            if (zako.m_Rect.y < newPosCoord2.y)
                zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y + velocity);
            else m_ReachedPos2 = true;
        }
    }

    // POSITION IDLE
    if (m_ReachedPos2)
    {
        if (zako.GetIdlePos().x < m_LastPosX)
        {
            if (zako.m_Rect.x > zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x - velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        else
        {
            if (zako.m_Rect.x < zako.GetIdlePos().x)
                zako.SetPosition(zako.GetTransform().GetPosition().x + velocity, zako.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        if (zako.m_Rect.y > zako.GetIdlePos().y)
            zako.SetPosition(zako.GetTransform().GetPosition().x, zako.GetTransform().GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }
}

std::shared_ptr<ZakoState> ExplodeStateZako::handleInput(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);
    
    if (zako.GetComponent<SpriteComponent>()->IsAnimPlayed())
    {
        zako.m_IsDead = true;
    }

    return nullptr;
}

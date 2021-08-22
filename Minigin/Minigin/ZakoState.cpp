#include "MiniginPCH.h"
#include "ZakoState.h"
#include "Zako.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "ScreenInfo.h"
#include "GameInfo.h"

std::shared_ptr<ZakoState> IdleStateZako::HandleState(Zako& zako)
{
    if (zako.m_IsHit)
    {
        zako.m_EnumState = State::Dead;
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
        zako.m_EnumState = State::Moving;
        std::shared_ptr<ShootingRunStateZako> ptr1 = std::make_shared<ShootingRunStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    if (zako.m_DoCrashRun)
    {
        zako.m_EnumState = State::Moving;
        std::shared_ptr<CrashRunStateZako> ptr1 = std::make_shared<CrashRunStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }


    return nullptr;
}

std::shared_ptr<ZakoState> SpawnStateZako::HandleState(Zako& zako)
{
    if (zako.m_IsHit)
    {
        zako.m_EnumState = State::Dead;
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
        zako.m_EnumState = State::Idle;
        std::shared_ptr<IdleStateZako> ptr1 = std::make_shared<IdleStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void SpawnStateZako::Update(Zako& zako)
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
                zako.SetPosition(zako.GetTransform()->GetPosition().x + velocity, zako.GetTransform()->GetPosition().y);
            else m_ReachedPosX1 = true;
        }
        else
        {
            if (zako.m_Rect.x > newPosCoord1.x)
                zako.SetPosition(zako.GetTransform()->GetPosition().x - velocity, zako.GetTransform()->GetPosition().y);
            else m_ReachedPosX1 = true;
        }
        if (zako.m_Rect.y > newPosCoord1.y)
            zako.SetPosition(zako.GetTransform()->GetPosition().x, zako.GetTransform()->GetPosition().y - velocity);
        else m_ReachedPosY1 = true;
    }

    if (m_ReachedPosX1 && m_ReachedPosY1)
    {
		if (zako.m_Rect.x > zako.GetIdlePos().x + 2)
			zako.SetPosition(zako.GetTransform()->GetPosition().x - velocity, zako.GetTransform()->GetPosition().y);
		else if (zako.m_Rect.x < zako.GetIdlePos().x - 2)
			zako.SetPosition(zako.GetTransform()->GetPosition().x + velocity, zako.GetTransform()->GetPosition().y);
		else m_ReachedPosXIdle = true;
        
        if (zako.m_Rect.y > zako.GetIdlePos().y)
            zako.SetPosition(zako.GetTransform()->GetPosition().x, zako.GetTransform()->GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }

}

std::shared_ptr<ZakoState> ShootingRunStateZako::HandleState(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);

    if (zako.m_IsHit)
    {
        zako.m_EnumState = State::Dead;
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
        zako.m_EnumState = State::Idle;
        zako.m_DoShootRun = false;
        std::shared_ptr<IdleStateZako> ptr1 = std::make_shared<IdleStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void ShootingRunStateZako::Update(Zako& zako)
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


    int speed = 150;
    float velocity = speed * Time::GetInstance().m_ElapsedSec;

    // POSITION 1
    if (!m_ReachedPosX1 || !m_ReachedPosY1)
    {
        GoToPosition(zako, newPosCoord1, velocity, m_ReachedPosX1, m_ReachedPosY1, false, true);
    }

    // POSITION 1 REACHED -> GO POSITION 2
    else if(!m_ReachedPosX2 || !m_ReachedPosY2)
    {
            GoToPosition(zako, newPosCoord2, velocity, m_ReachedPosX2, m_ReachedPosY2);        
    }

    // POSITION 2 REACHED -> GO POSITION 3
    else if (!m_ReachedPosX3 || !m_ReachedPosY3)
    {
            GoToPosition(zako, newPosCoord3, velocity, m_ReachedPosX3, m_ReachedPosY3, true, false);
    }

    // POSITION 3 REACHED -> GO POSITION IDLE
    else
    {
        GoToPosition(zako, zako.GetIdlePos(), velocity, m_ReachedPosXIdle, m_ReachedPosYIdle);
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

std::shared_ptr<ZakoState> CrashRunStateZako::HandleState(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);

    if (zako.m_IsHit)
    {
        zako.m_EnumState = State::Dead;
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
        zako.m_EnumState = State::Idle;
        zako.m_DoCrashRun = false;
        std::shared_ptr<IdleStateZako> ptr1 = std::make_shared<IdleStateZako>();
        std::shared_ptr<ZakoState> ptr2 = std::static_pointer_cast<ZakoState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void CrashRunStateZako::Update(Zako& zako)
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
    float velocity = speed * elapsedSec;

    // POSITION 1
    if (!m_ReachedPosX1 || !m_ReachedPosY1)
    {
        GoToPosition(zako, newPosCoord1, velocity, m_ReachedPosX1, m_ReachedPosY1);
    }

    // POSITION 2
    if (m_ReachedPosX1 && m_ReachedPosY1)
    {
        if (!m_ReachedPosX2 || !m_ReachedPosY2)
        {
            GoToPosition(zako, newPosCoord2, velocity, m_ReachedPosX2, m_ReachedPosY2, false, true);
        }
    }

    // POSITION IDLE
	if (m_ReachedPosX2 && m_ReachedPosY2)
	{
        GoToPosition(zako, zako.GetIdlePos(), velocity, m_ReachedPosXIdle, m_ReachedPosYIdle);
    }
}

std::shared_ptr<ZakoState> ExplodeStateZako::HandleState(Zako& zako)
{
    UNREFERENCED_PARAMETER(zako);
    
    if (zako.GetComponent<SpriteComponent>()->IsAnimPlayed())
    {
        zako.m_IsDead = true;
    }

    return nullptr;
}

void ZakoState::GoToPosition(Zako& zako, glm::vec2 newPos, float velocity, bool& reachedX, bool& reachedY, bool xFirst, bool yFirst)
{
    bool doX = true;
    bool doY = true;
    if (yFirst)
    {
        if (!reachedY)
            doX = false;
    }
    else if (xFirst)
    {
        if (!reachedX)
            doY = false;
    }

    if (doX)
    {
        if (zako.GetTransform()->GetPosition().x > newPos.x + 2.0f)
            zako.SetPosition(zako.GetTransform()->GetPosition().x - velocity, zako.GetTransform()->GetPosition().y);
        else if (zako.GetTransform()->GetPosition().x < newPos.x - 2.0f)
            zako.SetPosition(zako.GetTransform()->GetPosition().x + velocity, zako.GetTransform()->GetPosition().y);
        else reachedX = true;
    }

    if (doY)
    {
        if (zako.GetTransform()->GetPosition().y < newPos.y - 2.0f)
            zako.SetPosition(zako.GetTransform()->GetPosition().x, zako.GetTransform()->GetPosition().y + velocity);
        else if (zako.GetTransform()->GetPosition().y > newPos.y + 2.0f)
            zako.SetPosition(zako.GetTransform()->GetPosition().x, zako.GetTransform()->GetPosition().y - velocity);
        else reachedY = true;
    }
}
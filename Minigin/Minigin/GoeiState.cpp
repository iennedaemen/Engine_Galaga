#include "MiniginPCH.h"
#include "GoeiState.h"
#include "SpriteComponent.h"
#include "Goei.h"
#include "ScreenInfo.h"
#include "Time.h"
#include "GameInfo.h"

std::shared_ptr<GoeiState> IdleStateGoei::HandleState(Goei& goei)
{
    if (goei.m_IsHit)
    {
        goei.m_EnumState = State::Dead;
        goei.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        goei.GetComponent<SpriteComponent>()->IsStatic(false);
        goei.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        goei.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateGoei> ptr1 = std::make_shared<ExplodeStateGoei>();
        std::shared_ptr<GoeiState> ptr2 = std::static_pointer_cast<GoeiState>(ptr1);
        return ptr2;
    }

    if (goei.m_DoShootRun)
    {
        goei.m_EnumState = State::Moving;
        std::shared_ptr<ShootingRunStateGoei> ptr1 = std::make_shared<ShootingRunStateGoei>();
        std::shared_ptr<GoeiState> ptr2 = std::static_pointer_cast<GoeiState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

std::shared_ptr<GoeiState> SpawnStateGoei::HandleState(Goei& goei)
{
    if (goei.m_IsHit)
    {
        goei.m_EnumState = State::Dead;
        goei.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        goei.GetComponent<SpriteComponent>()->IsStatic(false);
        goei.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        goei.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateGoei> ptr1 = std::make_shared<ExplodeStateGoei>();
        std::shared_ptr<GoeiState> ptr2 = std::static_pointer_cast<GoeiState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        goei.m_EnumState = State::Idle;
        std::shared_ptr<IdleStateGoei> ptr1 = std::make_shared<IdleStateGoei>();
        std::shared_ptr<GoeiState> ptr2 = std::static_pointer_cast<GoeiState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void SpawnStateGoei::Update(Goei& goei)
{
    glm::vec2 newPosCoord1 = { ScreenInfo::GetInstance().screenwidth / 2 - goei.m_Rect.w / 2, ScreenInfo::GetInstance().screenheigth / 2 };

    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    if (!m_ReachedPosX1 || !m_ReachedPosY1)
    {
        if (goei.m_SpawnedLeft)
        {
            if (goei.m_Rect.x < newPosCoord1.x)
                goei.SetPosition(goei.GetTransform()->GetPosition().x + velocity, goei.GetTransform()->GetPosition().y);
            else m_ReachedPosX1 = true;
        }
        else
        {
            if (goei.m_Rect.x > newPosCoord1.x)
                goei.SetPosition(goei.GetTransform()->GetPosition().x - velocity, goei.GetTransform()->GetPosition().y);
            else m_ReachedPosX1 = true;
        }
        if (goei.m_Rect.y > newPosCoord1.y)
            goei.SetPosition(goei.GetTransform()->GetPosition().x, goei.GetTransform()->GetPosition().y - velocity);
        else m_ReachedPosY1 = true;
    }

    if (m_ReachedPosX1 && m_ReachedPosY1)
    {

		if (goei.m_Rect.x > goei.GetIdlePos().x + 2)
			goei.SetPosition(goei.GetTransform()->GetPosition().x - velocity, goei.GetTransform()->GetPosition().y);
		else if (goei.m_Rect.x < goei.GetIdlePos().x - 2)
			goei.SetPosition(goei.GetTransform()->GetPosition().x + velocity, goei.GetTransform()->GetPosition().y);
		else m_ReachedPosXIdle = true;
        
        if (goei.m_Rect.y > goei.GetIdlePos().y)
            goei.SetPosition(goei.GetTransform()->GetPosition().x, goei.GetTransform()->GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }

}

std::shared_ptr<GoeiState> ShootingRunStateGoei::HandleState(Goei& goei)
{
    if (goei.m_IsHit)
    {
        goei.m_EnumState = State::Dead;
        goei.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        goei.GetComponent<SpriteComponent>()->IsStatic(false);
        goei.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        goei.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateGoei> ptr1 = std::make_shared<ExplodeStateGoei>();
        std::shared_ptr<GoeiState> ptr2 = std::static_pointer_cast<GoeiState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        goei.m_EnumState = State::Idle;
        goei.m_DoShootRun = false;
        std::shared_ptr<IdleStateGoei> ptr1 = std::make_shared<IdleStateGoei>();
        std::shared_ptr<GoeiState> ptr2 = std::static_pointer_cast<GoeiState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void ShootingRunStateGoei::Update(Goei& goei)
{
    float maxDivePosY = ScreenInfo::GetInstance().screenheigth - 100.0f;
    float newPosCoordLeft = 50.0f;
    float newPosCoordRight = ScreenInfo::GetInstance().screenwidth - 50.0f;


    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    // POSITIONS
    if (!m_ReachedPosY)
    {
        if (goei.m_Rect.y < maxDivePosY)
            goei.SetPosition(goei.GetTransform()->GetPosition().x, goei.GetTransform()->GetPosition().y + velocity);
        else m_ReachedPosY = true;
    }

    if (!m_ReachedPosYIdle)
    {
        if (goei.m_SpawnedLeft)
        {
            if (!m_ReachedPos1 && m_ReachedPos2)
            {
                if (goei.m_Rect.x > newPosCoordLeft)
                    goei.SetPosition(goei.GetTransform()->GetPosition().x - velocity, goei.GetTransform()->GetPosition().y);
                else
                {
                    m_ReachedPos1 = true;
                    m_ReachedPos2 = false;
                }
            }
            if (m_ReachedPos1 && !m_ReachedPos2)
            {
                if (goei.m_Rect.x < newPosCoordRight)
                    goei.SetPosition(goei.GetTransform()->GetPosition().x + velocity, goei.GetTransform()->GetPosition().y);
                else
                {
                    m_ReachedPos1 = false;
                    m_ReachedPos2 = true;
                }
            }
        }

        if (!goei.m_SpawnedLeft)
        {
            if (!m_ReachedPos1 && m_ReachedPos2)
            {
                if (goei.m_Rect.x < newPosCoordRight)
                    goei.SetPosition(goei.GetTransform()->GetPosition().x + velocity, goei.GetTransform()->GetPosition().y);
                else
                {
                    m_ReachedPos1 = true;
                    m_ReachedPos2 = false;
                }
            }
            if (m_ReachedPos1 && !m_ReachedPos2)
            {
                if (goei.m_Rect.x > newPosCoordLeft)
                    goei.SetPosition(goei.GetTransform()->GetPosition().x - velocity, goei.GetTransform()->GetPosition().y);
                else
                {
                    m_ReachedPos1 = false;
                    m_ReachedPos2 = true;
                }
            }
        }
    }

    // POSITION IDLE
    if (m_ReachedPosY && (!m_ReachedPosYIdle || !m_ReachedPosXIdle))
    {
        GoToPosition(goei, goei.GetIdlePos(), velocity, m_ReachedPosXIdle, m_ReachedPosYIdle, false, true);
    }


    //SHOOT
    if (goei.m_PlayerPos.x > goei.m_Rect.x + 5
        && goei.m_PlayerPos.x - 5 < goei.m_Rect.x + goei.m_Rect.w / 2)
    {
        goei.ShootLaser(std::make_shared<Goei>(goei));
    }

    if (GameInfo::GetInstance().player2Active)
    {
        if (goei.m_Player2Pos.x > goei.m_Rect.x + 5
            && goei.m_Player2Pos.x - 5 < goei.m_Rect.x + goei.m_Rect.w / 2)
        {
            goei.ShootLaser(std::make_shared<Goei>(goei));
        }
    }
}

std::shared_ptr<GoeiState> ExplodeStateGoei::HandleState(Goei& goei)
{
    if (goei.GetComponent<SpriteComponent>()->IsAnimPlayed())
    {
        goei.m_IsDead = true;
    }

    return nullptr;
}

void GoeiState::GoToPosition(Goei& goei, glm::vec2 newPos, float velocity, bool& reachedX, bool& reachedY, bool xFirst, bool yFirst)
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
        if (goei.m_Rect.x > newPos.x + 2)
            goei.SetPosition(goei.GetTransform()->GetPosition().x - velocity, goei.GetTransform()->GetPosition().y);
        else if (goei.m_Rect.x < newPos.x - 2)
            goei.SetPosition(goei.GetTransform()->GetPosition().x + velocity, goei.GetTransform()->GetPosition().y);
        else reachedX = true;
    }

    if (doY)
    {
        if (goei.m_Rect.y < newPos.y - 2)
            goei.SetPosition(goei.GetTransform()->GetPosition().x, goei.GetTransform()->GetPosition().y + velocity);
        else if (goei.m_Rect.y > newPos.y + 2)
            goei.SetPosition(goei.GetTransform()->GetPosition().x, goei.GetTransform()->GetPosition().y - velocity);
        else reachedY = true;
    }
}
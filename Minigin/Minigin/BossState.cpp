#include "MiniginPCH.h"
#include "BossState.h"
#include "Boss.h"
#include "SpriteComponent.h"
#include "ScreenInfo.h"
#include "Time.h"
#include "Beam.h"
#include "GameInfo.h"

std::shared_ptr<BossState> IdleStateBoss::handleInput(Boss& boss)
{
    if (boss.GetLives() <= 0)
    {
        boss.SetIsIdle(false);
        boss.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        boss.GetComponent<SpriteComponent>()->IsStatic(false);
        boss.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        boss.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);
        boss.GetComponent<SpriteComponent>()->SetCurrentFrame(0);

        std::shared_ptr<ExplodeStateBoss> ptr1 = std::make_shared<ExplodeStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (boss.DoShootRun())
    {
        boss.SetIsIdle(false);
        std::shared_ptr<ShootingRunStateBoss> ptr1 = std::make_shared<ShootingRunStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (boss.DoBeamRun())
    {
        boss.SetIsIdle(false);
        std::shared_ptr<BeamRunStateBoss> ptr1 = std::make_shared<BeamRunStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

std::shared_ptr<BossState> SpawnStateBoss::handleInput(Boss& boss)
{
    if (boss.GetLives() <= 0)
    {
        boss.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        boss.GetComponent<SpriteComponent>()->IsStatic(false);
        boss.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        boss.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateBoss> ptr1 = std::make_shared<ExplodeStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        boss.SetIsIdle(true);
        std::shared_ptr<IdleStateBoss> ptr1 = std::make_shared<IdleStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void SpawnStateBoss::update(Boss& boss)
{
    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

        if (boss.GetIdlePos().x < ScreenInfo::GetInstance().screenwidth / 2)
        {
            if (boss.m_Rect.x > boss.GetIdlePos().x)
                boss.SetPosition(boss.GetTransform().GetPosition().x - velocity, boss.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        else
        {
            if (boss.m_Rect.x < boss.GetIdlePos().x)
                boss.SetPosition(boss.GetTransform().GetPosition().x + velocity, boss.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        if (boss.m_Rect.y < boss.GetIdlePos().y)
            boss.SetPosition(boss.GetTransform().GetPosition().x, boss.GetTransform().GetPosition().y + velocity);
        else m_ReachedPosYIdle = true;
  

}

std::shared_ptr<BossState> ShootingRunStateBoss::handleInput(Boss& boss)
{
    if (boss.GetLives() <= 0)
    {
        boss.SetIsIdle(false);
        boss.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        boss.GetComponent<SpriteComponent>()->IsStatic(false);
        boss.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        boss.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateBoss> ptr1 = std::make_shared<ExplodeStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosXIdle && m_ReachedPosYIdle)
    {
        boss.SetShootRun(false);
        boss.SetIsIdle(true);
        std::shared_ptr<IdleStateBoss> ptr1 = std::make_shared<IdleStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void ShootingRunStateBoss::update(Boss& boss)
{
    glm::vec2 newPosCoord1 = { boss.m_Rect.x , ScreenInfo::GetInstance().screenheigth - 275 };
    glm::vec2 newPosCoord2;
    glm::vec2 newPosCoord3;

    if (boss.SpawnedLeft())
    {
        newPosCoord2 = { 50, ScreenInfo::GetInstance().screenheigth - 175 };
        newPosCoord3 = { ScreenInfo::GetInstance().screenwidth - 50, boss.m_Rect.y };
    }
    else
    {
        newPosCoord2 = { ScreenInfo::GetInstance().screenwidth - 50, ScreenInfo::GetInstance().screenheigth - 175 };
        newPosCoord3 = { 50, boss.m_Rect.y };
    }


    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    // POSITION 1
    if (!m_ReachedPos1)
    {
        if (boss.m_Rect.y < newPosCoord1.y)
            boss.SetPosition(boss.GetTransform().GetPosition().x, boss.GetTransform().GetPosition().y + velocity);
        else m_ReachedPos1 = true;
    }

    // POSITION 2
    if (m_ReachedPos1)
    {
        if (!m_ReachedPosX2 || !m_ReachedPosY2)
        {
            if (boss.SpawnedLeft())
            {
                if (boss.m_Rect.x > newPosCoord2.x)
                    boss.SetPosition(boss.GetTransform().GetPosition().x - velocity, boss.GetTransform().GetPosition().y);
                else m_ReachedPosX2 = true;
            }
            else
            {
                if (boss.m_Rect.x < newPosCoord2.x)
                    boss.SetPosition(boss.GetTransform().GetPosition().x + velocity, boss.GetTransform().GetPosition().y);
                else m_ReachedPosX2 = true;
            }
            if (boss.m_Rect.y < newPosCoord2.y)
                boss.SetPosition(boss.GetTransform().GetPosition().x, boss.GetTransform().GetPosition().y + velocity);
            else m_ReachedPosY2 = true;
        }
    }

    // POSITION 3
    if (m_ReachedPosX2 && m_ReachedPosY2)
    {
        if (!m_ReachedPos3)
        {
            if (boss.SpawnedLeft())
            {
                if (boss.m_Rect.x < newPosCoord3.x)
                    boss.SetPosition(boss.GetTransform().GetPosition().x + velocity, boss.GetTransform().GetPosition().y);
                else
                {
                    m_ReachedPos3 = true;
                    m_LastPosX = boss.GetTransform().GetPosition().x;
                }
            }
            else
            {
                if (boss.m_Rect.x > newPosCoord3.x)
                    boss.SetPosition(boss.GetTransform().GetPosition().x - velocity, boss.GetTransform().GetPosition().y);
                else
                {
                    m_ReachedPos3 = true;
                    m_LastPosX = boss.GetTransform().GetPosition().x;
                }
            }
        }
    }

    // POSITION IDLE
    if (m_ReachedPos3)
    {
        if (boss.GetIdlePos().x < m_LastPosX)
        {
            if (boss.m_Rect.x > boss.GetIdlePos().x)
                boss.SetPosition(boss.GetTransform().GetPosition().x - velocity, boss.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        else
        {
            if (boss.m_Rect.x < boss.GetIdlePos().x)
                boss.SetPosition(boss.GetTransform().GetPosition().x + velocity, boss.GetTransform().GetPosition().y);
            else m_ReachedPosXIdle = true;
        }
        if (boss.m_Rect.y > boss.GetIdlePos().y)
            boss.SetPosition(boss.GetTransform().GetPosition().x, boss.GetTransform().GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }

    //SHOOT
    if (boss.GetPlayerPos().x > boss.m_Rect.x + 5
        && boss.GetPlayerPos().x - 5 < boss.m_Rect.x + boss.m_Rect.w / 2)
    {
        boss.ShootLaser();
    }

    if (GameInfo::GetInstance().player2Active)
    {
        if (boss.GetPlayer2Pos().x > boss.m_Rect.x + 5
            && boss.GetPlayer2Pos().x - 5 < boss.m_Rect.x + boss.m_Rect.w / 2)
        {
            boss.ShootLaser();
        }
    }
}

std::shared_ptr<BossState> BeamRunStateBoss::handleInput(Boss& boss)
{
    if (boss.GetLives() <= 0)
    {
        boss.SetIsIdle(false);
        boss.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        boss.GetComponent<SpriteComponent>()->IsStatic(false);
        boss.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        boss.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateBoss> ptr1 = std::make_shared<ExplodeStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosYIdle)
    {
        boss.SetBeamRun(false);
        boss.SetIsIdle(true);
        std::shared_ptr<IdleStateBoss> ptr1 = std::make_shared<IdleStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void BeamRunStateBoss::update(Boss& boss)
{
    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    // POSITION 1
    if (!m_ReachedPos)
    {
        if (boss.m_Rect.y < ScreenInfo::GetInstance().screenheigth/2 + 50)
            boss.SetPosition(boss.GetTransform().GetPosition().x, boss.GetTransform().GetPosition().y + velocity);
        else m_ReachedPos = true;
    }

    // POSITION IDLE
    if (m_BeamDone)
    {
        if (boss.m_Rect.y > boss.GetIdlePos().y)
            boss.SetPosition(boss.GetTransform().GetPosition().x, boss.GetTransform().GetPosition().y - velocity);
        else m_ReachedPosYIdle = true;
    }

    //BEAM
    if (m_ReachedPos && !m_BeamDone)
    {
        std::shared_ptr<Beam> dBeam = std::dynamic_pointer_cast<Beam> (boss.GetBeam());
        dBeam->SetActive(true);
        m_BeamTimer += elapsedSec;

        if (m_BeamTimer >= m_BeamTime)
        {
            m_BeamTimer = 0.0f;
            dBeam->SetActive(false);
            m_BeamDone = true;
        }

    }
}

std::shared_ptr<BossState> ExplodeStateBoss::handleInput(Boss& boss)
{
        if (boss.GetComponent<SpriteComponent>()->IsAnimPlayed())
        {
            boss.SetIsDead(true);
        }

    return nullptr;
}
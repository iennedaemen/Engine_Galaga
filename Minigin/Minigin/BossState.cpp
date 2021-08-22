#include "MiniginPCH.h"
#include "BossState.h"
#include "Boss.h"
#include "SpriteComponent.h"
#include "ScreenInfo.h"
#include "Time.h"
#include "Beam.h"
#include "GameInfo.h"
#include "Player.h"

std::shared_ptr<BossState> IdleStateBoss::handleInput(Boss& boss)
{
    if (boss.m_Lives <= 0)
    {
        boss.m_EnumState = State::Dead;
        boss.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        boss.GetComponent<SpriteComponent>()->IsStatic(false);
        boss.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        boss.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);
        boss.GetComponent<SpriteComponent>()->SetCurrentFrame(0);

        std::shared_ptr<ExplodeStateBoss> ptr1 = std::make_shared<ExplodeStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (boss.m_DoShootRun)
    {
        boss.m_EnumState = State::Moving;
        std::shared_ptr<ShootingRunStateBoss> ptr1 = std::make_shared<ShootingRunStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (boss.m_DoBeamRun)
    {
        boss.m_EnumState = State::Moving;
        std::shared_ptr<BeamRunStateBoss> ptr1 = std::make_shared<BeamRunStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

std::shared_ptr<BossState> SpawnStateBoss::handleInput(Boss& boss)
{
    if (boss.m_Lives <= 0)
    {
        boss.m_EnumState = State::Dead;
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
        boss.m_EnumState = State::Idle;
        std::shared_ptr<IdleStateBoss> ptr1 = std::make_shared<IdleStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void SpawnStateBoss::update(Boss& boss)
{
    int speed = 150;
    float velocity{};
    velocity = speed * Time::GetInstance().m_ElapsedSec;

    GoToPosition(boss, boss.GetIdlePos(), velocity, m_ReachedPosXIdle, m_ReachedPosYIdle, false, true);
}

std::shared_ptr<BossState> ShootingRunStateBoss::handleInput(Boss& boss)
{
    if (boss.m_Lives <= 0)
    {
        boss.m_EnumState = State::Dead;
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
        boss.m_EnumState = State::Idle;
        boss.m_DoShootRun = false;
        std::shared_ptr<IdleStateBoss> ptr1 = std::make_shared<IdleStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void ShootingRunStateBoss::update(Boss& boss)
{
    glm::vec2 newPosCoord1 = { boss.GetTransform()->GetPosition().x , ScreenInfo::GetInstance().screenheigth - 275 };
    glm::vec2 newPosCoord2;
    glm::vec2 newPosCoord3;

    if (boss.m_SpawnedLeft)
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
    if (!m_ReachedPosX1 || !m_ReachedPosY1)
    {
        GoToPosition(boss, newPosCoord1, velocity, m_ReachedPosX1, m_ReachedPosY1, true, false);
    }

    // POSITION 1 REACHED -> GO POSITION 2
	else if (!m_ReachedPosX2 || !m_ReachedPosY2)
	{
		GoToPosition(boss, newPosCoord2, velocity, m_ReachedPosX2, m_ReachedPosY2);
	}
    
    // POSITION 2 REACHED -> GO POSITION 3
	else if (!m_ReachedPosX3 || !m_ReachedPosY3)
	{
		GoToPosition(boss, newPosCoord3, velocity, m_ReachedPosX3, m_ReachedPosY3, true, false);
	}

    // POSITION 3 REACHED -> GO POSITION IDLE
    else
	{
        GoToPosition(boss, boss.GetIdlePos(), velocity, m_ReachedPosXIdle, m_ReachedPosYIdle);
	}

    //SHOOT
    if (boss.m_PlayerPos.x > boss.m_Rect.x + 5
        && boss.m_PlayerPos.x - 5 < boss.m_Rect.x + boss.m_Rect.w / 2)
    {
        boss.ShootLaser(std::make_shared<Boss>(boss));
    }

    if (GameInfo::GetInstance().player2Active)
    {
        if (boss.m_Player2Pos.x > boss.m_Rect.x + 5
            && boss.m_Player2Pos.x - 5 < boss.m_Rect.x + boss.m_Rect.w / 2)
        {
            boss.ShootLaser(std::make_shared<Boss>(boss));
        }
    }
}

std::shared_ptr<BossState> BeamRunStateBoss::handleInput(Boss& boss)
{
    if (boss.m_Lives <= 0)
    {
        boss.m_EnumState = State::Dead;
        boss.GetComponent<SpriteComponent>()->SetTexture("Explosion.png", 180, 36, 5, 1);
        boss.GetComponent<SpriteComponent>()->IsStatic(false);
        boss.GetComponent<SpriteComponent>()->SetNrFramesToPlay(5);
        boss.GetComponent<SpriteComponent>()->SetPlayAnimOnce(true);

        std::shared_ptr<ExplodeStateBoss> ptr1 = std::make_shared<ExplodeStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    if (m_ReachedPosYIdle && m_ReachedPosXIdle)
    {
        boss.m_EnumState = State::Idle;
        boss.m_DoBeamRun = false;
        std::shared_ptr<IdleStateBoss> ptr1 = std::make_shared<IdleStateBoss>();
        std::shared_ptr<BossState> ptr2 = std::static_pointer_cast<BossState>(ptr1);
        return ptr2;
    }

    return nullptr;
}

void BeamRunStateBoss::update(Boss& boss)
{
    glm::vec2 newPosCoord = { boss.GetTransform()->GetPosition().x, ScreenInfo::GetInstance().screenheigth / 2 + 50 };

    float elapsedSec = Time::GetInstance().m_ElapsedSec;
    int speed = 150;
    float velocity{};
    velocity = speed * elapsedSec;

    // POSITION 1
    if (!m_ReachedPosX || !m_ReachedPosY)
    {
        GoToPosition(boss, newPosCoord, velocity, m_ReachedPosX, m_ReachedPosY, false, true);
    }
    // POSITION 1 REACHED -> DO BEAM
    else if (!m_BeamDone)
    {
        std::shared_ptr<Beam> dBeam = std::dynamic_pointer_cast<Beam> (boss.GetBeam());
        dBeam->m_IsActive = true;
        m_BeamTimer += elapsedSec;

        if (m_BeamTimer >= m_BeamTime)
        {
            if (boss.m_AbductedPlayer)
            {
                if (boss.m_AbductedPlayer->GetReachedAbductionPos())
                {
                    m_BeamTimer = 0.0f;
                    dBeam->m_IsActive = false;
                    m_BeamDone = true;
                    boss.m_AbductedPlayer = nullptr;
                }
            }
            else
            {
                m_BeamTimer = 0.0f;
                dBeam->m_IsActive = false;
                m_BeamDone = true;
            }
        }

    }

    // BEAM IS DONE -> GO POSITION IDLE
    else if (m_BeamDone)
	{
        GoToPosition(boss, boss.GetIdlePos(), velocity, m_ReachedPosXIdle, m_ReachedPosYIdle, false, true);
    }
}

std::shared_ptr<BossState> ExplodeStateBoss::handleInput(Boss& boss)
{
        if (boss.GetComponent<SpriteComponent>()->IsAnimPlayed())
        {
            boss.m_IsDead = true;
        }

    return nullptr;
}

void BossState::GoToPosition(Boss& boss, glm::vec2 newPos, float velocity, bool& reachedX, bool& reachedY,bool xFirst, bool yFirst)
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
        if (boss.m_Rect.x > newPos.x + 2)
            boss.SetPosition(boss.GetTransform()->GetPosition().x - velocity, boss.GetTransform()->GetPosition().y);
        else if (boss.m_Rect.x < newPos.x - 2)
            boss.SetPosition(boss.GetTransform()->GetPosition().x + velocity, boss.GetTransform()->GetPosition().y);
        else reachedX = true;
    }

    if (doY)
    {
        if (boss.m_Rect.y < newPos.y - 2)
            boss.SetPosition(boss.GetTransform()->GetPosition().x, boss.GetTransform()->GetPosition().y + velocity);
        else if (boss.m_Rect.y > newPos.y + 2)
            boss.SetPosition(boss.GetTransform()->GetPosition().x, boss.GetTransform()->GetPosition().y - velocity);
        else reachedY = true;
    }
}
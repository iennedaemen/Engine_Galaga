#pragma once
#include "structs.h"
#include <SDL_hints.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Boss;

class BossState
{
public:

    virtual ~BossState() {}
    virtual std::shared_ptr<BossState> handleInput(Boss& boss)
    {
        UNREFERENCED_PARAMETER(boss);
        return nullptr;
    }
    virtual void update(Boss& boss)
    {
        UNREFERENCED_PARAMETER(boss);
    }

protected:
    void GoToPosition(Boss& boss, glm::vec2 newPos, float velocity, bool& reachedX, bool& reachedY, bool xFirst = false, bool yFirst = false);
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class IdleStateBoss : public BossState
{
public:
    IdleStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;

};

class SpawnStateBoss : public BossState
{
public:
    SpawnStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;
    virtual void update(Boss& boss) override;

private:
};

class ShootingRunStateBoss : public BossState
{
public:
    ShootingRunStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;
    virtual void update(Boss& boss) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
    bool m_ReachedPosX2 = false;
    bool m_ReachedPosY2 = false;
    bool m_ReachedPosX3 = false;
    bool m_ReachedPosY3 = false;
};

class BeamRunStateBoss : public BossState
{
public:
    BeamRunStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;
    virtual void update(Boss& boss) override;

private:
    bool m_ReachedPosX = false;
    bool m_ReachedPosY = false;
    bool m_BeamDone = false;
    float m_BeamTime = 5.0f;
    float m_BeamTimer = 0.0f;
};

class ExplodeStateBoss : public BossState
{
public:
    ExplodeStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;

};

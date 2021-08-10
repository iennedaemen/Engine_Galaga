#pragma once
#include "structs.h"
#include <SDL_hints.h>
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
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class ShootingRunStateBoss : public BossState
{
public:
    ShootingRunStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;
    virtual void update(Boss& boss) override;

private:
    bool m_ReachedPos1 = false;
    bool m_ReachedPosX2 = false;
    bool m_ReachedPosY2 = false;
    bool m_ReachedPos3 = false;
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
    float m_LastPosX = 0;
};

class BeamRunStateBoss : public BossState
{
public:
    BeamRunStateBoss() {}

    virtual std::shared_ptr<BossState> handleInput(Boss& boss) override;
    virtual void update(Boss& boss) override;

private:
    bool m_ReachedPos = false;
    bool m_ReachedPosYIdle = false;
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

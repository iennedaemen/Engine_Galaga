#pragma once
#include "structs.h"
#include <SDL_hints.h>
class Goei;

class GoeiState
{
public:

    virtual ~GoeiState() {}
    virtual std::shared_ptr<GoeiState> HandleState(Goei& goei)
    {
        UNREFERENCED_PARAMETER(goei);
        return nullptr;
    }
    virtual void Update(Goei& goei)
    {
        UNREFERENCED_PARAMETER(goei);
    }
};

class IdleStateGoei : public GoeiState
{
public:
    IdleStateGoei() {}

    virtual std::shared_ptr<GoeiState> HandleState(Goei& goei) override;

};

class SpawnStateGoei : public GoeiState
{
public:
    SpawnStateGoei() {}

    virtual std::shared_ptr<GoeiState> HandleState(Goei& goei) override;
    virtual void Update(Goei& goei) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class ShootingRunStateGoei : public GoeiState
{
public:
    ShootingRunStateGoei() {}

    virtual std::shared_ptr<GoeiState> HandleState(Goei& goei) override;
    virtual void Update(Goei& goei) override;

private:
    bool m_ReachedPos1 = false;
    bool m_ReachedPos2 = true;
    bool m_ReachedPosY = false;
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class ExplodeStateGoei : public GoeiState
{
public:
    ExplodeStateGoei() {}

    virtual std::shared_ptr<GoeiState> HandleState(Goei& goei) override;

};

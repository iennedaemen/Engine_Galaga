#pragma once
#include "structs.h"
#include <SDL_hints.h>
class Zako;

class ZakoState
{
public:

    virtual ~ZakoState() {}
    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako)
    {
        UNREFERENCED_PARAMETER(zako);
        return nullptr;
    }
    virtual void update(Zako& zako)
    {
        UNREFERENCED_PARAMETER(zako);
    }

};

class IdleStateZako : public ZakoState
{
public:
    IdleStateZako() {}

    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako) override;

};

class SpawnStateZako : public ZakoState
{
public:
    SpawnStateZako() {}

    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako) override;
    virtual void update(Zako& zako) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class ShootingRunStateZako : public ZakoState
{
public:
    ShootingRunStateZako() {}

    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako) override;
    virtual void update(Zako& zako) override;

private:
    bool m_ReachedPos1 = false;
    bool m_ReachedPosX2 = false;
    bool m_ReachedPosY2 = false;
    bool m_ReachedPos3 = false;
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class CrashRunStateZako : public ZakoState
{
public:
    CrashRunStateZako() {}

    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako) override;
    virtual void update(Zako& zako) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
    bool m_ReachedPos2 = false;
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class ExplodeStateZako : public ZakoState
{
public:
    ExplodeStateZako() {}

    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako) override;

};

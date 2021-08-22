#pragma once
#include "structs.h"
#include <SDL_hints.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Zako;

class ZakoState
{
public:
    virtual ~ZakoState() {}
    virtual std::shared_ptr<ZakoState> HandleState(Zako& zako)
    {
        UNREFERENCED_PARAMETER(zako);
        return nullptr;
    }
    virtual void Update(Zako& zako)
    {
        UNREFERENCED_PARAMETER(zako);
    }

protected:
    void GoToPosition(Zako& zako, glm::vec2 newPos, float velocity, bool& reachedX, bool& reachedY, bool xFirst = false, bool yFirst = false);
    bool m_ReachedPosXIdle = false;
    bool m_ReachedPosYIdle = false;
};

class IdleStateZako : public ZakoState
{
public:
    IdleStateZako() {}

    virtual std::shared_ptr<ZakoState> HandleState(Zako& zako) override;

};

class SpawnStateZako : public ZakoState
{
public:
    SpawnStateZako() {}

    virtual std::shared_ptr<ZakoState> HandleState(Zako& zako) override;
    virtual void Update(Zako& zako) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
};

class ShootingRunStateZako : public ZakoState
{
public:
    ShootingRunStateZako() {}

    virtual std::shared_ptr<ZakoState> HandleState(Zako& zako) override;
    virtual void Update(Zako& zako) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
    bool m_ReachedPosX2 = false;
    bool m_ReachedPosY2 = false;
    bool m_ReachedPosX3 = false;
    bool m_ReachedPosY3 = false;
};

class CrashRunStateZako : public ZakoState
{
public:
    CrashRunStateZako() {}

    virtual std::shared_ptr<ZakoState> HandleState(Zako& zako) override;
    virtual void Update(Zako& zako) override;

private:
    bool m_ReachedPosX1 = false;
    bool m_ReachedPosY1 = false;
    bool m_ReachedPosX2 = false;
    bool m_ReachedPosY2 = false;
};

class ExplodeStateZako : public ZakoState
{
public:
    ExplodeStateZako() {}

    virtual std::shared_ptr<ZakoState> HandleState(Zako& zako) override;

};

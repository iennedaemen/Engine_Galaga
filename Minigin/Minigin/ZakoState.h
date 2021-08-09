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
};

class ExplodeStateZako : public ZakoState
{
public:
    ExplodeStateZako() {}

    virtual std::shared_ptr<ZakoState> handleInput(Zako& zako) override;

};

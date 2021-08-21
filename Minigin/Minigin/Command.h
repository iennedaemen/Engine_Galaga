#pragma once
#include "Player.h"
#include "Time.h"
#include "ScreenInfo.h"

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(Player& player) = 0;
    virtual void undo(Player& player) { UNREFERENCED_PARAMETER(player); }
};

class MoveLeftCommand : public Command
{
public:
    virtual void execute(Player& player) override;
};

class MoveRightCommand : public Command
{
public:
    virtual void execute(Player& player) override;
};

class ShootCommand : public Command
{
public:
    virtual void execute(Player& player) override;
};
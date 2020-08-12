#pragma once
#include "Player.h"
#include "Time.h"

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(Player& player) = 0;
    virtual void undo(Player& player) { UNREFERENCED_PARAMETER(player); }
};

class WalkLeftCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        float elapsedSec = Time::GetInstance().m_ElapsedSec;
        int speed = 40;
    	
        glm::vec2 velocity{};
    	velocity.x = speed * elapsedSec;
        glm::vec2 pos = { player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y };

        player.SetPosition(pos.x - velocity.x, pos.y);
    }
};

class WalkRightCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        float elapsedSec = Time::GetInstance().m_ElapsedSec;
        int speed = 40;

        glm::vec2 velocity{};
        velocity.x = speed * elapsedSec;
        glm::vec2 pos = { player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y };

        player.SetPosition(pos.x + velocity.x, pos.y);
    } 
};

class WalkUpCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        float elapsedSec = Time::GetInstance().m_ElapsedSec;
        int speed = 40;

        glm::vec2 velocity{};
        velocity.y = speed * elapsedSec;
        glm::vec2 pos = { player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y };

        player.SetPosition(pos.x , pos.y - velocity.y);
    } 
};

class WalkDownCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        float elapsedSec = Time::GetInstance().m_ElapsedSec;
        int speed = 40;

        glm::vec2 velocity{};
        velocity.y = speed * elapsedSec;
        glm::vec2 pos = { player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y };

        player.SetPosition(pos.x, pos.y + velocity.y);
    }
};

class ShootCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        UNREFERENCED_PARAMETER(player);
    }
};
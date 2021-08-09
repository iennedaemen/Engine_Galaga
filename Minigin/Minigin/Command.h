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

protected:
    bool isMoving = false;
};

class MoveLeftCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        int boundery = 0;

        if (player.GetTransform().GetPosition().x > boundery)
        {
            float elapsedSec = Time::GetInstance().m_ElapsedSec;
            int speed = 200;
            float velocity = speed * elapsedSec;
            player.SetPosition(player.GetTransform().GetPosition().x - velocity, player.GetTransform().GetPosition().y);
        }
        else
        {
            player.SetPosition((float)boundery, player.GetTransform().GetPosition().y);
        }
        /*if (player.IsLookingRight())
        {
            glm::vec2 newPos = { player.GetCurrentPosition().x, player.GetCurrentPosition().y - 1 };
            if (newPos.y < 0 || newPos.x >= newPos.y + 1)
            {
                player.SetMoving(false);
                return;
            }
            glm::vec2 newPosCoord = { PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].x, PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].y };
           
            float elapsedSec = Time::GetInstance().m_ElapsedSec;
            int speedX = 40;
            int speedY = 58;
            glm::vec2 velocity{};
            velocity.x = speedX * elapsedSec;
            velocity.y = speedY * elapsedSec;
            if(player.m_Rect.x < newPosCoord.x)
                player.SetPosition(player.GetTransform().GetPosition().x + velocity.x, player.GetTransform().GetPosition().y);
            if (player.m_Rect.y > newPosCoord.y)
                player.SetPosition(player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y - velocity.y);
            
            if (player.m_Rect.x >= newPosCoord.x && player.m_Rect.y <= newPosCoord.y)
            {
                player.SetPosition(newPosCoord.x, newPosCoord.y);
                player.SetCurrentPosition(newPos);
                player.SetMoving(false);
            }
        }

        if (!player.IsLookingRight())
        {
            glm::vec2 newPos = { player.GetCurrentPosition().x - 1, player.GetCurrentPosition().y - 1 };
            if (newPos.y < 0 || newPos.x < 0)
            {
                player.SetMoving(false);
                return;
            }
            glm::vec2 newPosCoord = { PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].x, PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].y };

            float elapsedSec = Time::GetInstance().m_ElapsedSec;
            int speedX = 40;
            int speedY = 58;
            glm::vec2 velocity{};
            velocity.x = speedX * elapsedSec;
            velocity.y = speedY * elapsedSec;
            if (player.m_Rect.x > newPosCoord.x)
                player.SetPosition(player.GetTransform().GetPosition().x - velocity.x, player.GetTransform().GetPosition().y);
            if (player.m_Rect.y > newPosCoord.y)
                player.SetPosition(player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y - velocity.y);

            if (player.m_Rect.x <= newPosCoord.x && player.m_Rect.y <= newPosCoord.y)
            {
                player.SetPosition(newPosCoord.x, newPosCoord.y);
                player.SetCurrentPosition(newPos);
                player.SetMoving(false);
            }
        }*/
    } 
};

class MoveRightCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        UNREFERENCED_PARAMETER(player);

        float boundary = float(ScreenInfo::GetInstance().screenwidth - player.GetRect().w) ;

        if (player.GetTransform().GetPosition().x < boundary)
        {
            float elapsedSec = Time::GetInstance().m_ElapsedSec;
            int speed = 200;
            float velocity = speed * elapsedSec;
            player.SetPosition(player.GetTransform().GetPosition().x + velocity, player.GetTransform().GetPosition().y);
        }
        else
        {
            player.SetPosition((float)boundary, player.GetTransform().GetPosition().y);
        }
        /*if (player.IsLookingRight())
        {
            glm::vec2 newPos = { player.GetCurrentPosition().x + 1, player.GetCurrentPosition().y + 1 };
            if (newPos.y > 6) 
            { 
                player.SetMoving(false);
                return; 
            }
            glm::vec2 newPosCoord = { PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].x, PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].y };

            float elapsedSec = Time::GetInstance().m_ElapsedSec;
            int speedX = 40;
            int speedY = 58;
            glm::vec2 velocity{};
            velocity.x = speedX * elapsedSec;
            velocity.y = speedY * elapsedSec;
            if (player.m_Rect.x < newPosCoord.x)
                player.SetPosition(player.GetTransform().GetPosition().x + velocity.x, player.GetTransform().GetPosition().y);
            if (player.m_Rect.y < newPosCoord.y)
                player.SetPosition(player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y + velocity.y);

            if (player.m_Rect.x >= newPosCoord.x && player.m_Rect.y >= newPosCoord.y)
            {
                player.SetPosition(newPosCoord.x, newPosCoord.y);
                player.SetCurrentPosition(newPos);
                player.SetMoving(false);
            }
        }

    
        if (!player.IsLookingRight())
        {
            glm::vec2 newPos = { player.GetCurrentPosition().x, player.GetCurrentPosition().y + 1 };
            if (newPos.y > 6)
            {
                player.SetMoving(false);
                return;
            }
            glm::vec2 newPosCoord = { PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].x, PyramidPos::GetInstance().Positions[int(newPos.y)][int(newPos.x)].y };

            float elapsedSec = Time::GetInstance().m_ElapsedSec;
            int speedX = 40;
            int speedY = 58;
            glm::vec2 velocity{};
            velocity.x = speedX * elapsedSec;
            velocity.y = speedY * elapsedSec;
            if (player.m_Rect.x > newPosCoord.x)
                player.SetPosition(player.GetTransform().GetPosition().x - velocity.x, player.GetTransform().GetPosition().y);
            if (player.m_Rect.y < newPosCoord.y)
                player.SetPosition(player.GetTransform().GetPosition().x, player.GetTransform().GetPosition().y + velocity.y);

            if (player.m_Rect.x <= newPosCoord.x && player.m_Rect.y >= newPosCoord.y)
            {
                player.SetPosition(newPosCoord.x, newPosCoord.y);
                player.SetCurrentPosition(newPos);
                player.SetMoving(false);
            }
        }*/
    }
};

class ShootCommand : public Command
{
public:
    virtual void execute(Player& player) override
    {
        UNREFERENCED_PARAMETER(player);

        if (!player.m_Shot)
        {
            player.ShootLaser();
            player.m_Shot = true;
        }
    }
};
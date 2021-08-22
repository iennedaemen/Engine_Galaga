#include "MiniginPCH.h"
#include "Command.h"

void MoveLeftCommand::execute(Player& player)
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
}

void MoveRightCommand::execute(Player& player)
{
    float boundary = float(ScreenInfo::GetInstance().screenwidth - player.GetRect().w);

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
}

void ShootCommand::execute(Player& player)
{
    if (!player.m_IsShot)
    {
        player.ShootLaser();
        player.m_IsShot = true;
    }
}
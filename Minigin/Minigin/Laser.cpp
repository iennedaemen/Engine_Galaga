#include "MiniginPCH.h"
#include "Laser.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "ColliderComponent.h"
#include "SceneManager.h"
#include "Scene.h"

void Laser::Initialize()
{
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(6.0f, 8.0f, 2, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Laser.png");
	pSpriteComp->IsStatic(true);
	if (m_IsPlayerLaser) pSpriteComp->SetSpriteSheetTopLeft(0, 0);
	else pSpriteComp->SetSpriteSheetTopLeft(1, 0);
	m_Rect = { m_Rect.x, m_Rect.y, 6, 16 };
	std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	AddComponent(pCollComp);
}

void Laser::Update()
{
	if (m_IsActive)
	{
		float elapsedSec = Time::GetInstance().m_ElapsedSec;
		int speed = 500;
		float velocity = speed * elapsedSec;
		if (m_IsPlayerLaser) SetPosition(GetTransform().GetPosition().x, GetTransform().GetPosition().y - velocity);
		else SetPosition(GetTransform().GetPosition().x, GetTransform().GetPosition().y + velocity);
	}
	else
	{
		SetPosition(-100, 0);
	}
}


#include "MiniginPCH.h"
#include "Beam.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"

void Beam::Initialize()
{
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(138.0f, 80.0f, 3, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Beam.png");
	pSpriteComp->IsStatic(false);
	std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	AddComponent(pCollComp);
	m_Rect = { m_Rect.x, m_Rect.y, 69, 120 };
}

void Beam::Update()
{
	if (!m_IsActive)
	{
		m_Rect.x = -100;
	}
}
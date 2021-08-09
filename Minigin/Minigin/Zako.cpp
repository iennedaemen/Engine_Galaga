#include "MiniginPCH.h"
#include "Zako.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"

void Zako::Initialize()
{
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(52.0f, 40.0f, 1, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Zako.png");
	pSpriteComp->IsStatic(true);
	pSpriteComp->SetSpriteSheetTopLeft(0, 0);
	m_Rect = { m_Rect.x, m_Rect.y, 20, 15 };

}

void Zako::Update()
{
	// STATE
	std::shared_ptr<ZakoState> newState = nullptr;
	newState = m_State->handleInput(*this);
	if (newState != nullptr)
	{
		m_State = newState;
	}
	if (m_State)
		m_State->update(*this);
}
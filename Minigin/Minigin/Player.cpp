#include "MiniginPCH.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "ColliderComponent.h"
#include "InputManager.h"


void Player::Initialize()
{	
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(244.0f, 61.0f, 4, 1, 0);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Player1.png");
	pSpriteComp->IsStatic(true);
	std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	pCollComp->SetSpeed(speed);
	AddComponent(pCollComp);
	SetPosition(30, 50);
	SetRect({ 30, 50, 128 / 8, 259 / 16 });
	m_Rect = GetRect();
}

void Player::Update()
{
	// COMMAND
	std::shared_ptr<Command> command = InputManager::GetInstance().HandleInput();
	if(command != nullptr)
	{
		command->execute(*this);
	}

	// STATE
	std::shared_ptr<PlayerState> newState = nullptr;
	newState = m_State->handleInput(*this);
	if(newState != nullptr)
	{
		m_State = newState;
	}
	if (m_State)
		m_State->update(*this);

	
	// COLLIDER RECT
	m_Rect.x = int(GetTransform().GetPosition().x);
	m_Rect.y = int(GetTransform().GetPosition().y);
	SetRect(m_Rect);
}

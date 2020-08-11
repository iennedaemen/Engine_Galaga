#include "MiniginPCH.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "ColliderComponent.h"


void Player::Initialize()
{	
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(128.0f, 256.0f, 8, 16, 5);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Walk2.png");
	std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	pCollComp->SetSpeed(speed);
	AddComponent(pCollComp);
	SetPosition(30, 50);
	SetRect({ 30, 50, 128 / 8, 259 / 16 });
	m_Rect = GetRect();
}

void Player::Update()
{
	float elapsedSec = Time::GetInstance().m_ElapsedSec;

	std::shared_ptr<PlayerState> newState = nullptr;
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT])
	{
		m_Velocity.x = speed * elapsedSec;
		glm::vec2 pos = { GetTransform().GetPosition().x, GetTransform().GetPosition().y };

		SetPosition(pos.x - m_Velocity.x, pos.y);
		newState = m_State->handleInput(*this, SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT]);
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT])
	{
		m_Velocity.x = speed * elapsedSec;
		glm::vec2 pos = { GetTransform().GetPosition().x, GetTransform().GetPosition().y };

		SetPosition(pos.x + m_Velocity.x, pos.y);
		newState = m_State->handleInput(*this, SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT]);
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DOWN])
	{
		m_Velocity.y = speed * elapsedSec;
		glm::vec2 pos = { GetTransform().GetPosition().x, GetTransform().GetPosition().y };

		SetPosition(pos.x, pos.y + m_Velocity.y);
		newState = m_State->handleInput(*this, SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DOWN]);
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP])
	{
		m_Velocity.y = speed * elapsedSec;
		glm::vec2 pos = { GetTransform().GetPosition().x, GetTransform().GetPosition().y };

		SetPosition(pos.x, pos.y - m_Velocity.y);
		newState = m_State->handleInput(*this, SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP]);

	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE])
	{
		newState = m_State->handleInput(*this, SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE]);
	}

	if(newState != nullptr)
	{
		m_State = newState;
	}

	m_Rect.x = int(GetTransform().GetPosition().x);
	m_Rect.y = int(GetTransform().GetPosition().y);
	SetRect(m_Rect);

	if(m_State)
		m_State->update(*this);
}

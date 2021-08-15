#include "MiniginPCH.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "InputManager.h"
#include "Laser.h"


void Player::Initialize()
{	
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(60.0f, 64.0f, 1, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Player" + std::to_string(m_PlayerNr) + ".png");
	pSpriteComp->IsStatic(true);
	pSpriteComp->SetSpriteSheetTopLeft(0, 0);
	std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	AddComponent(pCollComp);	
	m_Rect = { m_Rect.x, m_Rect.y, 25, 27 };

	m_pLasers[0] = std::shared_ptr<GameObject>(std::make_shared<Laser>(true));
	Add(m_pLasers[0]);
	m_pLasers[0]->SetPosition(-100, 0);

	m_pLasers[1] = std::shared_ptr<GameObject>(std::make_shared<Laser>(true));
	Add(m_pLasers[1]);
	m_pLasers[1]->SetPosition(-100, 0);


}

void Player::Update()
{
	// COMMAND
	if (!m_Exploding)
	{
		if (!m_Abducted)
		{
			std::shared_ptr<Command> command = InputManager::GetInstance().HandleInput(m_PlayerNr, *this);
			if (command)
			{
				command->execute(*this);
			}
		}
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


	// LASERS
	for (std::shared_ptr<GameObject> laser : m_pLasers)
	{
		if (laser->GetRect().y < -20)
		{
			laser->m_Rect.x = -100;
			std::shared_ptr<Laser> derived = std::dynamic_pointer_cast<Laser> (laser);
			derived->SetActive(false);
		}
	}

	// ABDUCTION
	if (m_Abducted)
	{
		float elapsedSec = Time::GetInstance().m_ElapsedSec;
		float velocity{};
		if(m_ReachedAbductionPos)
			velocity = 150 * elapsedSec;
		else velocity = 50 * elapsedSec;


		if (GetTransform().GetPosition().x > m_pKidnapper->GetRect().x + 2.0f)
			SetPosition(GetTransform().GetPosition().x - velocity, GetTransform().GetPosition().y);
		else if (GetTransform().GetPosition().x < m_pKidnapper->GetRect().x + 1.0f)
			SetPosition(GetTransform().GetPosition().x + velocity, GetTransform().GetPosition().y);
		else SetPosition(m_pKidnapper->GetRect().x + 2.0f, GetTransform().GetPosition().y);

		if (GetTransform().GetPosition().y > m_pKidnapper->GetRect().y + 40)
			SetPosition(GetTransform().GetPosition().x, GetTransform().GetPosition().y - velocity);
		else m_ReachedAbductionPos = true;

		if (m_pKidnapper->IsIdle())
		{
			m_IsHit = true;
			m_ReachedAbductionPos = false;
			m_Abducted = false;

		}

		
	}
}

void Player::ShootLaser()
{
	for (std::shared_ptr<GameObject> laser : m_pLasers)
	{
		std::shared_ptr<Laser> derived = std::dynamic_pointer_cast<Laser> (laser);
		if (!derived->IsActive())
		{
			derived->SetActive(true);
 			laser->SetPosition(float(m_Rect.x + m_Rect.w / 2 - laser->m_Rect.w / 2), float(m_Rect.y));
			return;
		}
	}
}

void Player::RemoveLaser(std::shared_ptr<Laser> laser)
{
	if (laser->IsActive())
	{
		if (laser == m_pLasers[0] || laser == m_pLasers[1])
		{
			laser->SetActive(false);
		}
	}
}
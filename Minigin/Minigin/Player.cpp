#include "MiniginPCH.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "InputManager.h"
#include "Laser.h"
#include "GameInfo.h"


void Player::Initialize()
{
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(60.0f, 64.0f, 1, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Player" + std::to_string(m_PlayerNr) + ".png");
	pSpriteComp->IsStatic(true);
	std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	AddComponent(pCollComp);
	m_Rect = { m_Rect.x, m_Rect.y, 60, 64 };
	GetTransform()->SetScale(0.4f);

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
	if (!m_IsExploding)
	{
		if (!m_IsAbducted)
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
	newState = m_pState->handleInput(*this);
	if (newState != nullptr)
		m_pState = newState;
	if (m_pState)
		m_pState->update(*this);


	// LASERS
	for (std::shared_ptr<GameObject> laser : m_pLasers)
	{
		if (laser->GetRect().y < -20)
		{
			laser->m_Rect.x = -100;
			std::shared_ptr<Laser> derived = std::dynamic_pointer_cast<Laser> (laser);
			derived->m_IsActive = false;
		}
	}

	// ABDUCTION
	if (m_IsAbducted)
	{
		float elapsedSec = Time::GetInstance().m_ElapsedSec;

		if (!m_ReachedAbductionPos || m_RotAngle >= 2)
		{
			m_RotAngle += 400 * elapsedSec;
			if (m_RotAngle >= 360) m_RotAngle = 0;
			GetTransform()->SetRotation(m_RotAngle);
		}

		float velocity{};
		if (m_ReachedAbductionPos)
			velocity = 150 * elapsedSec;
		else velocity = 50 * elapsedSec;


		if (GetTransform()->GetPosition().x > m_pKidnapper->GetRect().x + 2.0f)
			SetPosition(GetTransform()->GetPosition().x - velocity, GetTransform()->GetPosition().y);
		else if (GetTransform()->GetPosition().x < m_pKidnapper->GetRect().x + 1.0f)
			SetPosition(GetTransform()->GetPosition().x + velocity, GetTransform()->GetPosition().y);
		else SetPosition(m_pKidnapper->GetRect().x + 2.0f, GetTransform()->GetPosition().y);

		if (GetTransform()->GetPosition().y > m_pKidnapper->GetRect().y + 40)
			SetPosition(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y - velocity);
		else m_ReachedAbductionPos = true;

		if (m_pKidnapper->m_EnumState == State::Idle)
		{
			m_IsHit = true;
			m_ReachedAbductionPos = false;
			m_IsAbducted = false;
			m_RotAngle = 0.0f;
			GetTransform()->SetRotation(0.0f);
		}
	}
}

void Player::ShootLaser()
{
	for (std::shared_ptr<GameObject> laser : m_pLasers)
	{
		std::shared_ptr<Laser> l = std::dynamic_pointer_cast<Laser> (laser);
		if (!l->m_IsActive)
		{
			if (m_PlayerNr == 1) GameInfo::GetInstance().shotsFiredP1 += 1;
			else if (m_PlayerNr == 2) GameInfo::GetInstance().shotsFiredP2 += 1;

			l->m_IsActive = true;
			laser->SetPosition(float(m_Rect.x + m_Rect.w / 2 - laser->m_Rect.w / 2), float(m_Rect.y));
			return;
		}
	}
}

void Player::RemoveLaser(std::shared_ptr<Laser> laser)
{
	if (laser->m_IsActive)
	{
		if (laser == m_pLasers[0] || laser == m_pLasers[1])
		{
			laser->m_IsActive = false;
		}
	}
}

void Player::SetState(PlayerState state)
{
	m_pState = std::make_shared<PlayerState>(state);
}

const int Player::GetPlayerNr()
{
	return m_PlayerNr;
}

const std::shared_ptr<GameObject> Player::GetLaser(int index)
{
	return m_pLasers[index];
}

const bool Player::IsAbducted()
{
	return m_IsAbducted;
}

void Player::SetAbducted(bool abducted)
{
	m_IsAbducted = abducted;
}

void Player::SetAbducted(bool abducted, std::shared_ptr<GameObject> kidnapper)
{
	m_IsAbducted = abducted;
	m_pKidnapper = std::dynamic_pointer_cast<Boss>(kidnapper);
}

const std::shared_ptr<GameObject> Player::GetKidnapper()
{
	return m_pKidnapper;
}
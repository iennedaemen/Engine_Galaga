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
	pSpriteComp->SetTexture("Player1.png");
	pSpriteComp->IsStatic(true);
	pSpriteComp->SetSpriteSheetTopLeft(0, 0);
	//std::shared_ptr<ColliderComponent> pCollComp = std::make_shared<ColliderComponent>();
	//pCollComp->SetSpeed(speed);
	//AddComponent(pCollComp);	
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
	std::shared_ptr<Command> command = InputManager::GetInstance().HandleInput(m_PlayerNr, *this);
	if (command)
	{
		command->execute(*this);
	}
	////std::shared_ptr<Command> command = InputManager::GetInstance().HandleInput(m_PlayerNr);
	//if (!m_IsMoving)
	//{
	//	m_pLastCommand = InputManager::GetInstance().HandleInput(m_PlayerNr);
	//}
	//if(m_pLastCommand != nullptr)
	//{
	//	m_IsMoving = true;
	//	m_pLastCommand->execute(*this);
	//}

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
	
	if (laser == m_pLasers[0] || laser == m_pLasers[1])
	{
		laser->SetActive(false);
		laser->m_Rect.x = -100;
	}
}
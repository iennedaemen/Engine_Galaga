#pragma once
#include "GameObject.h"
//#include "Command.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include <SDL.h>
#include "PlayerState.h"
#pragma warning(pop)
#include "Boss.h"

class Command;
class Laser;

class Player : public GameObject
{
public:
	Player(int playerNr) : m_PlayerNr(playerNr) {}

	void SetState(PlayerState state)
	{
		m_State = std::make_shared<PlayerState>(state);
	}

	void SetVelocity(glm::vec2 velocity)
	{
		m_Velocity = velocity;
	}

	int getPlayerNr()
	{
		return m_PlayerNr;
	}

	void SetMoving(bool isMoving)
	{
		m_IsMoving = isMoving;
	}

	bool GetMoving()
	{
		return m_IsMoving;
	}


	void ShootLaser();
	void RemoveLaser(std::shared_ptr<Laser> laser);
	//void Move();

	std::shared_ptr<GameObject> GetLaser(int index)
	{
		return m_pLasers[index];
	}

	bool m_Shot = false;

	void SetIsHit(bool isHit)
	{
		m_IsHit = isHit;
	}
	bool GetIsHit()
	{
		return m_IsHit;
	}
	void SetIsDead(bool isDead)
	{
		m_IsDead = isDead;
	}
	bool GetIsDead()
	{
		return m_IsDead;
	}
	int GetLives()
	{
		return m_Lives;
	}
	void SetLives(int nrLives)
	{
		m_Lives = nrLives;
	}

	bool IsAbducted()
	{
		return m_Abducted;
	}
	void SetAbducted(bool abducted, std::shared_ptr<GameObject> kidnapper)
	{
		m_Abducted = abducted;
		m_pKidnapper = std::dynamic_pointer_cast<Boss>(kidnapper);
	}

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	int m_PlayerNr;	
	int m_Lives = 3;
	bool m_IsHit = false;
	bool m_IsDead = false;
	glm::vec2 m_Velocity = {0, 0};
	float m_Gravity = -0.981f;
	float speed = 40;
	bool m_IsMoving = false;
	std::shared_ptr<Command> m_pLastCommand = nullptr;
	bool m_Abducted = false;
	std::shared_ptr<Boss> m_pKidnapper = nullptr;

	std::shared_ptr<PlayerState> m_State = std::make_shared<IdleState>();
	//std::vector<std::vector<glm::vec2>> m_PossiblePos;

	// LASERS
	std::shared_ptr<GameObject> m_pLasers[2];

	// ABDUCTION
	bool m_ReachedAbductionPos = false;
};


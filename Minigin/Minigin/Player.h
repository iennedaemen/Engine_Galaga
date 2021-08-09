#pragma once
#include "GameObject.h"
//#include "Command.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include <SDL.h>
#include "PlayerState.h"
#pragma warning(pop)
class Command;
class Laser;

class Player : public GameObject
{
public:
	Player(int playerNr) : m_PlayerNr(playerNr){}
	
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
	
protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	int m_PlayerNr;	
	glm::vec2 m_Velocity = {0, 0};
	float m_Gravity = -0.981f;
	float speed = 40;
	bool m_IsMoving = false;
	std::shared_ptr<Command> m_pLastCommand = nullptr;

	std::shared_ptr<PlayerState> m_State = std::make_shared<IdleState>();
	//std::vector<std::vector<glm::vec2>> m_PossiblePos;

	// LASERS
	std::shared_ptr<GameObject> m_pLasers[2];
};


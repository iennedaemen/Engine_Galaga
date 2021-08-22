#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include <SDL.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Command;
class Laser;
class Boss;

class Player : public GameObject
{
public:
	Player(int playerNr) : m_PlayerNr(playerNr) {}

	void ShootLaser();
	void RemoveLaser(std::shared_ptr<Laser> laser);
	void SetState(PlayerState state);
	const int GetPlayerNr();
	const std::shared_ptr<GameObject> GetLaser(int index);
	const bool IsAbducted();
	void SetAbducted(bool abducted);
	void SetAbducted(bool abducted, std::shared_ptr<GameObject> kidnapper);
	const std::shared_ptr<GameObject> GetKidnapper();
	bool GetReachedAbductionPos()
	{
		return m_ReachedAbductionPos;
	}

	bool m_IsHit = false;
	bool m_IsExploding = false;
	bool m_IsDead = false;
	bool m_IsShot = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	std::shared_ptr<Command> m_pLastCommand = nullptr;
	std::shared_ptr<PlayerState> m_pState = std::make_shared<IdleState>();
	std::shared_ptr<GameObject> m_pLasers[2];
	std::shared_ptr<Boss> m_pKidnapper = nullptr;
	float m_RotAngle = 0.0f;
	int m_PlayerNr;	
	bool m_IsAbducted = false;
	bool m_ReachedAbductionPos = false;

	//float q;
};


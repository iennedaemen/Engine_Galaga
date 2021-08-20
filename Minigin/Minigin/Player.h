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

	void ShootLaser();
	void RemoveLaser(std::shared_ptr<Laser> laser);
	void SetState(PlayerState state);
	const int GetPlayerNr();
	const std::shared_ptr<GameObject> GetLaser(int index);
	const bool IsAbducted();
	void SetAbducted(bool abducted);
	void SetAbducted(bool abducted, std::shared_ptr<GameObject> kidnapper);

	bool m_IsHit = false;
	bool m_Exploding = false;
	bool m_IsDead = false;
	bool m_Shot = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	std::shared_ptr<Command> m_pLastCommand = nullptr;
	std::shared_ptr<PlayerState> m_State = std::make_shared<IdleState>();
	std::shared_ptr<GameObject> m_pLasers[2];
	std::shared_ptr<Boss> m_pKidnapper = nullptr;
	int m_PlayerNr;	
	bool m_Abducted = false;
	bool m_ReachedAbductionPos = false;
};


#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "structs.h"
class GameObject;
class Laser;

class Enemy
{
public:
	Enemy() = default;
	~Enemy() = default;

	void ShootLaser(std::shared_ptr<GameObject> obj);

	const glm::vec2 GetIdlePos();
	const std::shared_ptr<GameObject> GetLaser();

	State m_EnumState = State::Moving;
	glm::vec2 m_IdlePos;
	glm::vec2 m_PlayerPos;
	glm::vec2 m_Player2Pos;
	bool m_IsHit = false;
	bool m_IsDead = false;
	bool m_SpawnedLeft = true;
	bool m_DoShootRun = false;

protected:
	Enemy(glm::vec2 idlePos) : m_IdlePos{ idlePos } {}

	std::shared_ptr<GameObject> m_pLaser = nullptr;
	float m_ActionTimer = 0.0f;
	float m_ActionTime = 5.0f;
};


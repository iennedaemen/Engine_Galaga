#pragma once
#include "GameObject.h"
#include "ZakoState.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Zako : public GameObject
{
public:
	Zako(glm::vec2 idlePos) : m_IdlePos(idlePos)
	{};

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

	glm::vec2 GetIdlePos()
	{
		return m_IdlePos;
	}

	bool SpawnedLeft()
	{
		return m_SpawnedLeft;
	}

	void SetSpawnedLeft(bool spawnedLeft)
	{
		m_SpawnedLeft = spawnedLeft;
	}

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	bool m_IsHit = false;
	bool m_IsDead = false;
	bool m_SpawnedLeft = true;
	glm::vec2 m_IdlePos;

	std::shared_ptr<ZakoState> m_State = std::make_shared<SpawnStateZako>();
};


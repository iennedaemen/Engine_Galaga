#pragma once
#include "GameObject.h"
#include "ZakoState.h"
#include "GameInfo.h"
#include "structs.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Zako : public GameObject
{
public:
	Zako(glm::vec2 idlePos) : m_IdlePos(idlePos)
	{};

	void ShootLaser();
	std::shared_ptr<GameObject> GetLaser()
	{
		return m_pLaser;
	}

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
	void SetPlayerPos(glm::vec2 playerPos, glm::vec2 player2Pos = {0, 0})
	{
		m_PlayerPos = playerPos;
		m_Player2Pos = player2Pos;
	}
	glm::vec2 GetPlayerPos()
	{
		return m_PlayerPos;
	}

	glm::vec2 GetPlayer2Pos()
	{
		return m_Player2Pos;
	}

	// ACTIONS
	void SetIsIdle(bool isIdle)
	{
		m_IsIdle = isIdle;
	}
	bool IsIdle()
	{
		return m_IsIdle;
	}
	bool DoShootRun()
	{
		return m_DoShootRun;
	}
	void SetShootRun(bool doShootRun)
	{
		m_DoShootRun = doShootRun;
	}
	bool DoCrashRun()
	{
		return m_DoCrashRun;
	}
	void SetCrashRun(bool doCrashRun)
	{
		m_DoCrashRun = doCrashRun;
	}
	void SetNextAction(bool doShootRun)
	{
		m_NextAction = doShootRun;
	}

	State GetEnumState()
	{
		return m_State->GetEnumState();
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
	glm::vec2 m_PlayerPos;
	glm::vec2 m_Player2Pos;
	std::shared_ptr<GameObject> m_pLaser = nullptr;

	std::shared_ptr<ZakoState> m_State = std::make_shared<SpawnStateZako>();


	// ACTIONS
	float m_ActionTimer = 0.0f;
	float m_ActionTime = 5.0f;
	bool m_DoShootRun = false;
	bool m_DoCrashRun = false;
	bool m_IsIdle = false;
	bool m_NextAction = false;
};


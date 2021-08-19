#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "ZakoState.h"
#include "GameInfo.h"
#include "structs.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Zako : public GameObject, public Enemy
{
public:
	Zako() {};
	Zako(glm::vec2 idlePos) : Enemy(idlePos)
	{};

	// ACTIONS
	void SetIsIdle(bool isIdle)
	{
		m_IsIdle = isIdle;
	}
	bool IsIdle()
	{
		return m_IsIdle;
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
	std::shared_ptr<ZakoState> m_State = std::make_shared<SpawnStateZako>();

	// ACTIONS
	bool m_DoCrashRun = false;
	bool m_IsIdle = false;
	bool m_NextAction = false;
};


#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "BossState.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Beam;

class Boss : public GameObject, public Enemy
{
public:
	Boss(glm::vec2 idlePos) : Enemy(idlePos)
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
	bool DoBeamRun()
	{
		return m_DoBeamRun;
	}
	void SetBeamRun(bool doBeamRun)
	{
		m_DoBeamRun = doBeamRun;
	}

	std::shared_ptr<GameObject> GetBeam()
	{
		return m_pBeam;
	}

	void SetNextAction(bool doShootRun)
	{
		m_NextAction = doShootRun;
	}

	int m_Lives = 2;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	std::shared_ptr<GameObject> m_pBeam = nullptr;
	std::shared_ptr<BossState> m_State = std::make_shared<SpawnStateBoss>();

	// ACTIONS
	bool m_DoBeamRun = false;
	bool m_IsIdle = false;
	bool m_NextAction = false;
};
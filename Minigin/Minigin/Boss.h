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
	Boss(glm::vec2 idlePos) : Enemy(idlePos) {}

	const std::shared_ptr<GameObject> GetBeam();
	void SetNextAction(bool doShootRun);

	int m_Lives = 2;
	bool m_DoBeamRun = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	std::shared_ptr<GameObject> m_pBeam = nullptr;
	std::shared_ptr<BossState> m_State = std::make_shared<SpawnStateBoss>();
	bool m_NextAction = false;
};
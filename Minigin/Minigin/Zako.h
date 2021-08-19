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

	void SetNextAction(bool doShootRun)
	{
		m_NextAction = doShootRun;
	}

	bool m_DoCrashRun = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;


private:
	std::shared_ptr<ZakoState> m_State = std::make_shared<SpawnStateZako>();
	bool m_NextAction = false;
};


#pragma once
#include "GameObject.h"
#include "GoeiState.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "Enemy.h"

class Goei : public GameObject, public Enemy
{
public:
	Goei(glm::vec2 idlePos) : Enemy(idlePos) {}

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	std::shared_ptr<GoeiState> m_pState = std::make_shared<SpawnStateGoei>();
};
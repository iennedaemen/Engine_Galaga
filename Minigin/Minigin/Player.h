#pragma once
#include "GameObject.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include <SDL.h>
#include "PlayerState.h"
#pragma warning(pop)

class Player : public GameObject
{
public:
	void SetState(PlayerState state)
	{
		m_State = std::make_shared<PlayerState>(state);
	}

	void SetVelocity(glm::vec2 velocity)
	{
		m_Velocity = velocity;
	}

	//void Move();
	
protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	glm::vec2 m_Velocity = {0, 0};
	float m_Gravity = -0.981f;
	float speed = 40;
	SDL_Rect m_Rect;

	std::shared_ptr<PlayerState> m_State = std::make_shared<IdleState>();
};


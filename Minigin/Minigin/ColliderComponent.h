#pragma once
#include "BaseComponent.h"
#include <SDL.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>

class ColliderComponent final : public BaseComponent
{
public:
	ColliderComponent();
	bool IsColliding(SDL_Rect rect);
	void Collide(SDL_Rect rect);
	bool IsGround(SDL_Rect rect);
	
	void SetSpeed(float& speed)
	{
		m_Velocity = speed;
	}

	bool IsOnGround()
	{
		return m_IsOnGround;
	}

	void setTrigger(bool isTrigger)
	{
		m_IsTrigger = isTrigger;
	}

protected:
	virtual void Update() override;
	virtual void Render() override {};
	virtual void Initialize() override{};

private:
	SDL_Rect* m_Rect;
	float m_Velocity;
	bool m_IsOnGround = false;
	bool m_IsTrigger = false;
	bool m_HitWall = false;

	bool CheckCollision(SDL_Rect A, SDL_Rect B);
};


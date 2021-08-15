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


protected:
	virtual void Update() override;
	virtual void Render() override {};
	virtual void Initialize() override{};

private:
	SDL_Rect* m_Rect;
};


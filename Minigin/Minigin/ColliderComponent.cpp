#include "MiniginPCH.h"
#include "ColliderComponent.h"

ColliderComponent::ColliderComponent()
	: m_Rect()
{}


bool ColliderComponent::IsColliding(SDL_Rect rect)
{

	SDL_Rect r = GetGameObject()->GetRect();
	if(SDL_HasIntersection(&rect, &r))
	{
		return true;
	}
	return false;
	
}

void ColliderComponent::Update()
{

}


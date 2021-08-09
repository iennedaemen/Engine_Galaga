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

bool ColliderComponent::IsGround(SDL_Rect rect)
{
	SDL_Rect r = GetGameObject()->GetRect();
	SDL_Rect r2{ r.x + 4, r.y + r.h - 4, r.w - 8, 8 };
	SDL_Rect r3{ r.x, r.y + 1, r.w, r.h };

	if (CheckCollision(r2, rect) && CheckCollision(r3, rect))
	{
		if (!m_IsOnGround)
		{
			m_IsOnGround = true;
			GetGameObject()->SetPosition(GetGameObject()->GetTransform().GetPosition().x, float(rect.y - r.h));
		}
		return true;
	}
	if (!CheckCollision(r2, rect) && m_IsOnGround)
	{
		m_IsOnGround = false;
		return false;
	}
	return false;
}

void ColliderComponent::Collide(SDL_Rect rect)
{
	if(m_IsTrigger)
		return;
		
	SDL_Rect r = GetGameObject()->GetRect();
	SDL_Rect r3{ r.x -4, r.y +4, r.w + 8, 2 };
	if (CheckCollision(rect, r) && CheckCollision(rect, r3))
	{
		if (r.x < rect.x)
			GetGameObject()->SetPosition(float(rect.x - r.w), GetGameObject()->GetTransform().GetPosition().y);
		if (r.x > rect.x)
			GetGameObject()->SetPosition(float(rect.x + rect.w), GetGameObject()->GetTransform().GetPosition().y);
	}
	//else if(!CheckCollision(rect, r3)) m_Velocity = 40;
}
void ColliderComponent::Update()
{

}

bool ColliderComponent::CheckCollision( SDL_Rect A, SDL_Rect B )
{
int leftA, leftB;
int rightA, rightB;
int topA, topB;
int bottomA, bottomB;

leftA = A.x;
rightA = A.x + A.w;
topA = A.y;
bottomA = A.y + A.h;

leftB = B.x;
rightB = B.x + B.w;
topB = B.y;
bottomB = B.y + B.h;

if (bottomA <= topB) { return false; }
if (topA >= bottomB) { return false; }
if (rightA <= leftB) { return false; }
if (leftA >= rightB) { return false; }
return true;
}


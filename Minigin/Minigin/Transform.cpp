#include "MiniginPCH.h"
#include "Transform.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SpriteComponent.h"

void Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;

	GetGameObject()->m_Rect.x = int(m_Position.x);
	GetGameObject()->m_Rect.y = int(m_Position.y);
}

void Transform::SetRotation(const float angle)
{
	m_RotationAngle = angle;
	GetGameObject()->GetComponent<SpriteComponent>()->UpdateRender();
}

void Transform::SetScale(const float scale)
{
	m_Scale = scale;
	GetGameObject()->m_Rect.w = int(GetGameObject()->m_Rect.w * scale);
	GetGameObject()->m_Rect.h = int(GetGameObject()->m_Rect.h * scale);
	GetGameObject()->GetComponent<SpriteComponent>()->UpdateRender();
}
#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "Time.h"
#include "ResourceManager.h"


RenderComponent::RenderComponent()
	: m_pTexture{ nullptr }
	, m_Height(0)
	, m_Width(0)
{
	m_NeedsUpdate = false;
}

void RenderComponent::SetTexture(const std::string& filename, float width, float height)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	m_Width = width;
	m_Height = height;
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::Render()
{
	if (m_Width == 0 || m_Height == 0)
	{
		Renderer::GetInstance().RenderTexture
		(
			*m_pTexture,
			GetGameObject()->GetTransform().GetPosition().x,
			GetGameObject()->GetTransform().GetPosition().y
		);
	}
	else {
		Renderer::GetInstance().RenderTexture
		(
			*m_pTexture,
			GetGameObject()->GetTransform().GetPosition().x,
			GetGameObject()->GetTransform().GetPosition().y,
			m_Width, m_Height
		);
	}
}
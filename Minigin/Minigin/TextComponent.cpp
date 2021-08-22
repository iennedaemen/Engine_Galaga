#include "MiniginPCH.h"
#include "TextComponent.h"
#include "TextObject.h"
#include "ResourceManager.h"
#include <string>
#include <SDL_ttf.h>
#include "RenderComponent.h"
#include "Renderer.h"
#include "GameObject.h"


TextComponent::TextComponent(std::string font, int size, SDL_Color color)
	: m_pText(new std::string(""))
	, m_Color(color)
{
	m_pFont = ResourceManager::GetInstance().LoadFont(font, size);
}

void TextComponent::Update()
{
		if(m_pText->empty())
		{
			return;
		}
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_pText->c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
}

void TextComponent::Render()
{
	if (m_pTexture != nullptr)
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, GetGameObject()->GetTransform()->GetPosition().x, GetGameObject()->GetTransform()->GetPosition().y);

	}
}

void TextComponent::SetFont(std::string font, int size)
{
	m_pFont = ResourceManager::GetInstance().LoadFont(font, size);
	m_NeedsUpdate = true;
}

void TextComponent::SetText(std::string text)
{
	m_pText = std::make_shared<std::string>(text);
	m_NeedsUpdate = true;
}

void TextComponent::SetColor(SDL_Color color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}
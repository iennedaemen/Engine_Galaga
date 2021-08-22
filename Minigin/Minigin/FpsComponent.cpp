#include "MiniginPCH.h"
#include "FpsComponent.h"
#include "TextComponent.h"
#include "Time.h"
#include "GameObject.h"

FpsComponent::FpsComponent(std::string font, int size, SDL_Color color)
{
	m_TextComp = std::make_shared<TextComponent>(font, size, color);
}

void FpsComponent::Initialize()
{
	GetGameObject()->AddComponent(m_TextComp);
	GetGameObject()->GetComponent<TextComponent>()->SetText("FPS: " + std::to_string(m_Fps));
}

void FpsComponent::Update()
{
	m_Fps = int(1/Time::GetInstance().m_ElapsedSec);
	GetGameObject()->GetComponent<TextComponent>()->SetText("FPS: " + std::to_string(m_Fps));
}

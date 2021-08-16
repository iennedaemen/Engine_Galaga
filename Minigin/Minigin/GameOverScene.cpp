#include "MiniginPCH.h"
#include "GameOverScene.h"
#include "TextComponent.h"
#include "SpriteComponent.h"
#include "InputManager.h"
#include "GameInfo.h"
#include "ScreenInfo.h"

GameOverScene::GameOverScene()
	: Scene("GameOverScene")
{

}

void GameOverScene::Initialize()
{
	std::shared_ptr<GameObject> pBackObj = std::make_shared<GameObject>();
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(1000.0f, 2000.0f, 1, 1);
	pBackObj->AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Background.png");
	pSpriteComp->IsStatic(true);
	pBackObj->m_Rect = { pBackObj->m_Rect.x, pBackObj->m_Rect.y, ScreenInfo::GetInstance().screenwidth, ScreenInfo::GetInstance().screenheigth };
	Add(pBackObj);

	m_pTextScore = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textScore = std::make_shared<TextComponent>("../Data/Pixel.otf", 20);
	textScore->SetText("Score: ");
	m_pTextScore->AddComponent(textScore);
	Add(m_pTextScore);
	m_pTextScore->SetPosition(150, 90);

	m_pTextScoreNr = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textScoreNr = std::make_shared<TextComponent>("../Data/Pixel.otf", 40);
	textScoreNr->SetText(std::to_string(GameInfo::GetInstance().score));
	m_pTextScoreNr->AddComponent(textScoreNr);
	Add(m_pTextScoreNr);
	m_pTextScoreNr->SetPosition(150, 110);

	m_pTextMenu = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textMenu = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	textMenu->SetText("Menu");
	m_pTextMenu->AddComponent(textMenu);
	Add(m_pTextMenu);
	m_pTextMenu->SetPosition(165, 300);

	m_pTextQuit = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textQuit = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	textQuit->SetText("Quit");
	m_pTextQuit->AddComponent(textQuit);
	Add(m_pTextQuit);
	m_pTextQuit->SetPosition(165, 350);
}

void GameOverScene::Update()
{
	if (InputManager::GetInstance().GetController()->IsPressed(ControllerButton::ButtonA))
	{
		if (!m_ButtonPressed)
		{
			m_ButtonPressed = true;

			if (m_ItemSelected == 0)
			{
				SceneManager::GetInstance().SetActiveScene("MenuScene");
			}
			if (m_ItemSelected == 1)
			{
				exit(0);
			}
		}
	}

	else if (InputManager::GetInstance().GetController()->IsPressed(ControllerButton::ButtonDown))
	{
		if (!m_ButtonPressed)
		{
			m_ButtonPressed = true;
			++m_ItemSelected;
			if (m_ItemSelected > 1) m_ItemSelected = 0;
		}
	}

	else if (InputManager::GetInstance().GetController()->IsPressed(ControllerButton::ButtonUp))
	{
		if (!m_ButtonPressed)
		{
			m_ButtonPressed = true;
			--m_ItemSelected;
			if (m_ItemSelected < 0) m_ItemSelected = 1;
		}
	}
	else m_ButtonPressed = false;

	if (m_ItemSelected == 0)
	{
		m_pTextMenu->GetComponent<TextComponent>()->SetColor(SDL_Color{ 215, 27, 27, 255 });
		m_pTextQuit->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
	}

	if (m_ItemSelected == 1)
	{
		m_pTextQuit->GetComponent<TextComponent>()->SetColor(SDL_Color{ 215, 27, 27, 255 });
		m_pTextMenu->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
	}

}

void GameOverScene::Reset()
{
	m_ItemSelected = 0;
	m_ButtonPressed = true;
	m_pTextScore->GetComponent<TextComponent>()->SetText("Score: ");
	m_pTextScoreNr->GetComponent<TextComponent>()->SetText(std::to_string(GameInfo::GetInstance().score));
}
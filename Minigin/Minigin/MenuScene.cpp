#include "MiniginPCH.h"
#include "MenuScene.h"
#include "TextComponent.h"
#include "SpriteComponent.h"
#include "InputManager.h"
#include "GameInfo.h"
#include "ScreenInfo.h"

MenuScene::MenuScene()
	: Scene("MenuScene")
{

}

void MenuScene::Initialize()
{
	std::shared_ptr<GameObject> pBackObj = std::make_shared<GameObject>();
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(1000.0f, 2000.0f, 1, 1);
	pBackObj->AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Background.png");
	pSpriteComp->IsStatic(true);
	pBackObj->m_Rect = { pBackObj->m_Rect.x, pBackObj->m_Rect.y, ScreenInfo::GetInstance().screenwidth, ScreenInfo::GetInstance().screenheigth };
	Add(pBackObj);
	//pBackObj->SetPosition(ScreenInfo::GetInstance().screenwidth / 2.0f - pLogoObj->m_Rect.w / 2.0f, 80);

	std::shared_ptr<GameObject> pLogoObj = std::make_shared<GameObject>();
	std::shared_ptr<SpriteComponent> pSpriteComp2 = std::make_shared<SpriteComponent>(2685.0f, 1389.0f, 1, 1);
	pLogoObj->AddComponent(pSpriteComp2);
	pSpriteComp2->SetTexture("Logo.png");
	pSpriteComp2->IsStatic(true);
	pLogoObj->m_Rect = { pLogoObj->m_Rect.x, pLogoObj->m_Rect.y, 260, 130 };
	Add(pLogoObj);
	pLogoObj->SetPosition(ScreenInfo::GetInstance().screenwidth/2.0f - pLogoObj->m_Rect.w/2.0f, 80);

	m_pTextP1 = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textP1 = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	textP1->SetText("1 player");
	m_pTextP1->AddComponent(textP1);
	Add(m_pTextP1);
	m_pTextP1->SetPosition(125, 250);

	m_pTextP2 = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textP2 = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	textP2->SetText("2 players");
	m_pTextP2->AddComponent(textP2);
	Add(m_pTextP2);
	m_pTextP2->SetPosition(115, 300);

	m_pTextQuit = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textQuit = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	textQuit->SetText("Quit");
	m_pTextQuit->AddComponent(textQuit);
	Add(m_pTextQuit);
	m_pTextQuit->SetPosition(165, 350);
}

void MenuScene::Update()
{
	if (InputManager::GetInstance().GetController()->IsPressed(ControllerButton::ButtonA))
	{
		if (!m_ButtonPressed)
		{
			m_ButtonPressed = true;

			if (m_ItemSelected == 0)
			{
				GameInfo::GetInstance().player2Active = false;
				SceneManager::GetInstance().SetActiveScene("GameScene1");
			}
			if (m_ItemSelected == 1)
			{
				GameInfo::GetInstance().player2Active = true;
				SceneManager::GetInstance().SetActiveScene("GameScene1");
			}
			if (m_ItemSelected == 2)
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
			if (m_ItemSelected > 2) m_ItemSelected = 0;
		}
	}

	else if (InputManager::GetInstance().GetController()->IsPressed(ControllerButton::ButtonUp))
	{
		if (!m_ButtonPressed)
		{
			m_ButtonPressed = true;
			--m_ItemSelected;
			if (m_ItemSelected < 0) m_ItemSelected = 2;
		}
	}
	else m_ButtonPressed = false;

	if (m_ItemSelected == 0)
	{
		m_pTextP1->GetComponent<TextComponent>()->SetColor(SDL_Color{ 215, 27, 27, 255 });
		m_pTextP2->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
		m_pTextQuit->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
	}

	if (m_ItemSelected == 1)
	{
		m_pTextP1->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
		m_pTextP2->GetComponent<TextComponent>()->SetColor(SDL_Color{ 215, 27, 27, 255 });
		m_pTextQuit->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
	}

	if (m_ItemSelected == 2)
	{
		m_pTextP1->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
		m_pTextP2->GetComponent<TextComponent>()->SetColor(SDL_Color{ 255, 255, 255, 255 });
		m_pTextQuit->GetComponent<TextComponent>()->SetColor(SDL_Color{ 215, 27, 27, 255 });
	}
}

void MenuScene::Reset()
{
	m_ItemSelected = 0;
	m_ButtonPressed = true;
}
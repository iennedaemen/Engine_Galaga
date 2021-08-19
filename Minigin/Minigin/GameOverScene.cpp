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

	Clear();
	Initialize();

	MakeScoreBoard(1);
	if(GameInfo::GetInstance().player2Active) 
		MakeScoreBoard(2);
}

void GameOverScene::MakeScoreBoard(int playerNr)
{

	std::shared_ptr<GameObject> m_pTextResult = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textResult = std::make_shared<TextComponent>("../Data/Pixel.otf", 20, SDL_Color{ 215, 27, 27, 255 });
	if (GameInfo::GetInstance().player2Active)
	{
		if (playerNr == 1)
			textResult->SetText("- Result player 1 -");
		else textResult->SetText("- Result player 2 -");
	}
	else textResult->SetText("- Result -");

	m_pTextResult->AddComponent(textResult);
	Add(m_pTextResult);
	if (playerNr == 1) m_pTextResult->SetPosition(60, 40);
	else m_pTextResult->SetPosition(60, 160);


	std::shared_ptr<GameObject> m_pTextScore = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textScore = std::make_shared<TextComponent>("../Data/Pixel.otf", 20);
	if(playerNr == 1)
	textScore->SetText("Score: " + std::to_string(GameInfo::GetInstance().scoreP1));
	else textScore->SetText("Score: " + std::to_string(GameInfo::GetInstance().scoreP2));
	m_pTextScore->AddComponent(textScore);
	Add(m_pTextScore);
	if (playerNr == 1) m_pTextScore->SetPosition(60, 60);
	else m_pTextScore->SetPosition(60, 180);

	std::shared_ptr<GameObject> m_pTextShots = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textShots = std::make_shared<TextComponent>("../Data/Pixel.otf", 20, SDL_Color{ 255, 205, 36, 255 });
	if (playerNr == 1)
		textShots->SetText("Shots fired: " + std::to_string(GameInfo::GetInstance().shotsFiredP1));
	else textShots->SetText("Shots fired: " + std::to_string(GameInfo::GetInstance().shotsFiredP2));
	m_pTextShots->AddComponent(textShots);
	Add(m_pTextShots);
	if (playerNr == 1) m_pTextShots->SetPosition(60, 80);
	else  m_pTextShots->SetPosition(60, 200);

	std::shared_ptr<GameObject> m_pTextHits = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textHits = std::make_shared<TextComponent>("../Data/Pixel.otf", 20, SDL_Color{ 255, 205, 36, 255 });
	if(playerNr == 1)
		textHits->SetText("Number of hits: " + std::to_string(GameInfo::GetInstance().shotsHitP1));
	else textHits->SetText("Number of hits: " + std::to_string(GameInfo::GetInstance().shotsHitP2));
	m_pTextHits->AddComponent(textHits);
	Add(m_pTextHits);
	if (playerNr == 1) m_pTextHits->SetPosition(60, 100);
	else m_pTextHits->SetPosition(60, 220);

	std::shared_ptr<GameObject> m_pTextHitsMiss = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textHitsMiss = std::make_shared<TextComponent>("../Data/Pixel.otf", 20, SDL_Color{ 255, 205, 36, 255 });
	int i{0};
	if (playerNr == 1)
	{
		if (GameInfo::GetInstance().shotsFiredP1 > 0)
			i = int((float(GameInfo::GetInstance().shotsHitP1) / float(GameInfo::GetInstance().shotsFiredP1)) * 100);
	}
	else if(GameInfo::GetInstance().shotsFiredP2 > 0)
		i = int((float(GameInfo::GetInstance().shotsHitP2) / float(GameInfo::GetInstance().shotsFiredP2)) * 100);
	textHitsMiss->SetText("Hit miss ratio: %" + std::to_string(i));
	m_pTextHitsMiss->AddComponent(textHitsMiss);
	Add(m_pTextHitsMiss);
	if (playerNr == 1) m_pTextHitsMiss->SetPosition(60, 120);
	else m_pTextHitsMiss->SetPosition(60, 240);
}
#include "MiniginPCH.h"
#include "GameScene.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FpsComponent.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "ColliderComponent.h"
#include "ScreenInfo.h"
#include "Laser.h"
#include "Zako.h"
#include "Goei.h"
#include "Boss.h"
#include "Time.h"
#include <algorithm>
#include <functional>
#include <fstream>
#include "json.hpp"
#include <string>
#include "SceneManager.h"
#include "GameInfo.h"
#include "Observer.h"
#include <sstream>
#include <iomanip>
#include "ScoreObserver.h"

using json = nlohmann::json;

GameScene::GameScene(int level)
	: Scene("GameScene" + std::to_string(level))
	, m_Level(level)
{}


void GameScene::Initialize()
{
	m_Observers.push_back(std::make_shared<ScoreObserver>());

	std::shared_ptr<GameObject> pBackObj = std::make_shared<GameObject>();
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(1000.0f, 2000.0f, 1, 1);
	pBackObj->AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Background.png");
	pSpriteComp->IsStatic(true);
	pBackObj->m_Rect = { pBackObj->m_Rect.x, pBackObj->m_Rect.y + 20, ScreenInfo::GetInstance().screenwidth, ScreenInfo::GetInstance().screenheigth };
	Add(pBackObj);

	m_pTextScoreP1 = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textScore1 = std::make_shared<TextComponent>("../Data/Pixel.otf", 15);
	std::stringstream ss;
	ss << std::setw(5) << std::setfill('0') << GameInfo::GetInstance().scoreP1;
	std::string s = ss.str();
	textScore1->SetText(s);
	m_pTextScoreP1->AddComponent(textScore1);
	Add(m_pTextScoreP1);
	m_pTextScoreP1->SetPosition(10, 1);

	m_pTextScoreP2 = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> textScore2 = std::make_shared<TextComponent>("../Data/Pixel.otf", 15, SDL_Color{ 215, 27, 27, 255 });
	if (GameInfo::GetInstance().player2Active)
	{
		std::stringstream ss2;
		ss2 << std::setw(5) << std::setfill('0') << GameInfo::GetInstance().scoreP2;
		s = ss2.str();
		textScore2->SetText(s);
	}
	else textScore2->SetText(" ");
	m_pTextScoreP2->AddComponent(textScore2);
	Add(m_pTextScoreP2);
	m_pTextScoreP2->SetPosition(100, 1);

	m_pLevelText = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> text = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	text->SetText("Level " + std::to_string(m_Level));
	m_pLevelText->AddComponent(text);
	Add(m_pLevelText);
	m_pLevelText->SetPosition(135, ScreenInfo::GetInstance().screenheigth/2.0f - 50);

	m_pPlayer = std::make_shared<Player>(1);
	Add(m_pPlayer);

	if (GameInfo::GetInstance().player2Active)
	{
		m_pPlayer2 = std::make_shared<Player>(2);
		Add(m_pPlayer2);
		m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer->m_Rect.w / 2.0f - 20), float(ScreenInfo::GetInstance().screenheigth - 75));
		m_pPlayer2->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer2->m_Rect.w / 2.0f + 20), float(ScreenInfo::GetInstance().screenheigth - 75));
	}
	else m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer->m_Rect.w / 2.0f), float(ScreenInfo::GetInstance().screenheigth - 75));

	for (int i{}; i < 2; ++i)
	{
		for (int j{}; j < 8; ++j)
		{
			m_ZakoPositions.push_back(glm::vec2{ 40.0f * (j + 1), 25 * i + 150 });
			m_GoeiPositions.push_back(glm::vec2{ 40.0f * (j + 1), 25 * i + 90 });

		}
	}

	for (int j{}; j < 8; ++j)
	{
		m_BossPositions.push_back(glm::vec2{ 40.0f * (j + 1), 25 + 20 });
	}

	ReadFile();
}

void GameScene::Update()
{
	if (m_IsBegin)
	{
		m_BeginTimer += Time::GetInstance().m_ElapsedSec;
		if (m_BeginTimer >= m_BeginTime)
		{
			m_IsBegin = false;
			m_pLevelText->GetComponent<TextComponent>()->SetText(" ");
		}
	}

	std::stringstream ss;
	ss << std::setw(5) << std::setfill('0') << GameInfo::GetInstance().scoreP1;
	std::string s = ss.str();
	m_pTextScoreP1->GetComponent<TextComponent>()->SetText(s);

	if (GameInfo::GetInstance().player2Active)
	{
		std::stringstream ss2;
		ss2 << std::setw(5) << std::setfill('0') << GameInfo::GetInstance().scoreP2;
		s = ss2.str();
		m_pTextScoreP2->GetComponent<TextComponent>()->SetText(s);
	}

	m_SpawnTimer += Time::GetInstance().m_ElapsedSec;

	SpawnZako();
	UpdateZako();

	SpawnGoei();
	UpdateGoei();

	SpawnBoss();
	UpdateBoss();

	UpdatePlayer(m_pPlayer);
	if(GameInfo::GetInstance().player2Active)
		UpdatePlayer(m_pPlayer2);


		if ((GetAsyncKeyState('P') & 0x8000) && (k == 0)) 
		{ 
			k = 1; 
			Reset();
			if (m_Level < 2)
				SceneManager::GetInstance().SetActiveScene("GameScene" + std::to_string(m_Level + 1));
			else SceneManager::GetInstance().SetActiveScene("GameScene" + std::to_string(1));
		}
		else if (GetAsyncKeyState('P') == 0) k = 0;
	

	if (m_EnemiesDead >= m_AmountZako + m_AmountGoei + m_AmountBoss)
	{
		if(m_Level < 2)
			SceneManager::GetInstance().SetActiveScene("GameScene" + std::to_string(m_Level + 1));
		else SceneManager::GetInstance().SetActiveScene("GameOverScene");

	}

	if (GameInfo::GetInstance().player2Active)
	{
		std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);

		if(dPlayer->GetIsDead() && dPlayer2->GetIsDead())
			SceneManager::GetInstance().SetActiveScene("GameOverScene");
	}
	else
	{
		std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);

		if (dPlayer->GetIsDead())
			SceneManager::GetInstance().SetActiveScene("GameOverScene");
	}
}

void GameScene::Render() const
{

}

void GameScene::Reset()
{
	//std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
	if (m_Level == 1)
	{
		GameInfo::GetInstance().player1Lives = 3;
		GameInfo::GetInstance().player2Lives = 3;
	}

	Remove(m_pPlayer);
	m_pPlayer = nullptr;
	m_pPlayer = std::make_shared<Player>(1);
	Add(m_pPlayer);
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
	dPlayer->SetLives(GameInfo::GetInstance().player1Lives);

	Remove(m_pPlayer2);
	m_pPlayer2 = nullptr;

	if (GameInfo::GetInstance().player2Active)
	{
		m_pPlayer2 = std::make_shared<Player>(2);
		Add(m_pPlayer2);
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
		dPlayer2->SetLives(GameInfo::GetInstance().player2Lives);
		m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer->m_Rect.w / 2.0f - 20), float(ScreenInfo::GetInstance().screenheigth - 75));
		m_pPlayer2->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer2->m_Rect.w / 2.0f + 20), float(ScreenInfo::GetInstance().screenheigth - 75));
	}
	else m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer->m_Rect.w / 2.0f), float(ScreenInfo::GetInstance().screenheigth - 75));


	m_BeginTimer = 0.0f;
	m_IsBegin = true;
	m_pLevelText->GetComponent<TextComponent>()->SetText("Level " + std::to_string(m_Level));
	
	m_SpawnTimer = 0;
	m_EnemiesDead = 0;

	if (GameInfo::GetInstance().player2Active)
	{
		std::stringstream ss;
		ss << std::setw(5) << std::setfill('0') << GameInfo::GetInstance().scoreP2;
		std::string s = ss.str();
		m_pTextScoreP2->GetComponent<TextComponent>()->SetText(s);
	}
	else m_pTextScoreP2->GetComponent<TextComponent>()->SetText(" ");

	// ZAKO
	for (int i{}; i < m_pZakos.size(); ++i)
	{
		Remove(m_pZakos[i]);
	}
	m_pZakos.clear();
	m_NrActiveZako = 0;
	m_SpawnAmountZako = 4;
	m_SpawnLeftZako = false;

	// GOEI
	for (int i{}; i < m_pGoeis.size(); ++i)
	{
		Remove(m_pGoeis[i]);
	}
	m_pGoeis.clear();
	m_NrActiveGoei = 0;
	m_SpawnAmountGoei = 0;
	m_SpawnLeftGoei = false;

	// BOSS
	for (int i{}; i < m_pBosses.size(); ++i)
	{
		Remove(m_pBosses[i]);
	}
	m_pBosses.clear();
	m_NrActiveBoss = 0;
	m_SpawnAmountBoss = 0;


	// READ
	m_AmountZako = 0;
	while(!m_ZakoTimes.empty())
		m_ZakoTimes.pop();
	while (!m_ZakoPos.empty())
		m_ZakoPos.pop();

	m_AmountGoei = 0;
	while (!m_GoeiTimes.empty())
		m_GoeiTimes.pop();
	while (!m_GoeiPos.empty())
		m_GoeiPos.pop();

	m_AmountBoss = 0;
	while (!m_BossTimes.empty())
		m_BossTimes.pop();
	while (!m_BossPos.empty())
		m_BossPos.pop();

	ReadFile();
}

void GameScene::UpdatePlayer(std::shared_ptr<GameObject> pPlayer)
{
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (pPlayer);
	if (!dPlayer->GetIsHit() && !dPlayer->IsAbducted())
	{
		// ZAKOS
		for (int i{}; i < m_pZakos.size(); ++i)
		{
			std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);
			if (zako->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (zako->GetLaser());
				dLaser->SetActive(false);

			}
			if (dPlayer->GetComponent<ColliderComponent>()->IsColliding(zako->m_Rect))
			{
				dPlayer->SetIsHit(true);
				zako->m_IsHit = true;
			}
		}

		// GOEIS
		for (int i{}; i < m_pGoeis.size(); ++i)
		{
			std::shared_ptr<Goei> goei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
			if (goei->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (goei->GetLaser());
				dLaser->SetActive(false);

			}
		}
		// BOSSES
		for (int i{}; i < m_pBosses.size(); ++i)
		{
			std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
			if (boss->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (boss->GetLaser());
				dLaser->SetActive(false);
			}
		}
	}

	if (dPlayer->GetIsDead())
	{
		Remove(pPlayer);
	}

	if (!dPlayer->IsExploding())
	{
		for (int i{}; i < m_pBosses.size(); ++i)
		{
			std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
			if (dBoss->GetBeam()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetAbducted(true, m_pBosses[i]);
			}
		}
	}

}

void GameScene::SpawnZako()
{
	if (m_NrActiveZako >= m_AmountZako)
		return;

	if (m_SpawnTimer >= m_ZakoTimes.front())
	{
			float time = m_ZakoTimes.front();
			m_ZakoTimes.pop();
			m_NrActiveZako++;

			if (!m_ZakoTimes.empty())
			{
				if (m_ZakoTimes.front() - time > 0.2f)
				{
					int r = std::rand() % 2;
					if (r == 0) m_SpawnLeftZako = true;
					else m_SpawnLeftZako = false;
				}
			}

			std::shared_ptr<GameObject> zakoObj = std::make_shared<Zako>(m_ZakoPositions[m_ZakoPos.front()]);
			Add(zakoObj);
			m_pZakos.push_back(zakoObj);
			std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako>(zakoObj);

			m_ZakoPos.pop();

			if (m_SpawnLeftZako)
			{
				zakoObj->SetPosition(-20, 400);
				zako->m_SpawnedLeft = true;
			}
			else
			{
				zakoObj->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 400.0f);
				zako->m_SpawnedLeft = false;
			}
	}
}

void GameScene::UpdateZako()
{
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
	std::vector<std::shared_ptr<GameObject>> pLasers;
	pLasers.push_back(dPlayer->GetLaser(0));
	pLasers.push_back(dPlayer->GetLaser(1));

	if (GameInfo::GetInstance().player2Active)
	{
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
		pLasers.push_back(dPlayer2->GetLaser(0));
		pLasers.push_back(dPlayer2->GetLaser(1));
	}

	// UPDATE DATA
	for (int i{}; i < m_pZakos.size(); ++i)
	{
		std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);

		int r = std::rand() % 2;
		if (r % 2 == 0)
			zako->SetNextAction(true);
		else zako->SetNextAction(false);

		zako->m_PlayerPos = { m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y };
		if (GameInfo::GetInstance().player2Active)
			zako->m_Player2Pos = { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y };
	}

	// COLLISION
	for (int i{}; i < pLasers.size(); ++i)
	{
		for (int j{}; j < m_pZakos.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(m_pZakos[j]->m_Rect))
			{
				std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako> (m_pZakos[j]);
				if (!zako->m_IsHit)
				{
					auto parent = pLasers[i]->GetParent();
					for (std::shared_ptr<Observer> observer : m_Observers)
					{
						observer->onNotify(Event::ZakoHit, m_pZakos[j], parent);
					}
					zako->m_IsHit = true;
					std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(parent);
					p->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (int i{}; i < m_pZakos.size(); ++i)
	{
		std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);
		if (zako->m_IsDead)
		{
			m_EnemiesDead++;
			Remove(m_pZakos[i]);
			idxRemove.push_back(i);
		}
	}

	if (!idxRemove.empty())
	{
		std::sort(idxRemove.begin(), idxRemove.end(), std::greater<int>());
		for (int index : idxRemove)
		{
			std::swap(m_pZakos[index], m_pZakos.back());
			m_pZakos.pop_back();
		}
	}
	idxRemove.clear();
}

void GameScene::SpawnGoei()
{
	if (m_NrActiveGoei >= m_AmountGoei)
		return;

	if (m_SpawnTimer >= m_GoeiTimes.front())
	{
		float time = m_GoeiTimes.front();
		m_GoeiTimes.pop();
		m_NrActiveGoei++;

		if (!m_GoeiTimes.empty())
		{
			if (m_GoeiTimes.front() - time > 0.5f)
			{
				int r = std::rand() % 2;
				if (r == 0) m_SpawnLeftGoei = true;
				else m_SpawnLeftGoei = false;
			}
		}

		std::shared_ptr<GameObject> goeiObj = std::make_shared<Goei>(m_GoeiPositions[m_GoeiPos.front()]);
		Add(goeiObj);
		m_pGoeis.push_back(goeiObj);
		std::shared_ptr<Goei> goei = std::dynamic_pointer_cast<Goei>(goeiObj);

		m_GoeiPos.pop();

		if (m_SpawnLeftGoei)
		{
			goeiObj->SetPosition(-20, 400);
			goei->m_SpawnedLeft = true;
		}
		else
		{
			goeiObj->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 400.0f);
			goei->m_SpawnedLeft = false;
		}
	}
}

void GameScene::UpdateGoei()
{
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
	std::vector<std::shared_ptr<GameObject>> pLasers;
	pLasers.push_back(dPlayer->GetLaser(0));
	pLasers.push_back(dPlayer->GetLaser(1));

	if (GameInfo::GetInstance().player2Active)
	{
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
		pLasers.push_back(dPlayer2->GetLaser(0));
		pLasers.push_back(dPlayer2->GetLaser(1));
	}

	// UPDATE DATA
	for (int i{}; i < m_pGoeis.size(); ++i)
	{
		std::shared_ptr<Goei> goei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
		
		goei->m_PlayerPos = { m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y };
		if(GameInfo::GetInstance().player2Active)
			goei->m_Player2Pos = { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y };
	}

	// COLLISION
	for (int i{}; i < pLasers.size(); ++i)
	{
		for (int j{}; j < m_pGoeis.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(m_pGoeis[j]->m_Rect))
			{
				std::shared_ptr<Goei> goei = std::dynamic_pointer_cast<Goei> (m_pGoeis[j]);
				if (!goei->m_IsHit)
				{
					auto parent = pLasers[i]->GetParent();
					for (std::shared_ptr<Observer> observer : m_Observers)
					{
						observer->onNotify(Event::GoeiHit, m_pGoeis[j], parent);
					}

					goei->m_IsHit = true;
					std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(parent);
					p->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (int i{}; i < m_pGoeis.size(); ++i)
	{
		std::shared_ptr<Goei> eGoei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
		if (eGoei->m_IsDead)
		{
			m_EnemiesDead++;
			Remove(m_pGoeis[i]);
			idxRemove.push_back(i);
		}
	}

	if (!idxRemove.empty())
	{
		std::sort(idxRemove.begin(), idxRemove.end(), std::greater<int>());
		for (int index : idxRemove)
		{
			std::swap(m_pGoeis[index], m_pGoeis.back());
			m_pGoeis.pop_back();
		}
	}
	idxRemove.clear();
}

void GameScene::SpawnBoss()
{
	if (m_NrActiveBoss >= m_AmountBoss)
		return;

	if (m_SpawnTimer >= m_BossTimes.front())
	{
		m_BossTimes.pop();
		m_NrActiveBoss++;

		std::shared_ptr<GameObject> bossObj = std::make_shared<Boss>(m_BossPositions[m_BossPos.front()]);
		Add(bossObj);
		m_pBosses.push_back(bossObj);
		std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss>(bossObj);

		m_BossPos.pop();

		bossObj->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2), -20);


		int r = std::rand() % 2;
		if (r == 0) boss->m_SpawnedLeft = true;
		else boss->m_SpawnedLeft = false;
	}
}

void GameScene::UpdateBoss()
{
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
	std::vector<std::shared_ptr<GameObject>> pLasers;
	pLasers.push_back(dPlayer->GetLaser(0));
	pLasers.push_back(dPlayer->GetLaser(1));

	if (GameInfo::GetInstance().player2Active)
	{
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
		pLasers.push_back(dPlayer2->GetLaser(0));
		pLasers.push_back(dPlayer2->GetLaser(1));
	}

	// UPDATE DATA
	for (int i{}; i < m_pBosses.size(); ++i)
	{
		std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);

		int r = std::rand() % 2;
		if (r % 2 == 0)
			boss->SetNextAction(true);
		else boss->SetNextAction(false);

		boss->m_PlayerPos = { m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y };
		if (GameInfo::GetInstance().player2Active)
			boss->m_Player2Pos = { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y };
	}

	// COLLISION
	for (int i{}; i < pLasers.size(); ++i)
	{
		for (int j{}; j < m_pBosses.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(m_pBosses[j]->m_Rect))
			{
				std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss> (m_pBosses[j]);
				if (!boss->m_IsHit)
				{
					auto parent = pLasers[i]->GetParent();
					for (std::shared_ptr<Observer> observer : m_Observers)
					{
						observer->onNotify(Event::BossHit, m_pBosses[j], parent);
					}

					boss->m_IsHit = true;
					std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(parent);
					p->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (int i{}; i < m_pBosses.size(); ++i)
	{
		std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
		if (boss->m_IsDead)
		{
			m_EnemiesDead++;
			Remove(boss);
			idxRemove.push_back(i);
		}
	}

	if (!idxRemove.empty())
	{
		std::sort(idxRemove.begin(), idxRemove.end(), std::greater<int>());
		for (int index : idxRemove)
		{
			std::swap(m_pBosses[index], m_pBosses.back());
			m_pBosses.pop_back();
		}
	}
	idxRemove.clear();
}

void GameScene::ReadFile()
{
	std::ifstream stream("../Data/Levels.json");
	if (stream)
	{
		json jf = json::parse(stream);

		std::string level = std::to_string(m_Level);

		// ZAKO
		m_AmountZako = jf[level]["AmountZako"];

		for (int i{}; i < jf[level]["ZakoTimes"].size(); ++i)
		{
			m_ZakoTimes.push(jf[level]["ZakoTimes"][i]);
		}

		for (int i{}; i < jf[level]["ZakoPositions"].size(); ++i)
		{
			m_ZakoPos.push(jf[level]["ZakoPositions"][i]);
		}


		// GOEI
		m_AmountGoei = jf[level]["AmountGoei"];

		for (int i{}; i < jf[level]["GoeiTimes"].size(); ++i)
		{
			m_GoeiTimes.push(jf[level]["GoeiTimes"][i]);
		}

		for (int i{}; i < jf[level]["GoeiPositions"].size(); ++i)
		{
			m_GoeiPos.push(jf[level]["GoeiPositions"][i]);
		}


		// BOSS
		m_AmountBoss = jf[level]["AmountBoss"];

		for (int i{}; i < jf[level]["BossTimes"].size(); ++i)
		{
			m_BossTimes.push(jf[level]["BossTimes"][i]);
		}

		for (int i{}; i < jf[level]["BossPositions"].size(); ++i)
		{
			m_BossPos.push(jf[level]["BossPositions"][i]);
		}
	}
}
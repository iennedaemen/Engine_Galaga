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

using json = nlohmann::json;

GameScene::GameScene(int level)
	: Scene("GameScene" + std::to_string(level))
	, m_Level(level)
{}


void GameScene::Initialize()
{
	std::shared_ptr<GameObject> pBackObj = std::make_shared<GameObject>();
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(1000.0f, 2000.0f, 1, 1);
	pBackObj->AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Background.png");
	pSpriteComp->IsStatic(true);
	pBackObj->m_Rect = { pBackObj->m_Rect.x, pBackObj->m_Rect.y, ScreenInfo::GetInstance().screenwidth, ScreenInfo::GetInstance().screenheigth };
	Add(pBackObj);

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
	//if (dPlayer->GetIsDead()) Add(m_pPlayer);
	//m_pPlayer->GetComponent<SpriteComponent>()->SetTexture("Player1.png", 60.0f, 64.0f, 1, 1);
	//m_pPlayer->GetComponent<SpriteComponent>()->SetSpriteSheetTopLeft(0, 0);
	//m_pPlayer->GetComponent<SpriteComponent>()->IsStatic(true);
	//m_pPlayer->GetComponent<SpriteComponent>()->SetCurrentFrame(0);
	//dPlayer->SetLives(GameInfo::GetInstance().player1Lives);
	//dPlayer->SetIsDead(false);
	//dPlayer->SetExploding(false);
	//dPlayer->SetIsHit(false);

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
			std::shared_ptr<Zako> dZakos = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);
			if (dZakos->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (dZakos->GetLaser());
				dLaser->SetActive(false);

			}
			if (dPlayer->GetComponent<ColliderComponent>()->IsColliding(dZakos->m_Rect))
			{
				dPlayer->SetIsHit(true);
				dZakos->SetIsHit(true);
			}
		}

		// GOEIS
		for (int i{}; i < m_pGoeis.size(); ++i)
		{
			std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
			if (dGoei->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (dGoei->GetLaser());
				dLaser->SetActive(false);

			}
		}
		// BOSSES
		for (int i{}; i < m_pBosses.size(); ++i)
		{
			std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
			if (dBoss->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (dBoss->GetLaser());
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
				if (m_ZakoTimes.front() - time > 0.5f)
				{
					int r = std::rand() % 2;
					if (r == 0) m_SpawnLeftZako = true;
					else m_SpawnLeftZako = false;
				}
			}

			std::shared_ptr<GameObject> zako = std::make_shared<Zako>(m_ZakoPositions[m_ZakoPos.front()]);
			Add(zako);
			m_pZakos.push_back(zako);
			std::shared_ptr<Zako> dZako = std::dynamic_pointer_cast<Zako>(zako);

			m_ZakoPos.pop();

			if (m_SpawnLeftZako)
			{
				zako->SetPosition(-20, 400);
				dZako->SetSpawnedLeft(true);
			}
			else
			{
				zako->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 400.0f);
				dZako->SetSpawnedLeft(false);
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
		std::shared_ptr<Zako> dZako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);

		int r = std::rand() % 2;
		if (r % 2 == 0)
			dZako->SetNextAction(true);
		else dZako->SetNextAction(false);

		if(GameInfo::GetInstance().player2Active)
			dZako->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y }, { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y });
		else dZako->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y });
	}

	// COLLISION
	for (int i{}; i < pLasers.size(); ++i)
	{
		for (int j{}; j < m_pZakos.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(m_pZakos[j]->m_Rect))
			{
				std::shared_ptr<Zako> dZako = std::dynamic_pointer_cast<Zako> (m_pZakos[j]);
				if (!dZako->GetIsHit())
				{
					dZako->SetIsHit(true);
					dPlayer->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
					if (GameInfo::GetInstance().player2Active)
					{
						std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
						dPlayer2->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
					}
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (int i{}; i < m_pZakos.size(); ++i)
	{
		std::shared_ptr<Zako> dZako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);
		if (dZako->GetIsDead())
		{
			m_EnemiesDead++;
			Remove(dZako);
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

		std::shared_ptr<GameObject> goei = std::make_shared<Goei>(m_GoeiPositions[m_GoeiPos.front()]);
		Add(goei);
		m_pGoeis.push_back(goei);
		std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei>(goei);

		m_GoeiPos.pop();

		if (m_SpawnLeftGoei)
		{
			goei->SetPosition(-20, 400);
			dGoei->SetSpawnedLeft(true);
		}
		else
		{
			goei->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 400.0f);
			dGoei->SetSpawnedLeft(false);
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
		std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
		if(GameInfo::GetInstance().player2Active)
			dGoei->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y }, { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y });
		else dGoei->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y });
	}

	// COLLISION
	for (int i{}; i < pLasers.size(); ++i)
	{
		for (int j{}; j < m_pGoeis.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(m_pGoeis[j]->m_Rect))
			{
				std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei> (m_pGoeis[j]);
				if (!dGoei->GetIsHit())
				{
					dGoei->SetIsHit(true);
					dPlayer->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
					if (GameInfo::GetInstance().player2Active)
					{
						std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
						dPlayer2->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
					}
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (int i{}; i < m_pGoeis.size(); ++i)
	{
		std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
		if (dGoei->GetIsDead())
		{
			m_EnemiesDead++;
			Remove(dGoei);
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

		std::shared_ptr<GameObject> boss = std::make_shared<Boss>(m_BossPositions[m_BossPos.front()]);
		Add(boss);
		m_pBosses.push_back(boss);
		std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss>(boss);

		m_BossPos.pop();

		boss->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2), -20);


		int r = std::rand() % 2;
		if (r == 0) dBoss->SetSpawnedLeft(true);
		else dBoss->SetSpawnedLeft(false);
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
		std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);

		int r = std::rand() % 2;
		if (r % 2 == 0)
			dBoss->SetNextAction(true);
		else dBoss->SetNextAction(false);

		if (GameInfo::GetInstance().player2Active)
			dBoss->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y }, { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y });
		else dBoss->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y });
	}

	// COLLISION
	for (int i{}; i < pLasers.size(); ++i)
	{
		for (int j{}; j < m_pBosses.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(m_pBosses[j]->m_Rect))
			{
				std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[j]);
				if (!dBoss->GetIsHit())
				{
					dBoss->SetIsHit(true);
					dPlayer->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
					if (GameInfo::GetInstance().player2Active)
					{
						std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
						dPlayer2->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
					}
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (int i{}; i < m_pBosses.size(); ++i)
	{
		std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
		if (dBoss->GetIsDead())
		{
			m_EnemiesDead++;
			Remove(dBoss);
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
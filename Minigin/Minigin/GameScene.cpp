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

using json = nlohmann::json;

GameScene::GameScene(int level)
	: Scene("GameScene")
	, m_Level(level)
{}


void GameScene::Initialize()
{
	m_pPlayer = std::make_shared<Player>(1);
	Add(m_pPlayer);
	m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f), float(ScreenInfo::GetInstance().screenheigth - 75));


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
	m_SpawnTimer += Time::GetInstance().m_ElapsedSec;

	SpawnZako();
	UpdateZako();

	SpawnGoei();
	UpdateGoei();

	SpawnBoss();
	UpdateBoss();

	UpdatePlayer();
}

void GameScene::Render() const
{

}

void GameScene::UpdatePlayer()
{
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
	if (!dPlayer->GetIsHit() && !dPlayer->IsAbducted())
	{
		// ZAKOS
		for (int i{}; i < m_pZakos.size(); ++i)
		{
			std::shared_ptr<Zako> dZakos = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);
			if (dZakos->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(m_pPlayer->m_Rect))
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
			if (dGoei->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(m_pPlayer->m_Rect))
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
			if (dBoss->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(m_pPlayer->m_Rect))
			{
				dPlayer->SetIsHit(true);

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (dBoss->GetLaser());
				dLaser->SetActive(false);
			}
		}
	}

	if (dPlayer->GetIsDead())
	{
		Remove(m_pPlayer);
	}


	for (int i{}; i < m_pBosses.size(); ++i)
	{
		std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
		if (dBoss->GetBeam()->GetComponent<ColliderComponent>()->IsColliding(m_pPlayer->m_Rect))
		{
			dPlayer->SetAbducted(true, m_pBosses[i]);
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
	std::shared_ptr<GameObject> pLasers[2];
	pLasers[0] = dPlayer->GetLaser(0);
	pLasers[1] = dPlayer->GetLaser(1);

	// UPDATE DATA
	for (int i{}; i < m_pZakos.size(); ++i)
	{
		std::shared_ptr<Zako> dZako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);

		int r = std::rand() % 2;
		if (r % 2 == 0)
			dZako->SetNextAction(true);
		else dZako->SetNextAction(false);

		dZako->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y });
	}

	// COLLISION
	for (int i{}; i < 2; ++i)
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
	std::shared_ptr<GameObject> pLasers[2];
	pLasers[0] = dPlayer->GetLaser(0);
	pLasers[1] = dPlayer->GetLaser(1);

	// UPDATE DATA
	for (int i{}; i < m_pGoeis.size(); ++i)
	{
		std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
		dGoei->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y });
	}

	// COLLISION
	for (int i{}; i < 2; ++i)
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
	std::shared_ptr<GameObject> pLasers[2];
	pLasers[0] = dPlayer->GetLaser(0);
	pLasers[1] = dPlayer->GetLaser(1);

	// UPDATE DATA
	for (int i{}; i < m_pBosses.size(); ++i)
	{
		std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);

		int r = std::rand() % 2;
		if (r % 2 == 0)
			dBoss->SetNextAction(true);
		else dBoss->SetNextAction(false);

		dBoss->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y });
	}

	// COLLISION
	for (int i{}; i < 2; ++i)
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
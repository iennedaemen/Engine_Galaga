#include "MiniginPCH.h"
#include "TestScene.h"
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

TestScene::TestScene()
	: Scene("TestScene")
{
	
}


void TestScene::Initialize()
{

	m_pPlayer = std::make_shared<Player>(1);
	Add(m_pPlayer);
	m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f), float(ScreenInfo::GetInstance().screenheigth - 75));

	//m_pPlayer2 = std::make_shared<Player>(2);
	//Add(m_pPlayer2);
	//m_pPlayer2->SetPosition(0, float(ScreenInfo::GetInstance().screenheigth - 175));

	for (int i{}; i < 2; ++i)
	{
		for (int j{}; j < 8; ++j)
		{
			m_ZakoPositions.push_back(std::pair<glm::vec2, bool>({ 40.0f * (j + 1), 25 * i + 150}, false));
			m_GoeiPositions.push_back(std::pair<glm::vec2, bool>({ 40.0f * (j + 1), 25 * i + 90 }, false));

		}
	}

		for (int j{}; j < 8; ++j)
		{
			m_BossPositions.push_back(std::pair<glm::vec2, bool>({ 40.0f * (j + 1), 25 + 20 }, false));
		}
	


}

void TestScene::Update()
{
	UpdateZako();
	UpdateGoei();
	UpdateBoss();

	UpdatePlayer();

	if (m_NrActiveZeko < 16 || m_NrActiveGoei < 16 || m_NrActiveBoss < 8)
	{
		m_SpawnTimer += Time::GetInstance().m_ElapsedSec;

		if (m_SpawnTimer >= m_SpawnTime)
		{
			if (m_SpawnNr == 0)
			{
				SpawnZako(m_SpawnLeftZeko);
				m_SpawnAmountZeko--;

				if (m_SpawnAmountZeko > 0)
					m_SpawnTime = 0.05f;
				else
				{
					m_SpawnTime = float(std::rand() % 5 + 5);
					int r = std::rand() % 2;
					if (r % 2 == 0)
						m_SpawnLeftZeko = true;
					else m_SpawnLeftZeko = false;
				}

				m_SpawnTimer = 0;
			}
			else if (m_SpawnNr == 1)
			{
				SpawnGoei(m_SpawnLeftGoei);
				m_SpawnAmountGoei--;

				if (m_SpawnAmountGoei > 0)
					m_SpawnTime = 0.05f;
				else
				{
					m_SpawnTime = float(std::rand() % 5 + 5);
					int r = std::rand() % 3;
					if (r % 2 == 0)
						m_SpawnLeftGoei = true;
					else m_SpawnLeftGoei = false;
				}

				m_SpawnTimer = 0;

			}

			else if (m_SpawnNr == 2)
			{
				SpawnBoss();
				m_SpawnAmountBoss--;

				if (m_SpawnAmountBoss > 0)
					m_SpawnTime = 0.05f;
				else
				{
					m_SpawnTime = float(std::rand() % 5 + 5);
				}

				m_SpawnTimer = 0;

			}

			if (m_SpawnAmountZeko <= 0 && m_SpawnAmountGoei <= 0 && m_SpawnAmountBoss <= 0)
			{
				m_SpawnNr = std::rand() % 3;
		
				if (m_SpawnNr == 0 && m_NrActiveZeko < 16)
					m_SpawnAmountZeko = std::rand() % 5 + 1;
				else if(m_NrActiveZeko >= 16) 
					m_SpawnNr++;

				if (m_SpawnNr == 1 && m_NrActiveGoei < 16)
					m_SpawnAmountGoei = std::rand() % 5 + 1;
				else if (m_NrActiveGoei >=  16)
					m_SpawnNr++;

				if (m_SpawnNr == 2 && m_NrActiveBoss < 8)
					m_SpawnAmountBoss = std::rand() % 1 + 1;
				else if (m_NrActiveBoss >= 8)
					m_SpawnNr = 0;

				m_SpawnTime = float(std::rand() % 5 + 5);
				m_SpawnTimer = 0;
			}
		}
	}
	else m_SpawnTimer = 0.0f;
}

void TestScene::Render() const
{
	
}

void TestScene::UpdatePlayer()
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

void TestScene::SpawnZako(bool SpawnLeft)
{

	if (m_NrActiveZeko + 1 > 16)
		return;

	if (m_NrActiveZeko < 16)
	{
		m_NrActiveZeko++;

		int r{};


			do r = std::rand() % 16;
			while (m_ZakoPositions[r].second);

			m_ZakoPositions[r].second = true;
			std::shared_ptr<GameObject> zako = std::make_shared<Zako>(m_ZakoPositions[r].first);
			Add(zako);
			m_pZakos.push_back(zako);
			std::shared_ptr<Zako> dZako = std::dynamic_pointer_cast<Zako>(zako);

			if (SpawnLeft)
			{
				zako->SetPosition(-20, 400);
				dZako->SetSpawnedLeft(true);
			}
			else
			{
				zako->SetPosition(float(ScreenInfo::GetInstance().screenwidth-20), 400.0f);
				dZako->SetSpawnedLeft(false);
			}
		
	}
}

void TestScene::UpdateZako()
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

		dZako->SetPlayerPos({ m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w/2,  m_pPlayer->m_Rect.y });
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
			auto it = std::find_if(m_ZakoPositions.begin(), m_ZakoPositions.end(),
				[dZako](const std::pair<glm::vec2, bool>& element) { return element.first == dZako->GetIdlePos(); });
			if (it != m_ZakoPositions.end())
			{
				it->second = false;
			}
			
			Remove(dZako);
			idxRemove.push_back(i);
			--m_NrActiveZeko;
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

void TestScene::SpawnGoei(bool SpawnLeft)
{
	if (m_NrActiveGoei + 1 > 16)
		return;

	if (m_NrActiveGoei < 16)
	{
		m_NrActiveGoei++;

		int r{};


		do r = std::rand() % 16;
		while (m_GoeiPositions[r].second);

		m_GoeiPositions[r].second = true;
		std::shared_ptr<GameObject> goei = std::make_shared<Goei>(m_GoeiPositions[r].first);
		Add(goei);
		m_pGoeis.push_back(goei);
		std::shared_ptr<Goei> dGoei = std::dynamic_pointer_cast<Goei>(goei);

		if (SpawnLeft)
		{
			goei->SetPosition(-20, 350);
			dGoei->SetSpawnedLeft(true);
		}
		else
		{
			goei->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 350.0f);
			dGoei->SetSpawnedLeft(false);
		}

	}
}

void TestScene::UpdateGoei()
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
			auto it = std::find_if(m_GoeiPositions.begin(), m_GoeiPositions.end(),
				[dGoei](const std::pair<glm::vec2, bool>& element) { return element.first == dGoei->GetIdlePos(); });
			if (it != m_GoeiPositions.end())
			{
				it->second = false;
			}

			Remove(dGoei);
			idxRemove.push_back(i);
			--m_NrActiveGoei;
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

void TestScene::SpawnBoss()
{
	if (m_NrActiveBoss + 1 > 8)
		return;

	if (m_NrActiveBoss < 8)
	{
		m_NrActiveGoei++;

		int r{};
		do r = std::rand() % 8;
		while (m_BossPositions[r].second);

		m_BossPositions[r].second = true;
		std::shared_ptr<GameObject> boss = std::make_shared<Boss>(m_BossPositions[r].first);
		Add(boss);
		m_pBosses.push_back(boss);
		std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss>(boss);


			boss->SetPosition(float(ScreenInfo::GetInstance().screenwidth/2), -20);
			dBoss->SetSpawnedLeft(false);
		

	}
}

void TestScene::UpdateBoss()
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
			auto it = std::find_if(m_BossPositions.begin(), m_BossPositions.end(),
				[dBoss](const std::pair<glm::vec2, bool>& element) { return element.first == dBoss->GetIdlePos(); });
			if (it != m_BossPositions.end())
			{
				it->second = false;
			}

			Remove(dBoss);
			idxRemove.push_back(i);
			--m_NrActiveGoei;
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
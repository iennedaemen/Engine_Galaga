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
#include "Time.h"

TestScene::TestScene()
	: Scene("TestScene")
{
	
}


void TestScene::Initialize()
{

	m_pPlayer = std::make_shared<Player>(1);
	Add(m_pPlayer);
	m_pPlayer->SetPosition(0, float(ScreenInfo::GetInstance().screenheigth - 75));

	//m_pPlayer2 = std::make_shared<Player>(2);
	//Add(m_pPlayer2);
	//m_pPlayer2->SetPosition(0, float(ScreenInfo::GetInstance().screenheigth - 175));

	for (int i{}; i < 2; ++i)
	{
		for (int j{}; j < 8; ++j)
		{
			m_ZakoPositions.push_back(std::pair<glm::vec2, bool>({ 40.0f * (j + 1), 30 * i + 100}, false));
		}
	}
}

void TestScene::Update()
{
	UpdateZako();

	if (m_NrActiveZeko < 16)
	{
		m_SpawnTimer += Time::GetInstance().m_ElapsedSec;

		if (m_SpawnTimer >= m_SpawnTime)
		{
			SpawnZako(m_SpawnLeftZeko);

			m_SpawnAmount--;
			m_SpawnTimer = 0;
			if (m_SpawnAmount > 0)
				m_SpawnTime = 0.05f;
			else
			{
				m_SpawnTime = float(std::rand() % 5 + 5);
				m_SpawnAmount = std::rand() % 5 + 1;
				int r = std::rand() % 2;
				if (r % 2 == 0)
					m_SpawnLeftZeko = true;
				else m_SpawnLeftZeko = false;
			}
		}
	}
	else m_SpawnTimer = 0.0f;

}

void TestScene::Render() const
{
	
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
		for (int index : idxRemove)
		{
			std::swap(m_pZakos[index], m_pZakos.back());
			m_pZakos.pop_back();
		}
	}
	idxRemove.clear();
}
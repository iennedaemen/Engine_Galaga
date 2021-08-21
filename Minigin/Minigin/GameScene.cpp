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
#include "HitObserver.h"

// json reader = https://github.com/nlohmann/json
using json = nlohmann::json;

GameScene::GameScene(int level)
	: Scene("GameScene" + std::to_string(level))
	, m_Level(level)
{}

void GameScene::Initialize()
{
	// OBSERVER(S)
	m_Observers.push_back(std::make_shared<HitObserver>());


	// BACKGROUND
	std::shared_ptr<GameObject> pBackObj = std::make_shared<GameObject>();
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(1000.0f, 2000.0f, 1, 1);
	pBackObj->AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Background.png");
	pSpriteComp->IsStatic(true);
	pBackObj->m_Rect = { pBackObj->m_Rect.x, pBackObj->m_Rect.y + 20, ScreenInfo::GetInstance().screenwidth, ScreenInfo::GetInstance().screenheigth };
	Add(pBackObj);


	// SCORE TEXT
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


	// LEVEL TEXT
	m_pLevelText = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> text = std::make_shared<TextComponent>("../Data/Pixel.otf", 25);
	text->SetText("Level " + std::to_string(m_Level));
	m_pLevelText->AddComponent(text);
	Add(m_pLevelText);
	m_pLevelText->SetPosition(135, ScreenInfo::GetInstance().screenheigth/2.0f - 50);


	// LIVES VISUALS
	for (unsigned int i{}; i < 3; ++i)
	{
		std::shared_ptr<GameObject> pLiveVisual = std::make_shared<GameObject>();
		std::shared_ptr<SpriteComponent> pSpriteComp2 = std::make_shared<SpriteComponent>(60.0f, 64.0f, 1, 1);
		pLiveVisual->AddComponent(pSpriteComp2);
		pSpriteComp2->SetTexture("Player1.png");
		pSpriteComp2->IsStatic(true);
		pLiveVisual->m_Rect = { pLiveVisual->m_Rect.x, pLiveVisual->m_Rect.y, 15, 17 };
		Add(pLiveVisual);
		pLiveVisual->SetPosition(5.0f + 20 * i, ScreenInfo::GetInstance().screenheigth - 20.0f);

		m_pLivesP1[i] = pLiveVisual;
	}

	for (unsigned int i{}; i < 3; ++i)
	{
		std::shared_ptr<GameObject> pLive2Visual = std::make_shared<GameObject>();
		std::shared_ptr<SpriteComponent> pSpriteComp3 = std::make_shared<SpriteComponent>(60.0f, 64.0f, 1, 1);
		pLive2Visual->AddComponent(pSpriteComp3);
		pSpriteComp3->SetTexture("Player2.png");
		pSpriteComp3->IsStatic(true);
		pLive2Visual->m_Rect = { pLive2Visual->m_Rect.x, pLive2Visual->m_Rect.y, 15, 17 };
		Add(pLive2Visual);
		pLive2Visual->SetPosition(ScreenInfo::GetInstance().screenwidth - 60.0f + 20.0f * i, ScreenInfo::GetInstance().screenheigth - 20.0f);

		m_pLivesP2[i] = pLive2Visual;
	}


	// PLAYER(S)
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


	// POSSIBLE ENEMY POSITIONS
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


	// READ FILE
	ReadFile();
}

void GameScene::Update()
{
	// LEVEL TEXT
	if (m_IsBegin)
	{
		m_BeginTimer += Time::GetInstance().m_ElapsedSec;
		if (m_BeginTimer >= m_BeginTime)
		{
			m_IsBegin = false;
			m_pLevelText->GetComponent<TextComponent>()->SetText(" ");
		}
	}


	// UPDATE LIVES VISUALS
	//if (GameInfo::GetInstance().player1Lives == 2)
	//{
	//	Remove(m_pLivesP1[2]);
	//}
	//else if (GameInfo::GetInstance().player1Lives == 1)
	//{
	//	Remove(m_pLivesP1[1]);
	//}
	//else if (GameInfo::GetInstance().player1Lives == 0)
	//{
	//	Remove(m_pLivesP1[0]);
	//}

	//if (GameInfo::GetInstance().player2Active)
	//{
	//	if (GameInfo::GetInstance().player2Lives == 2)
	//	{
	//		Remove(m_pLivesP2[2]);
	//	}
	//	else if (GameInfo::GetInstance().player2Lives == 1)
	//	{
	//		Remove(m_pLivesP2[1]);
	//	}
	//	else if (GameInfo::GetInstance().player2Lives == 0)
	//	{
	//		Remove(m_pLivesP2[0]);
	//	}
	//}

	// UPDATE SCORE VISUALS
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


	// SPAWN TIMER
	m_SpawnTimer += Time::GetInstance().m_ElapsedSec;


	// UPDATE PLAYER(S)
	UpdatePlayer(m_pPlayer);
	if (GameInfo::GetInstance().player2Active)
		UpdatePlayer(m_pPlayer2);


	// UPDATE ENEMIES
	SpawnEnemy(EnemyType::Zako, m_ZakoPositions, m_ZakoTimes);
	SpawnEnemy(EnemyType::Goei, m_GoeiPositions, m_GoeiTimes);
	SpawnEnemy(EnemyType::Boss, m_BossPositions, m_BossTimes);
	UpdateEnemy(EnemyType::Zako, m_pZakos);
	UpdateEnemy(EnemyType::Goei, m_pGoeis);
	UpdateEnemy(EnemyType::Boss, m_pBosses);


	// SCENE MANAGEMENT
	if ((GetAsyncKeyState('P') & 0x8000) && (k == 0))
	{
		k = 1;
		Reset();
		if (m_Level < GameInfo::GetInstance().amountLevels)
			SceneManager::GetInstance().SetActiveScene("GameScene" + std::to_string(m_Level + 1));
		else SceneManager::GetInstance().SetActiveScene("GameScene" + std::to_string(1));
	}
	else if (GetAsyncKeyState('P') == 0) k = 0;

	if (m_EnemiesDead >= m_AmountZako + m_AmountGoei + m_AmountBoss)
	{
		if (m_Level < GameInfo::GetInstance().amountLevels)
			SceneManager::GetInstance().SetActiveScene("GameScene" + std::to_string(m_Level + 1));
		else SceneManager::GetInstance().SetActiveScene("GameOverScene");

	}

	if (GameInfo::GetInstance().player2Active)
	{
		std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);

		if (dPlayer->m_IsDead && dPlayer2->m_IsDead)
			SceneManager::GetInstance().SetActiveScene("GameOverScene");
	}
	else
	{
		std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);

		if (dPlayer->m_IsDead)
			SceneManager::GetInstance().SetActiveScene("GameOverScene");
	}
}

void GameScene::Render() const
{

}

void GameScene::Reset()
{
	// RESET PLAYER(S)
	Remove(m_pPlayer);
	m_pPlayer = nullptr;
	m_pPlayer = std::make_shared<Player>(1);
	Add(m_pPlayer);
	std::shared_ptr<Player> dPlayer = std::dynamic_pointer_cast<Player> (m_pPlayer);

	Remove(m_pPlayer2);
	m_pPlayer2 = nullptr;

	if (GameInfo::GetInstance().player2Active)
	{
		m_pPlayer2 = std::make_shared<Player>(2);
		Add(m_pPlayer2);
		std::shared_ptr<Player> dPlayer2 = std::dynamic_pointer_cast<Player> (m_pPlayer2);
		m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer->m_Rect.w / 2.0f - 20), float(ScreenInfo::GetInstance().screenheigth - 75));
		m_pPlayer2->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer2->m_Rect.w / 2.0f + 20), float(ScreenInfo::GetInstance().screenheigth - 75));
	}
	else m_pPlayer->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2.0f - m_pPlayer->m_Rect.w / 2.0f), float(ScreenInfo::GetInstance().screenheigth - 75));


	// RESET LEVEL TEXT
	m_BeginTimer = 0.0f;
	m_IsBegin = true;
	m_pLevelText->GetComponent<TextComponent>()->SetText("Level " + std::to_string(m_Level));
	

	//RESET PLAYER 2 SCORE VISUALS
	if (GameInfo::GetInstance().player2Active)
	{
		std::stringstream ss;
		ss << std::setw(5) << std::setfill('0') << GameInfo::GetInstance().scoreP2;
		std::string s = ss.str();
		m_pTextScoreP2->GetComponent<TextComponent>()->SetText(s);
	}
	else m_pTextScoreP2->GetComponent<TextComponent>()->SetText(" ");


	// RESET LIVES VISUALS
	Add(m_pLivesP1[0]);
	Add(m_pLivesP1[1]);
	Add(m_pLivesP1[2]);


	// RESET SPAWNER
	m_SpawnTimer = 0;


	// RESET DEAD ENEMIES;
	m_EnemiesDead = 0;


	// RESET ZAKO
	for (unsigned int i{}; i < m_pZakos.size(); ++i)
	{
		Remove(m_pZakos[i]);
	}
	m_pZakos.clear();
	m_NrActiveZako = 0;
	m_SpawnAmountZako = 4;
	m_SpawnLeftZako = false;


	// RESET GOEI
	for (unsigned int i{}; i < m_pGoeis.size(); ++i)
	{
		Remove(m_pGoeis[i]);
	}
	m_pGoeis.clear();
	m_NrActiveGoei = 0;
	m_SpawnAmountGoei = 0;
	m_SpawnLeftGoei = false;


	// RESET BOSS
	for (unsigned int i{}; i < m_pBosses.size(); ++i)
	{
		Remove(m_pBosses[i]);
	}
	m_pBosses.clear();
	m_NrActiveBoss = 0;
	m_SpawnAmountBoss = 0;


	// RESET READ VARIABLES
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
	if (!dPlayer->m_IsHit && !dPlayer->IsAbducted())
	{
		// ZAKOS
		for (unsigned int i{}; i < m_pZakos.size(); ++i)
		{
			std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako> (m_pZakos[i]);
			if (zako->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->m_IsHit = true;
				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (zako->GetLaser());
				dLaser->m_IsActive = false;

			}
			if (dPlayer->GetComponent<ColliderComponent>()->IsColliding(zako->m_Rect))
			{
				dPlayer->m_IsHit = true;
				zako->m_IsHit = true;
			}
		}

		// GOEIS
		for (unsigned int i{}; i < m_pGoeis.size(); ++i)
		{
			std::shared_ptr<Goei> goei = std::dynamic_pointer_cast<Goei> (m_pGoeis[i]);
			if (goei->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->m_IsHit = true;

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (goei->GetLaser());
				dLaser->m_IsActive = false;

			}
		}
		// BOSSES
		for (unsigned int i{}; i < m_pBosses.size(); ++i)
		{
			std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
			if (boss->GetLaser()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->m_IsHit = true;

				std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (boss->GetLaser());
				dLaser->m_IsActive = false;
			}
		}
	}

	if (dPlayer->m_IsDead)
	{
		Remove(pPlayer);
	}

	if (!dPlayer->m_Exploding)
	{
		for (unsigned int i{}; i < m_pBosses.size(); ++i)
		{
			std::shared_ptr<Boss> dBoss = std::dynamic_pointer_cast<Boss> (m_pBosses[i]);
			if (dBoss->GetBeam()->GetComponent<ColliderComponent>()->IsColliding(pPlayer->m_Rect))
			{
				dPlayer->SetAbducted(true, m_pBosses[i]);
			}
		}
	}

}

void GameScene::SpawnEnemy(EnemyType type, std::vector<glm::vec2> possiblePos, std::queue<float> &spawnTimes)
{
	if (spawnTimes.empty())
		return;

	if (m_SpawnTimer >= spawnTimes.front())
	{
		spawnTimes.pop();

		std::shared_ptr<GameObject> enemyObj = nullptr;
		std::shared_ptr<Zako> zako = nullptr;
		std::shared_ptr<Goei> goei = nullptr;
		std::shared_ptr<Boss> boss = nullptr;

		switch (type)
		{
		case EnemyType::Zako:
			if (m_NrActiveZako >= m_AmountZako)
				return;

			m_NrActiveZako++;

			enemyObj = std::make_shared<Zako>(m_ZakoPositions[m_ZakoPos.front().first]);
			Add(enemyObj);
			m_pZakos.push_back(enemyObj);
			zako = std::dynamic_pointer_cast<Zako>(enemyObj);

			if (m_ZakoPos.front().second)
			{
				enemyObj->SetPosition(-20, 400);
				zako->m_SpawnedLeft = true;
			}
			else
			{
				enemyObj->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 400.0f);
				zako->m_SpawnedLeft = false;
			}

			m_ZakoPos.pop();

			break;

		case EnemyType::Goei:
			if (m_NrActiveGoei >= m_AmountGoei)
				return;

			m_NrActiveGoei++;

			enemyObj = std::make_shared<Goei>(m_GoeiPositions[m_GoeiPos.front().first]);
			Add(enemyObj);
			m_pGoeis.push_back(enemyObj);
			goei = std::dynamic_pointer_cast<Goei>(enemyObj);

			if (m_GoeiPos.front().second)
			{
				enemyObj->SetPosition(-20, 400);
				goei->m_SpawnedLeft = true;
			}
			else
			{
				enemyObj->SetPosition(float(ScreenInfo::GetInstance().screenwidth - 20), 400.0f);
				goei->m_SpawnedLeft = false;
			}

			m_GoeiPos.pop();

			break;

		case EnemyType::Boss:
			if (m_NrActiveBoss >= m_AmountBoss)
				return;

			m_NrActiveBoss++;

			enemyObj = std::make_shared<Boss>(m_BossPositions[m_BossPos.front().first]);
			Add(enemyObj);
			m_pBosses.push_back(enemyObj);
			boss = std::dynamic_pointer_cast<Boss>(enemyObj);

			enemyObj->SetPosition(float(ScreenInfo::GetInstance().screenwidth / 2), -20);

			if (m_BossPos.front().second) boss->m_SpawnedLeft = true;
			else boss->m_SpawnedLeft = false;

			m_BossPos.pop();

			break;

		default:

			break;
		}
	}
}

void GameScene::UpdateEnemy(EnemyType type, std::vector<std::shared_ptr<GameObject>>& Enemies)
{
	// GET LASERS FROM PLAYER
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
	for (unsigned int i{}; i < Enemies.size(); ++i)
	{
		if (type == EnemyType::Zako)
		{
			std::shared_ptr<Zako> zako = std::dynamic_pointer_cast<Zako> (Enemies[i]);
			int r = std::rand() % 2;
			if (r % 2 == 0)
				zako->SetNextAction(true);
			else zako->SetNextAction(false);
		}
		else if (type == EnemyType::Boss)
		{
			std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss> (Enemies[i]);
			int r = std::rand() % 2;
			if (r % 2 == 0)
				boss->SetNextAction(true);
			else boss->SetNextAction(false);
		}

		std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy> (Enemies[i]);

		enemy->m_PlayerPos = { m_pPlayer->m_Rect.x + m_pPlayer->m_Rect.w / 2,  m_pPlayer->m_Rect.y };
		if (GameInfo::GetInstance().player2Active)
			enemy->m_Player2Pos = { m_pPlayer2->m_Rect.x + m_pPlayer2->m_Rect.w / 2,  m_pPlayer2->m_Rect.y };
	}

	// COLLISION
	for (unsigned int i{}; i < pLasers.size(); ++i)
	{
		for (unsigned int j{}; j < Enemies.size(); ++j)
		{
			if (pLasers[i]->GetComponent<ColliderComponent>()->IsColliding(Enemies[j]->m_Rect))
			{
				std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy> (Enemies[j]);

				if (!enemy->m_IsHit)
				{
					auto parent = pLasers[i]->GetParent();
					for (std::shared_ptr<Observer> observer : m_Observers)
					{
						if(type == EnemyType::Zako)
							observer->OnNotify(Event::ZakoHit, Enemies[j].get(), parent);
						else if (type == EnemyType::Goei)
							observer->OnNotify(Event::GoeiHit, Enemies[j].get(), parent);
						else if (type == EnemyType::Boss)
							observer->OnNotify(Event::BossHit, Enemies[j].get(), parent);
					}
					enemy->m_IsHit = true;
					Player* p = static_cast<Player*>(parent);
					p->RemoveLaser(std::dynamic_pointer_cast<Laser>(pLasers[i]));
				}
			}
		}
	}

	// DELETE
	std::vector<int> idxRemove;
	for (unsigned int i{}; i < Enemies.size(); ++i)
	{
		std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy> (Enemies[i]);
		if (enemy->m_IsDead)
		{
			m_EnemiesDead++;
			Remove(Enemies[i]);
			idxRemove.push_back(i);
		}
	}

	if (!idxRemove.empty())
	{
		std::sort(idxRemove.begin(), idxRemove.end(), std::greater<int>());
		for (int index : idxRemove)
		{
			std::swap(Enemies[index], Enemies.back());
			Enemies.pop_back();
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

		for (unsigned int i{}; i < jf[level]["ZakoTimes"].size(); ++i)
		{
			m_ZakoTimes.push(jf[level]["ZakoTimes"][i]);
		}

		for (unsigned int i{}; i < jf[level]["ZakoPositions"].size(); ++i)
		{
			m_ZakoPos.push(std::pair<int, bool>(jf[level]["ZakoPositions"][i], jf[level]["ZakoSpawnLeft"][i]));
		}


		// GOEI
		m_AmountGoei = jf[level]["AmountGoei"];

		for (unsigned int i{}; i < jf[level]["GoeiTimes"].size(); ++i)
		{
			m_GoeiTimes.push(jf[level]["GoeiTimes"][i]);
		}

		for (unsigned int i{}; i < jf[level]["GoeiPositions"].size(); ++i)
		{
			m_GoeiPos.push(std::pair<int, bool>(jf[level]["GoeiPositions"][i], jf[level]["GoeiSpawnLeft"][i]));
		}


		// BOSS
		m_AmountBoss = jf[level]["AmountBoss"];

		for (unsigned int i{}; i < jf[level]["BossTimes"].size(); ++i)
		{
			m_BossTimes.push(jf[level]["BossTimes"][i]);
		}

		for (unsigned int i{}; i < jf[level]["BossPositions"].size(); ++i)
		{
			m_BossPos.push(std::pair<int, bool>(jf[level]["BossPositions"][i], jf[level]["BossSpawnLeft"][i]));
		}
	}
}
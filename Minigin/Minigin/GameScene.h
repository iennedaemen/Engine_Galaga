#pragma once
#include "Scene.h"
#include <queue>
#include "Observer.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include <unordered_map>

class GameScene : public Scene
{
public:
	GameScene(int level);

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const override;
	virtual void Reset() override;

private:
	int k = 0;

	std::shared_ptr<GameObject> m_pPlayer = nullptr;
	std::shared_ptr<GameObject> m_pPlayer2 = nullptr;

	std::shared_ptr<GameObject> m_pTextScoreP1 = nullptr;
	std::shared_ptr<GameObject> m_pTextScoreP2 = nullptr;

	float m_SpawnTimer = 0;
	int m_EnemiesDead = 0;

	// LIVES VISUALS
	std::shared_ptr<GameObject> m_pLivesP1[3];
	std::shared_ptr<GameObject> m_pLivesP2[3];

	//BEGIN
	std::shared_ptr<GameObject> m_pLevelText = nullptr;
	float m_BeginTimer = 0.0f;
	float m_BeginTime = 4.0f;
	bool m_IsBegin = true;

	// ZAKO
	std::vector<glm::vec2> m_ZakoPositions;
	std::vector<std::shared_ptr<GameObject>> m_pZakos;
	int m_NrActiveZako = 0;
	int m_SpawnAmountZako = 4;
	bool m_SpawnLeftZako = false;

	// GOEI
	std::vector<glm::vec2> m_GoeiPositions;
	std::vector<std::shared_ptr<GameObject>> m_pGoeis;
	int m_NrActiveGoei = 0;
	int m_SpawnAmountGoei = 0;
	bool m_SpawnLeftGoei = false;

	// BOSS
	std::vector<glm::vec2> m_BossPositions;
	std::vector<std::shared_ptr<GameObject>> m_pBosses;
	int m_NrActiveBoss = 0;
	int m_SpawnAmountBoss = 0;

	// OBSERVER
	std::vector<std::shared_ptr<Observer>> m_Observers;

	// READ
	int m_Level;

	int m_AmountZako = 0;
	std::queue<float> m_ZakoTimes;
	std::queue<std::pair<int, bool>> m_ZakoPos;

	int m_AmountGoei = 0;
	std::queue<float> m_GoeiTimes;
	std::queue<std::pair<int, bool>> m_GoeiPos;

	int m_AmountBoss = 0;
	std::queue<float> m_BossTimes;
	std::queue<std::pair<int, bool>> m_BossPos;

	// FUNCTIONS
	void SpawnEnemy(EnemyType type, std::vector<glm::vec2> possiblePos, std::queue<float>& spawnTimes);
	void UpdateEnemy(EnemyType type, std::vector<std::shared_ptr<GameObject>>& Enemies);
	void UpdatePlayer(std::shared_ptr<GameObject> pPlayer);
	void ReadFile();

};


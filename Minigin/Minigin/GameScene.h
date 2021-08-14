#pragma once
#include "Scene.h"
#include <queue>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class GameScene : public Scene
{
public:
	GameScene(int level);

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const override;

	//SET SCENE

private:
	std::shared_ptr<GameObject> m_pPlayer = nullptr;
	std::shared_ptr<GameObject> m_pPlayer2 = nullptr;

	float m_SpawnTimer = 0;

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

	// FUNCTIONS
	void UpdatePlayer();
	void SpawnZako();
	void UpdateZako();
	void SpawnGoei();
	void UpdateGoei();
	void SpawnBoss();
	void UpdateBoss();


	// READ
	void ReadFile();

	int m_Level;

	int m_AmountZako = 0;
	std::queue<float> m_ZakoTimes;
	std::queue<int> m_ZakoPos;

	int m_AmountGoei = 0;
	std::queue<float> m_GoeiTimes;
	std::queue<int> m_GoeiPos;

	int m_AmountBoss = 0;
	std::queue<float> m_BossTimes;
	std::queue<int> m_BossPos;

};


#pragma once
#include "Scene.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class TestScene : public Scene
{
public:
	TestScene();
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const override;
	
private:
	std::shared_ptr<GameObject> m_pPlayer = nullptr;
	std::shared_ptr<GameObject> m_pPlayer2 = nullptr;



	// ENEMIES
	float m_SpawnTimer = 0;
	float m_SpawnTime = 5;
	int m_SpawnNr = 0;

	// ZEKO
	std::vector<std::pair<glm::vec2, bool>> m_ZakoPositions;
	std::vector<std::shared_ptr<GameObject>> m_pZakos;
	int m_NrActiveZeko = 0;
	int m_SpawnAmountZeko = 4;
	bool m_SpawnLeftZeko = false;

	// GOEI
	std::vector<std::pair<glm::vec2, bool>> m_GoeiPositions;
	std::vector<std::shared_ptr<GameObject>> m_pGoeis;
	int m_NrActiveGoei = 0;
	int m_SpawnAmountGoei = 0;
	bool m_SpawnLeftGoei = false;

	// BOSS
	std::vector<std::pair<glm::vec2, bool>> m_BossPositions;
	std::vector<std::shared_ptr<GameObject>> m_pBosses;
	int m_NrActiveBoss = 0;
	int m_SpawnAmountBoss = 0;

	// FUNCTIONS
	void UpdatePlayer();
	void SpawnZako(bool SpawnLeft);
	void UpdateZako();
	void SpawnGoei(bool SpawnLeft);
	void UpdateGoei();
	void SpawnBoss();
	void UpdateBoss();

};


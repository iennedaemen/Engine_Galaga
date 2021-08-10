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

	std::vector<std::shared_ptr<GameObject>> m_pZakos;
	std::vector<std::shared_ptr<GameObject>> m_pGoeis;
	std::vector<std::pair<glm::vec2, bool>> m_ZakoPositions;
	std::vector<std::pair<glm::vec2, bool>> m_GoeiPositions;

	int m_SpawnNr = 0;
	// ZEKO
	int m_NrActiveZeko = 0;
	float m_SpawnTimer = 0;
	float m_SpawnTime = 5;
	int m_SpawnAmountZeko = 4;
	bool m_SpawnLeftZeko = false;

	// GOEI
	int m_NrActiveGoei = 0;
	int m_SpawnAmountGoei = 0;
	bool m_SpawnLeftGoei = false;

	// FUNCTIONS
	void UpdatePlayer();
	void SpawnZako(bool SpawnLeft);
	void UpdateZako();
	void SpawnGoei(bool SpawnLeft);
	void UpdateGoei();


};


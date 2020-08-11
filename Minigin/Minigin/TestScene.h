#pragma once
#include "Scene.h"

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
};


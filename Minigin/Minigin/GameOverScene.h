#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
	GameOverScene();
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const {};
	virtual void Reset() override;

private:
	std::shared_ptr<GameObject> m_pTextMenu = nullptr;
	std::shared_ptr<GameObject> m_pTextQuit = nullptr;

	int m_ItemSelected = 0;
	bool m_ButtonPressed = true;

	void MakeScoreBoard(int playerNr);
};
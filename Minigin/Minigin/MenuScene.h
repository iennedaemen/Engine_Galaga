#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene();
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const {};
	virtual void Reset() override;

private:
	std::shared_ptr<GameObject> m_pTextP1 = nullptr;
	std::shared_ptr<GameObject> m_pTextP2 = nullptr;
	std::shared_ptr<GameObject> m_pTextQuit = nullptr;

	int m_ItemSelected = 0;

	bool m_ButtonPressed = true;
};
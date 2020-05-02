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
	
};


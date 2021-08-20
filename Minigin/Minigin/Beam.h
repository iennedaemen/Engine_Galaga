#pragma once
#include "GameObject.h"

class Beam : public GameObject
{
public:
	Beam() {}

	bool m_IsActive = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;
};
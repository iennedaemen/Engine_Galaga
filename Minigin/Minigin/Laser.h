#pragma once
#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(bool isPlayerLaser) : m_IsPlayerLaser(isPlayerLaser) {}

	bool m_IsActive = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	bool m_IsPlayerLaser;

};


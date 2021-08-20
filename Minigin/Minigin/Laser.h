#pragma once
#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(bool playerLasers) : m_PlayerLasers(playerLasers) {}

	bool m_IsActive = false;

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	bool m_PlayerLasers;

};


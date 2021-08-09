#pragma once
#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(bool playerLasers) : m_PlayerLasers(playerLasers) {}

	void SetActive(bool active)
	{
		m_IsActive = active;
	}

	bool IsActive()
	{
		return m_IsActive;
	}

protected:
	void Initialize() override;
	void Render() override {};
	void Update() override;

private:
	bool m_PlayerLasers;
	bool m_IsActive = false;
};


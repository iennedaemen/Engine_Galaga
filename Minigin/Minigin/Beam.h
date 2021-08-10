#pragma once
#include "GameObject.h"

class Beam : public GameObject
{
public:
	Beam() {}

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
	bool m_IsActive = false;
};
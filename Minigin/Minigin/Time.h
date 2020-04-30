#pragma once
#include "Singleton.h"

class Time final : public Singleton<Time>
{
public:
	float GetElapsedSec()
	{
		return m_ElapsedSec;
	}
	void SetElapsedSec(float elapsedSec)
	{
		m_ElapsedSec = elapsedSec;
	}
	
private:
	float m_ElapsedSec = 0.0f;
};

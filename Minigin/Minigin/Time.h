#pragma once
#include "Singleton.h"

class Time final : public Singleton<Time>
{
public:
	float m_ElapsedSec = 0;
};

#pragma once
#include "Singleton.h"

class GameInfo final : public Singleton<GameInfo>
{
public:
	bool player2Active = false;
	bool enemyPlayerActive = false;
};


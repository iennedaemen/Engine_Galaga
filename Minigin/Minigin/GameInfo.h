#pragma once
#include "Singleton.h"

class GameInfo final : public Singleton<GameInfo>
{
public:
	int player1Lives = 3;
	int player2Lives = 3;
	bool player2Active = false;
	bool enemyPlayerActive = false;
	int score = 0;
};


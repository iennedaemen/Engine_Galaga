#pragma once
#include "Singleton.h"

class GameInfo final : public Singleton<GameInfo>
{
public:
	int player1Lives = 3;
	int player2Lives = 3;
	bool player2Active = false;

	int scoreP1 = 0;
	int scoreP2 = 0;

	int shotsFiredP1 = 0;
	int shotsFiredP2 = 0;

	int shotsHitP1 = 0;
	int shotsHitP2 = 0;

	int amountLevels = 3;
};


#pragma once
#include "Observer.h"
#include "structs.h"
#include "GameInfo.h"
#include <map>
#include "Zako.h"
#include "Goei.h"
#include "Boss.h"
#include "Player.h"

class ScoreObserver : public Observer
{
public:
	ScoreObserver()
	{
		std::map<State, int> zakoScoreMap;
		zakoScoreMap[State::Idle] = 50;
		zakoScoreMap[State::Moving] = 100;
		m_ScoreMap[EnemyType::Zako] = zakoScoreMap;

		std::map<State, int> goeiScoreMap;
		goeiScoreMap[State::Idle] = 80;
		goeiScoreMap[State::Moving] = 160;
		m_ScoreMap[EnemyType::Goei] = goeiScoreMap;

		std::map<State, int> bossScoreMap;
		bossScoreMap[State::Idle] = 150;
		bossScoreMap[State::Moving] = 400;
		m_ScoreMap[EnemyType::Boss] = bossScoreMap;
	};

	virtual void  onNotify(const Event event, GameObject* arg) 
	{
		UNREFERENCED_PARAMETER(event);
		UNREFERENCED_PARAMETER(arg);
	};

	virtual void onNotify(const Event event, GameObject* arg1, GameObject* arg2)
	{
		Player* pPlayer = static_cast<Player*>(arg2);

		if (pPlayer->getPlayerNr() == 1)
			GameInfo::GetInstance().shotsHitP1 += 1;

		else if (pPlayer->getPlayerNr() == 2)
			GameInfo::GetInstance().shotsHitP2 += 1;

		switch (event)
		{
		case Event::ZakoHit:
		{
			Zako* pZako = static_cast<Zako*>(arg1);

			if (pPlayer->getPlayerNr() == 1)
			{
				GameInfo::GetInstance().scoreP1 += m_ScoreMap[EnemyType::Zako][pZako->m_EnumState];
			}
			else if (pPlayer->getPlayerNr() == 2)
			{
				GameInfo::GetInstance().scoreP2 += m_ScoreMap[EnemyType::Zako][pZako->m_EnumState];
			}
			//error
			break;
		}

		case Event::GoeiHit:
		{
			Goei* pGoei = static_cast<Goei*>(arg1);

			if (pPlayer->getPlayerNr() == 1)
			{
				GameInfo::GetInstance().scoreP1 += m_ScoreMap[EnemyType::Goei][pGoei->m_EnumState];
			}
			else if (pPlayer->getPlayerNr() == 2)
			{
				GameInfo::GetInstance().scoreP2 += m_ScoreMap[EnemyType::Goei][pGoei->m_EnumState];
			}
			//error
			break;
		}

		case Event::BossHit:
		{
			Boss* pBoss = static_cast<Boss*>(arg1);

			if (pBoss->m_Lives <= 0)
			{
				if (pPlayer->getPlayerNr() == 1)
				{
					GameInfo::GetInstance().scoreP1 += m_ScoreMap[EnemyType::Boss][pBoss->m_EnumState];
				}
				else if (pPlayer->getPlayerNr() == 2)
				{
					GameInfo::GetInstance().scoreP2 += m_ScoreMap[EnemyType::Boss][pBoss->m_EnumState];
				}
				//error
			}
			break;
		}
		default:

			break;
		}
	}

private:
    std::map<EnemyType, std::map<State, int>> m_ScoreMap;
};
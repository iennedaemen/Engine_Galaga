#pragma once
#include "Observer.h"
#include "structs.h"
#include "GameInfo.h"
#include <map>
#include "Zako.h"
#include "Player.h"

class ScoreCounter : public Observer
{
public:
	ScoreCounter()
	{
		std::map<State, int> zakoScoreMap;
		zakoScoreMap[State::Idle] = 50;
		zakoScoreMap[State::Attacking] = 100;
		m_ScoreMap[EnemyType::Zako] = zakoScoreMap;

		m_ScoreP1 = 0;
		m_ScoreP2 = 0;
	};

	virtual void  onNotify(const Event event, std::shared_ptr<GameObject> arg) 
	{
		UNREFERENCED_PARAMETER(event);
		UNREFERENCED_PARAMETER(arg);
	};

	virtual void onNotify(const Event event, std::shared_ptr<GameObject> arg1, std::shared_ptr<GameObject> arg2)
	{
		switch (event)
		{
		case Event::ZakoHit:
			
			std::shared_ptr<Zako> pZako = std::dynamic_pointer_cast<Zako>(arg1);
			std::shared_ptr<Player> pPlayer = std::dynamic_pointer_cast<Player>(arg2);

			if (pPlayer->getPlayerNr() == 1)
			{
				m_ScoreP1 += m_ScoreMap[EnemyType::Zako][pZako->GetEnumState()];
			}
			else if (pPlayer->getPlayerNr() == 2)
			{
				m_ScoreP2 += m_ScoreMap[EnemyType::Zako][pZako->GetEnumState()];
			}
			//error
			break;
		}

		//if (arg1 == 1)
		//{
		//	m_ScoreP1 += m_ScoreMap[type][state];
		//}
		//else if (playerNr == 2)
		//{
		//	m_ScoreP2 += m_ScoreMap[type][state];
		//}
		//// error
	}

private:
    std::map<EnemyType, std::map<State, int>> m_ScoreMap;
	int m_ScoreP1;
	int m_ScoreP2;
};
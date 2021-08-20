#pragma once
#include "Observer.h"
#include "structs.h"
#include "GameInfo.h"
#include <map>
#include "Zako.h"
#include "Goei.h"
#include "Boss.h"
#include "Player.h"

class HitObserver : public Observer
{
public:
	HitObserver();

	virtual void  OnNotify(const Event event, GameObject* arg) override;
	virtual void OnNotify(const Event event, GameObject* arg1, GameObject* arg2) override;

private:
	std::map<EnemyType, std::map<State, int>> m_ScoreMap;
};
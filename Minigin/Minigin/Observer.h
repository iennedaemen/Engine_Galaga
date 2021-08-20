#pragma once
#include "GameObject.h"
#include "structs.h"

class Observer
{
public:
	virtual ~Observer() {};
	virtual void onNotify(const Event event, GameObject* arg) = 0;
	virtual void onNotify(const Event event, GameObject* arg1, GameObject* arg2) = 0;
};
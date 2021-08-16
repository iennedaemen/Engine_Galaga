#pragma once
#include "GameObject.h"
#include "structs.h"

class Observer
{
public:
	virtual ~Observer() {};
	virtual void onNotify(const Event event, std::shared_ptr<GameObject> arg) = 0;
	virtual void onNotify(const Event event, std::shared_ptr<GameObject> arg1, std::shared_ptr<GameObject> arg2) = 0;
};
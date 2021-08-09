#pragma once
#include "Singleton.h"

class ScreenInfo final : public Singleton<ScreenInfo>
{
public:
	int screenwidth = 400;
	int screenheigth = 540;
	float framesPerSecond = 10.0f;
};
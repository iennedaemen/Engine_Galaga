#pragma once
#include "Singleton.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class PyramidPos final : public Singleton<PyramidPos>
{
public:
	std::vector<std::vector<glm::vec2>> Positions;
};
#pragma once

#include "Engine/Core/Blackboard.hpp"
#include <vector>
#include <string>

#define UNUSED(x) (void)(x);

typedef std::vector<std::string> Strings;
typedef std::vector<int> Ints;

extern Blackboard g_gameConfigBlackboard; // declared in EngineCommon.hpp, defined in .cpp
#pragma once

#include "Engine/ThirdParty/tinyxml2.h"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteAnimSetDefinition.hpp"
#include <string>
#include <map>

class EntityDefinition
{
public:
	virtual ~EntityDefinition();
	EntityDefinition(const tinyxml2::XMLElement & element);

public:
	std::string m_name;
	bool canWalk = false;;
	bool canSwim = false;
	bool canFly = false;
	float m_speed = 0;
	float m_physicsRadius;
	AABB2 m_localDrawBounds;
	Rgba m_defaultTint;
	SpriteAnimSetDefinition* m_spriteAnimSetDef;
};

#pragma once

#include "Game/TileDefinition.hpp"
#include "Game/MapGenStep.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/ThirdParty/tinyxml2.h"
#include <map>
#include <vector>

class MapDefinition
{
public:
	~MapDefinition();
	MapDefinition(const tinyxml2::XMLElement & element);

	static void LoadDefinitions( const std::string& path);
	static MapDefinition* GetDefinition( const std::string& defName);

public:
	static std::map<std::string, MapDefinition*> s_definitions;

	std::string m_name;
	TileDefinition* m_defaultTile = nullptr;
	IntRange m_widthRange;
	IntRange m_heightRange;
	std::vector<MapGenStep*> m_MapGenSteps;
};


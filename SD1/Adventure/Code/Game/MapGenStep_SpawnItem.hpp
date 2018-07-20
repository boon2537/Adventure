#pragma once

#include "Game/MapGenStep.hpp"
#include "Game/TileDefinition.hpp"
#include <string>

class MapGenStep_SpawnItem : public MapGenStep
{
public:
	~MapGenStep_SpawnItem();
	MapGenStep_SpawnItem( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;

public:
	std::string m_itemToSpawn;
	TileDefinition* m_onTile = nullptr;
};

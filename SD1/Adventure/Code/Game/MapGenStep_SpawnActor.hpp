#pragma once

#include "Game/MapGenStep.hpp"
#include "Game/TileDefinition.hpp"
#include <string>

class MapGenStep_SpawnActor : public MapGenStep
{
public:
	~MapGenStep_SpawnActor();
	MapGenStep_SpawnActor( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;

public:
	std::string m_actorToSpawn;
	TileDefinition* m_onTile = nullptr;
};

#pragma once

#include "Game/MapGenStep.hpp"
#include "Game/TileDefinition.hpp"

class MapGenStep_FillAndEdge : public MapGenStep
{
public:
	~MapGenStep_FillAndEdge();
	MapGenStep_FillAndEdge( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;

public:
	TileDefinition* m_fillTile = nullptr;
	TileDefinition* m_edgeTile = nullptr;
};

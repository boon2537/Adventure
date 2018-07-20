#pragma once

#include "Game/MapGenStep.hpp"
#include "Game/TileDefinition.hpp"

class MapGenStep_CellularAutomata : public MapGenStep
{
public:
	~MapGenStep_CellularAutomata();
	MapGenStep_CellularAutomata( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;
	bool PassNeighborChecks(Map& map, int x, int y);

public:
	TileDefinition* m_toType = nullptr;
	TileDefinition* m_ifType = nullptr;
	TileDefinition* m_ifNeighborType = nullptr;
	IntRange m_ifNeighborCount;
	float m_chanceToMutate = 1.f; 
};

#pragma once

#include "Game/MapGenStep.hpp"
#include "Game/TileDefinition.hpp"

class MapGenStep_Mutate : public MapGenStep
{
public:
	~MapGenStep_Mutate();
	MapGenStep_Mutate( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;

public:
	TileDefinition* m_toType = nullptr;
	TileDefinition* m_ifType = nullptr;
	float m_chanceToMutate = 1.f; 
};

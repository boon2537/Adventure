#pragma once

#include "Game/MapGenStep.hpp"
#include "Game/TileDefinition.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/IntRange.hpp"

class MapGenStep_PerlinNoise : public MapGenStep
{
public:
	~MapGenStep_PerlinNoise();
	MapGenStep_PerlinNoise( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;

public:
	TileDefinition* m_toType = nullptr;
	TileDefinition* m_ifType = nullptr;
	FloatRange m_ifNoise;
	IntRange m_gridSize;
	int m_octaves;
	float m_persistence;
	float m_octaveScale;
};

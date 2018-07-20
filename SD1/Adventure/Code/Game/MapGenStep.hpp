#pragma once

#include "Engine/ThirdParty/tinyxml2.h"
#include "Engine/Math/IntRange.hpp"
#include <map>

class Map;

class MapGenStep
{
public:
	virtual ~MapGenStep();
	MapGenStep( const tinyxml2::XMLElement& genStepXmlElement );
	void Generate( Map& map );
	virtual void Run( Map& map ) = 0; // "pure virtual", MUST be overridden by subclasses

	static MapGenStep* CreateMapGenStep( const tinyxml2::XMLElement& genStepXmlElement );

public:
	IntRange m_iterations = IntRange(1);
	float m_chanceToRun = 1.f;

private:
	std::string	m_name;
};

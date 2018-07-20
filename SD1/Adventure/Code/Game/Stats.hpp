#pragma once

#include "Engine/ThirdParty/tinyxml2.h"
#include "Engine/Core/Rgba.hpp"
#include <string>

enum STAT_ID
{
	STAT_FIRE_ATK,
	STAT_FIRE_DEF,
	STAT_WATER_ATK,
	STAT_WATER_DEF,
	STAT_EARTH_ATK,
	STAT_EARTH_DEF,
	NUM_STATS
};

class Stats
{
public:
	~Stats() {};
	Stats();
	const Stats operator+( const Stats& statsToAdd ) const;

	static STAT_ID GetStatIDForName(std::string name);
	static std::string GetNameForStatID(STAT_ID stat);
	static int ResolveDamageDealt(Stats attacker, Stats defender);
	static Rgba GetElementColor(int fire, int water, int earth);

public:
	int m_statValues[STAT_ID::NUM_STATS];
};

Stats ParseXmlAttribute( const tinyxml2::XMLElement& element, const Stats defaultValue );

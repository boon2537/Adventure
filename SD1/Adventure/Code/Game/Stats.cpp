#include "Game/Stats.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/XmlUtilities.hpp"


Stats::Stats()
{
	for (int i = 0; i < STAT_ID::NUM_STATS; i++)
	{
		m_statValues[i] = 0;
	}
}

const Stats Stats::operator+(const Stats & statsToAdd) const
{
	Stats output;
	for (int i = 0; i < STAT_ID::NUM_STATS; i++)
	{
		output.m_statValues[i] = m_statValues[i] + statsToAdd.m_statValues[i];
	}

	return output;
}

STAT_ID Stats::GetStatIDForName(std::string name)
{
	if (name == "fire_atk")		return STAT_ID::STAT_FIRE_ATK;
	if (name == "fire_def")		return STAT_ID::STAT_FIRE_DEF;
	if (name == "water_atk")	return STAT_ID::STAT_WATER_ATK;
	if (name == "water_def")	return STAT_ID::STAT_WATER_DEF;
	if (name == "earth_atk")	return STAT_ID::STAT_EARTH_ATK;
	if (name == "earth_def")	return STAT_ID::STAT_EARTH_DEF;

	ERROR_RECOVERABLE("Stat name not found");
	return STAT_ID::STAT_FIRE_ATK;
}

std::string Stats::GetNameForStatID(STAT_ID stat)
{
	switch (stat)
	{
		case STAT_FIRE_ATK:		return "fire_atk";
		case STAT_FIRE_DEF:		return "fire_def";
		case STAT_WATER_ATK:	return "water_atk";
		case STAT_WATER_DEF:	return "water_def";
		case STAT_EARTH_ATK:	return "earth_atk";
		case STAT_EARTH_DEF:	return "earth_def";
	}

	ERROR_RECOVERABLE("Cannot find STAT_ID to look for name");
	return "";
}

int Stats::ResolveDamageDealt(Stats attacker, Stats defender)
{
	int fireDamageTaken = 0;
	if (attacker.m_statValues[STAT_ID::STAT_FIRE_ATK] > 0)
		fireDamageTaken = max(attacker.m_statValues[STAT_ID::STAT_FIRE_ATK] - defender.m_statValues[STAT_ID::STAT_FIRE_DEF], 0);

	int waterDamageTaken = 0;
	if (attacker.m_statValues[STAT_ID::STAT_WATER_ATK] > 0)
		waterDamageTaken = max(attacker.m_statValues[STAT_ID::STAT_WATER_ATK] - defender.m_statValues[STAT_ID::STAT_WATER_DEF], 0);

	int earthDamageTaken = 0;
	if (attacker.m_statValues[STAT_ID::STAT_EARTH_ATK] > 0)
		earthDamageTaken = max(attacker.m_statValues[STAT_ID::STAT_EARTH_ATK] - defender.m_statValues[STAT_ID::STAT_EARTH_DEF], 0);

	return fireDamageTaken + waterDamageTaken + earthDamageTaken;
}

Rgba Stats::GetElementColor(int fire, int water, int earth)
{
	float highest = 100.f;
	unsigned char r = (unsigned char) RangeMapFloat((float) fire, 0, highest, 180, 255);
	unsigned char g = (unsigned char) RangeMapFloat((float) earth, 0, highest, 180, 255);
	unsigned char b = (unsigned char) RangeMapFloat((float) water, 0, highest, 180, 255);

	if (fire <= 0)
		r = 0;
	if (water <= 0)
		b = 0;
	if (earth <= 0)
		g = 0;
	if (fire <= 0  && water <= 0 && earth <= 0)
		return Rgba(180, 180, 180, 255);

	return Rgba(r, g, b, 255);
}

Stats ParseXmlAttribute(const tinyxml2::XMLElement& element, const Stats defaultValue)
{
	Stats output = defaultValue;
	for (int i = 0; i < STAT_ID::NUM_STATS; i++)
	{
		output.m_statValues[i] = ParseXmlAttribute(element, Stats::GetNameForStatID((STAT_ID) i).c_str(), output.m_statValues[i]);
	}

	return output;
}

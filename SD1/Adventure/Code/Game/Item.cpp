#include "Game/Item.hpp"
#include "Engine/Math/MathUtils.hpp"

Item::~Item()
{
}

Item::Item(const std::string name, ItemDefinition * def, Map * attachedMap)
	: Entity(name, def, attachedMap),
	m_itemDef(def)
{
	for (int i = 0; i < STAT_ID::NUM_STATS; i++)
	{
		m_stats.m_statValues[i] = GetRandomIntInRange(m_itemDef->m_minStats.m_statValues[i], m_itemDef->m_maxStats.m_statValues[i]);
	}
}

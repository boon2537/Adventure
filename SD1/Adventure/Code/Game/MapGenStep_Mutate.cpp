#include "Game/MapGenStep_Mutate.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

MapGenStep_Mutate::~MapGenStep_Mutate()
{
}

MapGenStep_Mutate::MapGenStep_Mutate(const tinyxml2::XMLElement& genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	m_chanceToMutate = ParseXmlAttribute(genStepXmlElement, "chanceToMutate", m_chanceToMutate);
	m_toType = ParseXmlAttribute(genStepXmlElement, "toType", m_toType);
	m_ifType = ParseXmlAttribute(genStepXmlElement, "ifType", m_ifType);
}

void MapGenStep_Mutate::Run(Map& map)
{
	for (int y = 0; y < map.m_mapHeight; y++)
	{
		for (int x = 0; x < map.m_mapWidth; x++)
		{
			if ((m_ifType == nullptr || m_ifType == map.GetTile(x, y).m_tileDef) && CheckRandomChance(m_chanceToMutate))
				map.GetTile(x, y).m_tileDef = m_toType;
		}
	}
}

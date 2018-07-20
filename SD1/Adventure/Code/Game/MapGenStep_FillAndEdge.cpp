#include "Game/MapGenStep_FillAndEdge.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

MapGenStep_FillAndEdge::~MapGenStep_FillAndEdge()
{
}

MapGenStep_FillAndEdge::MapGenStep_FillAndEdge(const tinyxml2::XMLElement& genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	m_fillTile = ParseXmlAttribute(genStepXmlElement, "fillTile", m_fillTile);
	m_edgeTile = ParseXmlAttribute(genStepXmlElement, "edgeTile", m_edgeTile);
}

void MapGenStep_FillAndEdge::Run(Map& map)
{
	for (int y = 0; y < map.m_mapHeight; y++)
	{
		for (int x = 0; x < map.m_mapWidth; x++)
		{
			if (y == 0 || x == 0 || y + 1 == map.m_mapHeight || x + 1 == map.m_mapWidth)
				map.GetTile(x, y).ChangeTileDef(m_edgeTile);
			else if (m_fillTile != nullptr)
				map.GetTile(x, y).ChangeTileDef(m_fillTile);
		}
	}
}

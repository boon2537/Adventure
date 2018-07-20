#include "Game/MapGenStep_CellularAutomata.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

MapGenStep_CellularAutomata::~MapGenStep_CellularAutomata()
{
}

MapGenStep_CellularAutomata::MapGenStep_CellularAutomata(const tinyxml2::XMLElement& genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	m_chanceToMutate = ParseXmlAttribute(genStepXmlElement, "chanceToMutate", m_chanceToMutate);
	m_toType = ParseXmlAttribute(genStepXmlElement, "changeToType", m_toType);
	m_ifType = ParseXmlAttribute(genStepXmlElement, "ifType", m_ifType);
	m_ifNeighborCount = ParseXmlAttribute(genStepXmlElement, "ifNeighborCount", IntRange(1));
	m_ifNeighborType = ParseXmlAttribute(genStepXmlElement, "ifNeighborType", m_ifNeighborType);
}

void MapGenStep_CellularAutomata::Run(Map& map)
{
	std::vector<IntVector2> tileToChange;

	for (int y = 0; y < map.m_mapHeight; y++)
	{
		for (int x = 0; x < map.m_mapWidth; x++)
		{
			if ((m_ifType == nullptr || m_ifType == map.GetTile(x, y).m_tileDef) && CheckRandomChance(m_chanceToMutate) && PassNeighborChecks(map, x, y))
			{
				tileToChange.push_back(IntVector2(x, y));
			}
		}
	}

	for (unsigned int i = 0; i < tileToChange.size(); i++)
		map.GetTile(tileToChange[i].x, tileToChange[i].y).m_tileDef = m_toType;
}

bool MapGenStep_CellularAutomata::PassNeighborChecks(Map& map, int x, int y)
{
	int neighborCountToPass = GetRandomIntInRange(m_ifNeighborCount.min, m_ifNeighborCount.max);
	int currentNeighborCount = 0;

	//north
	if (map.CanGetTile(x, y + 1) && map.GetTile(x, y + 1).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//south
	if (map.CanGetTile(x, y - 1) && map.GetTile(x, y - 1).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//east
	if (map.CanGetTile(x + 1, y) && map.GetTile(x + 1, y).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//west
	if (map.CanGetTile(x - 1, y) && map.GetTile(x - 1, y).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//NE
	if (map.CanGetTile(x + 1, y + 1) && map.GetTile(x + 1, y + 1).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//NW
	if (map.CanGetTile(x - 1, y + 1) && map.GetTile(x - 1, y + 1).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//SE
	if (map.CanGetTile(x + 1, y - 1) && map.GetTile(x + 1, y - 1).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;
	//SW
	if (map.CanGetTile(x - 1, y - 1) && map.GetTile(x - 1, y - 1).m_tileDef == m_ifNeighborType)
		currentNeighborCount++;

	return currentNeighborCount >= neighborCountToPass;
}

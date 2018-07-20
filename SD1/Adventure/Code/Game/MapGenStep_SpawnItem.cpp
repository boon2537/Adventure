#include "Game/MapGenStep_SpawnItem.hpp"
#include "Game/Tile.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

MapGenStep_SpawnItem::~MapGenStep_SpawnItem()
{
}

MapGenStep_SpawnItem::MapGenStep_SpawnItem(const tinyxml2::XMLElement & genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	m_itemToSpawn = ParseXmlAttribute(genStepXmlElement, "type", "");
	m_onTile = ParseXmlAttribute(genStepXmlElement, "onTile", m_onTile);
}

void MapGenStep_SpawnItem::Run(Map & map)
{
	Vector2 spawnPosition = Vector2(0, 0);
	if (m_onTile != nullptr)
	{
		Tile* tile = map.GetRandomTileOfDef(m_onTile);
		spawnPosition =  Vector2((float) tile->m_tileCoords.x + 0.5f, (float) tile->m_tileCoords.y + 0.5f);
	}
	//TODO: items on random tiles which permit it

	map.SpawnItem(m_itemToSpawn, spawnPosition, 0);
}

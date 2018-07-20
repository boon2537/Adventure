#include "Game/MapGenStep_SpawnActor.hpp"
#include "Game/Tile.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

MapGenStep_SpawnActor::~MapGenStep_SpawnActor()
{
}

MapGenStep_SpawnActor::MapGenStep_SpawnActor(const tinyxml2::XMLElement & genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	m_actorToSpawn = ParseXmlAttribute(genStepXmlElement, "type", "");
	m_onTile = ParseXmlAttribute(genStepXmlElement, "onTile", m_onTile);
}

void MapGenStep_SpawnActor::Run(Map & map)
{
	Vector2 spawnPosition = Vector2(0, 0);
	if (m_onTile != nullptr)
	{
		Tile* tile = map.GetRandomTileOfDef(m_onTile);
		spawnPosition =  Vector2((float) tile->m_tileCoords.x + 0.5f, (float) tile->m_tileCoords.y + 0.5f);
	}
	//TODO: actors on random tiles which permit it

	map.SpawnActor(m_actorToSpawn, spawnPosition, 0);
}

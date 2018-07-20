#include "Game/Adventure.hpp"
#include "Game/Map.hpp"
#include "Game/Actor.hpp"
#include "Game/GameCommon.hpp"

Adventure::~Adventure()
{
	std::map<std::string, Map*>::iterator it;
	for (it = m_mapDict.begin(); it != m_mapDict.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
}

Adventure::Adventure(const std::string name, AdventureDefinition * def)
	: m_name(name),
	m_adventureDef(def)
{
	//Generate maps
	std::map<std::string, MapDefinition*>::iterator it;
	for (it = def->m_mapsToGenerate.begin(); it != def->m_mapsToGenerate.end(); it++)
	{
		m_mapDict[it->first] = new Map(it->first, it->second);
	}

	m_currentMap = m_mapDict[def->m_mapToStart];

	//Generate Portals
	for (unsigned int i = 0; i < def->m_portalsToSpawn.size(); i++)
	{
		AdventureDefinition::PortalToSpawn* portalData = def->m_portalsToSpawn[i];
		Tile* tileToSpawn = nullptr;
		Map* mapToSpawn = nullptr;
		Portal* beginPortal = nullptr;
		Portal* endPortal = nullptr;
		if (portalData->m_type != nullptr)
		{
			mapToSpawn = m_mapDict[portalData->m_myMapToSpawn];
			tileToSpawn = mapToSpawn->GetRandomTileOfDef(portalData->m_myTileDef);

			if (tileToSpawn != nullptr)
				beginPortal = mapToSpawn->SpawnPortal(portalData->m_type->m_name, Vector2(tileToSpawn->m_tileCoords.x + 0.5f, tileToSpawn->m_tileCoords.y + 0.5f), 0);
		}

		if (portalData->m_reciprocalType != nullptr)
		{
			mapToSpawn = m_mapDict[portalData->m_reciprocalMapToSpawn];
			tileToSpawn = mapToSpawn->GetRandomTileOfDef(portalData->m_reciprocalTileDef);

			if (tileToSpawn != nullptr)
				endPortal = mapToSpawn->SpawnPortal(portalData->m_reciprocalType->m_name, Vector2(tileToSpawn->m_tileCoords.x + 0.5f, tileToSpawn->m_tileCoords.y + 0.5f), 0);
		}

		if (beginPortal != nullptr && endPortal != nullptr)
		{
			beginPortal->m_linkedPortal = endPortal;
			endPortal->m_linkedPortal = beginPortal;
		}
	}

	//Generate Actors
	for (unsigned int i = 0; i < def->m_actorsToSpawn.size(); i++)
	{
		AdventureDefinition::ActorToSpawn* actorData = def->m_actorsToSpawn[i];
		Tile* tileToSpawn = nullptr;
		if (actorData->m_actorDef != nullptr)
		{
			tileToSpawn = m_mapDict[actorData->m_mapToSpawnAt]->GetRandomTileOfDef(actorData->m_spawnOnTileDef);
			if (tileToSpawn != nullptr)
				m_mapDict[actorData->m_mapToSpawnAt]->SpawnActor(actorData->m_actorDef->m_name, Vector2(tileToSpawn->m_tileCoords.x + 0.5f, tileToSpawn->m_tileCoords.y + 0.5f), 0);
		}
	}
}

void Adventure::Update(float deltaTime)
{
	m_currentMap->Update(deltaTime);
}

void Adventure::LateUpdate(float deltaTime)
{
	m_currentMap->LateUpdate(deltaTime);
}

void Adventure::Render()
{
	m_currentMap->Render();
}

void Adventure::SpawnPlayerInAdventure(Player* player)
{
	IntVector2 tileCoordsToSpawn = m_currentMap->GetRandomTileOfDef(m_adventureDef->m_tileDefToStart)->m_tileCoords;
	player->m_position = Vector2(tileCoordsToSpawn.x + 0.5f, tileCoordsToSpawn.y + 0.5f);

	m_player = player;
	m_currentMap->AttachPlayerToMap(player);
}

void Adventure::SwitchMap(std::string nextMap)
{
	Map* previousMap = m_currentMap;
	m_currentMap = m_mapDict[nextMap];

	if (m_player != nullptr)
	{
		previousMap->DeattachPlayerFromMap(m_player);
		m_currentMap->AttachPlayerToMap(m_player);
	}
}

void Adventure::CheckKillObjective(Actor* target)
{
	if (m_adventureDef->m_killObjective != nullptr && target->m_actorDef == m_adventureDef->m_killObjective)
	{
		g_theGame->StartTransitionToState(GAME_STATE::VICTORY);
	}
}

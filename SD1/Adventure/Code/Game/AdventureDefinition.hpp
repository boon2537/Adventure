#pragma once

#include "Game/MapDefinition.hpp"
#include "Game/ActorDefinition.hpp"
#include "Game/PortalDefinition.hpp"
#include "Game/TileDefinition.hpp"
#include "Engine/ThirdParty/tinyxml2.h"

class AdventureDefinition
{
public:
	struct ActorToSpawn
	{
		ActorToSpawn(std::string mapAttached, const tinyxml2::XMLElement& element);
		
		std::string m_mapToSpawnAt;
		ActorDefinition* m_actorDef = nullptr;
		TileDefinition* m_spawnOnTileDef = nullptr;
	};

	struct PortalToSpawn
	{
		PortalToSpawn(std::string mapAttached, const tinyxml2::XMLElement& element);

		PortalDefinition* m_type = nullptr;
		PortalDefinition* m_reciprocalType = nullptr;
		std::string m_myMapToSpawn;
		TileDefinition* m_myTileDef = nullptr;
		std::string m_reciprocalMapToSpawn;
		TileDefinition* m_reciprocalTileDef = nullptr;
	};

	~AdventureDefinition();
	AdventureDefinition(const tinyxml2::XMLElement& element);

	static void LoadAdventureDefinitions( const std::string& path);
	static AdventureDefinition* GetAdventureDefinition( const std::string& defName);

public:
	static std::map<std::string, AdventureDefinition*> s_definitions;

	std::string m_name;
	std::string m_title;
	std::string m_mapToStart;
	TileDefinition* m_tileDefToStart;
	ActorDefinition* m_killObjective = nullptr; //TODO: refactor this for more complex objective later
	std::map<std::string, MapDefinition*> m_mapsToGenerate;
	std::vector<ActorToSpawn*> m_actorsToSpawn;
	std::vector<PortalToSpawn*> m_portalsToSpawn;
};

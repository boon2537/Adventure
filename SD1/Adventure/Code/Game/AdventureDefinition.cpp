#include "Game/AdventureDefinition.hpp"
#include "Engine/Core/XmlUtilities.hpp"

std::map<std::string, AdventureDefinition*> AdventureDefinition::s_definitions;

AdventureDefinition::ActorToSpawn::ActorToSpawn(std::string mapAttached, const tinyxml2::XMLElement& element)
	: m_mapToSpawnAt(mapAttached)
{
	m_actorDef = ParseXmlAttribute(element, "type", m_actorDef);
	m_spawnOnTileDef = ParseXmlAttribute(element, "onTileType", m_spawnOnTileDef);
}

AdventureDefinition::PortalToSpawn::PortalToSpawn(std::string mapAttached, const tinyxml2::XMLElement& element)
	: m_myMapToSpawn(mapAttached)
{
	m_type = PortalDefinition::GetPortalDefinition(ParseXmlAttribute(element, "type", "empty"));
	m_myTileDef = ParseXmlAttribute(element, "onTileType", m_myTileDef);

	//TODO: make one way portal later?
	m_reciprocalType = PortalDefinition::GetPortalDefinition(ParseXmlAttribute(element, "reciprocalType", "empty"));
	m_reciprocalTileDef = ParseXmlAttribute(element, "toTileType", m_reciprocalTileDef);
	m_reciprocalMapToSpawn = ParseXmlAttribute(element, "toMap", "empty");
}

AdventureDefinition::~AdventureDefinition()
{
	for (unsigned int i = 0; i < m_portalsToSpawn.size(); i++)
	{
		delete m_portalsToSpawn[i];
		m_portalsToSpawn[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_actorsToSpawn.size(); i++)
	{
		delete m_actorsToSpawn[i];
		m_actorsToSpawn[i] = nullptr;
	}
}

AdventureDefinition::AdventureDefinition(const tinyxml2::XMLElement& element)
{
	m_name = ParseXmlAttribute(element, "name", m_name);
	m_title = ParseXmlAttribute(element, "title", m_title);

	const tinyxml2::XMLElement* elementToUse = element.FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string elementName = elementToUse->Name();
		if (elementName == "StartConditions")
		{
			m_mapToStart = ParseXmlAttribute(*elementToUse, "startMap", m_mapToStart);
			m_tileDefToStart = ParseXmlAttribute(*elementToUse, "startOnTileType", m_tileDefToStart);
		}
		else if (elementName == "VictoryConditions")
		{
			m_killObjective = ParseXmlAttribute(*elementToUse, "haveDied", m_killObjective);
		}
		else if (elementName == "Map")
		{
			std::string mapName = ParseXmlAttribute(*elementToUse, "name", "empty");
			m_mapsToGenerate[mapName] = MapDefinition::GetDefinition(ParseXmlAttribute(*elementToUse, "mapDefinition", "empty"));

			const tinyxml2::XMLElement* child = elementToUse->FirstChildElement();
			while (child != nullptr)
			{
				std::string childName = child->Name();
				if (childName == "Portal")
					m_portalsToSpawn.push_back(new PortalToSpawn(mapName, *child));
				else if (childName == "Actor")
					m_actorsToSpawn.push_back(new ActorToSpawn(mapName, *child));

				child = child->NextSiblingElement();
			}
		}
		elementToUse = elementToUse->NextSiblingElement();
	}
}

void AdventureDefinition::LoadAdventureDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new AdventureDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}

AdventureDefinition* AdventureDefinition::GetAdventureDefinition(const std::string & defName)
{
	std::map<std::string, AdventureDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

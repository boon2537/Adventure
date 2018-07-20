#include "Game/MapDefinition.hpp"
#include "Engine/Core/XmlUtilities.hpp"

std::map<std::string, MapDefinition*> MapDefinition::s_definitions;

MapDefinition::~MapDefinition()
{
}

MapDefinition::MapDefinition(const tinyxml2::XMLElement & element)
{
	m_name = ParseXmlAttribute(element, "name", "empty");

	m_defaultTile = ParseXmlAttribute(element, "defaultTile", m_defaultTile);
	m_widthRange = ParseXmlAttribute(element, "width", IntRange(16, 32));
	m_heightRange = ParseXmlAttribute(element, "height", IntRange(16, 32));

	//load MapGenSteps
	const tinyxml2::XMLElement* elementToUse = element.FirstChildElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		m_MapGenSteps.push_back(MapGenStep::CreateMapGenStep(*elementToUse));
		elementToUse = elementToUse->NextSiblingElement();
	}
}

void MapDefinition::LoadDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChild()->ToElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new MapDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}

MapDefinition * MapDefinition::GetDefinition(const std::string & defName)
{
	std::map<std::string, MapDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

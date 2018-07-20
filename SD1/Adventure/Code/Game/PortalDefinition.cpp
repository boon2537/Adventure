#include "Game/PortalDefinition.hpp"
#include "Engine/Core/XmlUtilities.hpp"

std::map<std::string, PortalDefinition*> PortalDefinition::s_definitions;

PortalDefinition::~PortalDefinition()
{
}

PortalDefinition::PortalDefinition(const tinyxml2::XMLElement & element)
	: EntityDefinition(element)
{
}

void PortalDefinition::LoadPortalDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new PortalDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}

PortalDefinition * PortalDefinition::GetPortalDefinition(const std::string & defName)
{
	std::map<std::string, PortalDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

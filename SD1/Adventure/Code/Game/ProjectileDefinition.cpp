#include "Game/ProjectileDefinition.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

std::map<std::string, ProjectileDefinition*> ProjectileDefinition::s_definitions;

ProjectileDefinition::~ProjectileDefinition()
{
}

ProjectileDefinition::ProjectileDefinition(const tinyxml2::XMLElement & element)
	: EntityDefinition(element)
{
	const tinyxml2::XMLElement* elementToUse = element.FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string elementName = elementToUse->Name();
		if (elementName == "Damage")
		{
			m_damageRange = ParseXmlAttribute(*elementToUse, "amount", IntRange(0));
			m_liveTime = ParseXmlAttribute(*elementToUse, "liveTime", m_liveTime);
			std::string modeName = ParseXmlAttribute(*elementToUse, "mode", "other");
			m_damageMode = GetDamageModeForName(modeName);
			m_targetFaction = ParseXmlAttribute(*elementToUse, "targetFaction", "");
		}

		elementToUse = elementToUse->NextSiblingElement();
	}
}

void ProjectileDefinition::LoadProjectileDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new ProjectileDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}


ProjectileDefinition * ProjectileDefinition::GetProjectileDefinition(const std::string & defName)
{
	std::map<std::string, ProjectileDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

DAMAGE_MODE ProjectileDefinition::GetDamageModeForName(std::string name)
{
	if (name == "target")		return DAMAGE_MODE::DAMAGE_MODE_TARGET_FACTION;
	if (name == "other")		return DAMAGE_MODE::DAMAGE_MODE_OTHER_FACTION;
	if (name == "all")			return DAMAGE_MODE::DAMAGE_MODE_ALL;

	ERROR_RECOVERABLE("Damage mode name not found");
	return DAMAGE_MODE::DAMAGE_MODE_OTHER_FACTION;
}

std::string ProjectileDefinition::GetNameForDamageMode(DAMAGE_MODE mode)
{
	switch (mode)
	{
	case DAMAGE_MODE_TARGET_FACTION:	return "target";
	case DAMAGE_MODE_OTHER_FACTION:		return "other";
	case DAMAGE_MODE_ALL:				return "all";
	}

	ERROR_RECOVERABLE("Cannot find DAMAGE_MODE to look for name");
	return "";
}

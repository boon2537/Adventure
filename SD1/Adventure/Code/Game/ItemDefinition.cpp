#include "Game/ItemDefinition.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/Math/MathUtils.hpp"

std::map<std::string, ItemDefinition*> ItemDefinition::s_definitions;
std::map<std::string, std::vector<ItemDefinition*>> ItemDefinition::s_lootTable;

ItemDefinition::~ItemDefinition()
{
}

ItemDefinition::ItemDefinition(const tinyxml2::XMLElement & element)
	: EntityDefinition(element)
{
	bool maxStatFound = false;

	const tinyxml2::XMLElement* elementToUse = element.FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string elementName = elementToUse->Name();
		if (elementName == "Equip")
		{
			m_equipSlot = (EQUIP_SLOT) ParseXmlAttribute(*elementToUse, "slot", m_equipSlot);
			m_equippedDrawBound = ParseXmlAttribute(*elementToUse, "equippedDrawBounds", AABB2(-0.5f, -0.5f, 0.5f, 0.5f));
			m_dropOnDeath = ParseXmlAttribute(*elementToUse, "dropOnDeath", false);
		}
		else if (elementName == "MinStats")
		{
			m_minStats = ParseXmlAttribute(*elementToUse, m_minStats);
		}
		else if (elementName == "MaxStats")
		{
			m_maxStats = ParseXmlAttribute(*elementToUse, m_maxStats);
			maxStatFound = true;
		}

		elementToUse = elementToUse->NextSiblingElement();
	}

	if (!maxStatFound)
		m_maxStats = m_minStats;
}

void ItemDefinition::LoadItemDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new ItemDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}

ItemDefinition * ItemDefinition::GetItemDefinition(const std::string & defName)
{
	std::map<std::string, ItemDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

void ItemDefinition::LoadLootTable(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_lootTable[name] = std::vector<ItemDefinition*>();
		Strings loots = Split(ParseXmlAttribute(*elementToUse, "items", ""), ',');

		for (unsigned int i = 0; i < loots.size(); i++)
		{
			s_lootTable[name].push_back(GetItemDefinition(loots[i]));
		}

		elementToUse = elementToUse->NextSiblingElement();
	}
}

ItemDefinition * ItemDefinition::GetRandomItemFromLootTable(const std::string & lootName)
{
	std::map<std::string, std::vector<ItemDefinition*>>::iterator search = s_lootTable.find(lootName);
	if (search != s_lootTable.end())
	{
		std::vector<ItemDefinition*>& loot = search->second;
		return loot[GetRandomIntLessThan(loot.size())];
	}

	return nullptr;
}

#include "Game/ActorDefinition.hpp"
#include "Game/Stats.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

std::map<std::string, ActorDefinition*> ActorDefinition::s_definitions;

ActorDefinition::~ActorDefinition()
{
}

ActorDefinition::ActorDefinition(const tinyxml2::XMLElement & element)
	: EntityDefinition(element)
{
	m_faction = ParseXmlAttribute(element, "faction", "Empty");

	const tinyxml2::XMLElement* elementToUse = element.FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string elementName = elementToUse->Name();
		if (elementName == "Health")
		{
			m_maxHealth = ParseXmlAttribute(*elementToUse, "max", m_maxHealth);
			m_startHealth = ParseXmlAttribute(*elementToUse, "start", m_startHealth);
		}
		else if (elementName == "Attack")
		{
			m_attackRate = ParseXmlAttribute(*elementToUse, "rate", m_attackRate);
			m_meleeOffset = ParseXmlAttribute(*elementToUse, "meleeOffset", m_meleeOffset);
			m_rangedOffset = ParseXmlAttribute(*elementToUse, "rangedOffset", m_rangedOffset);
			m_projectileName = ParseXmlAttribute(*elementToUse, "projectileName", m_projectileName);
			m_attackDelay = ParseXmlAttribute(*elementToUse, "attackDelay", m_attackDelay);
		}
		else if (elementName == "Behaviors")
		{
			const tinyxml2::XMLElement* behaviourChild = elementToUse->FirstChildElement();
			while (behaviourChild != nullptr)
			{
				m_behaviors.push_back(AIBehavior::CreateAIBehavior(*behaviourChild));
				behaviourChild = behaviourChild->NextSiblingElement();
			}
		}
		else if (elementName == "Stats")
		{
			m_baseStats = ParseXmlAttribute(*elementToUse, m_baseStats);
		}
		else if (elementName == "Loot")
		{
			std::pair<std::string, float> data;
			data.first = ParseXmlAttribute(*elementToUse, "name", "");
			data.second = ParseXmlAttribute(*elementToUse, "chance", 1.f);
			m_lootToSpawn.push_back(data);
		}
		else if (elementName == "Slot")
		{
			m_hasAlternateSlot = ParseXmlAttribute(*elementToUse, "alt_slot", false);
		}
		else if (elementName == "Physics")
		{
			m_enableActorVsActorPhysics = ParseXmlAttribute(*elementToUse, "enableActorVsActor", true);
			m_mass = ParseXmlAttribute(*elementToUse, "mass", m_mass);
		}
		elementToUse = elementToUse->NextSiblingElement();
	}
}

void ActorDefinition::LoadActorDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new ActorDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}

ActorDefinition * ActorDefinition::GetActorDefinition(const std::string & defName)
{
	std::map<std::string, ActorDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

ActorDefinition * ParseXmlAttribute(const tinyxml2::XMLElement & element, const char * attributeName, ActorDefinition* defaultValue)
{
	std::string actorName = ParseXmlAttribute(element, attributeName, "empty");

	ActorDefinition* output = ActorDefinition::GetActorDefinition(actorName);
	if (output != nullptr)
		return ActorDefinition::GetActorDefinition(actorName);

	return defaultValue;
}

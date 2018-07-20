#include "Game/GameCommon.hpp"
#include "Game/EntityDefinition.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/Core/StringUtils.hpp"

EntityDefinition::~EntityDefinition()
{
}

EntityDefinition::EntityDefinition(const tinyxml2::XMLElement & element)
{
	m_name = ParseXmlAttribute(element, "name", "empty");

	const tinyxml2::XMLElement* elementToUse = element.FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string elementName = elementToUse->Name();
		if (elementName == "Size")
		{
			m_physicsRadius = ParseXmlAttribute(*elementToUse, "physicsRadius", m_physicsRadius);
			if (elementToUse->FindAttribute("localDrawBounds") != 0)
				m_localDrawBounds = ParseXmlAttribute(*elementToUse, "localDrawBounds", m_localDrawBounds);
			else
			{
				float drawRadius = 0.5f; //default is 0.5 radius
				drawRadius = ParseXmlAttribute(*elementToUse, "drawRadius", drawRadius);
				m_localDrawBounds = AABB2(-drawRadius, -drawRadius, drawRadius, drawRadius);
			}

			m_defaultTint = ParseXmlAttribute(*elementToUse, "tint", Rgba::white);
		}
		else if (elementName == "Movement")
		{
			const tinyxml2::XMLElement* moveElement = elementToUse->FirstChildElement();
			std::string moveElementName = moveElement->Name();
			if (moveElementName == "Walk")
				canWalk = true;
			else if (moveElementName == "Fly")
				canFly = true;
			else if (moveElementName == "Swim")
				canSwim = true;

			m_speed = ParseXmlAttribute(*moveElement, "speed", m_speed);
		}
		else if (elementName == "SpriteAnimSet")
			m_spriteAnimSetDef = new SpriteAnimSetDefinition(*elementToUse, *g_theRenderer);

		elementToUse = elementToUse->NextSiblingElement();
	}
}

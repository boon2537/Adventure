#include "Game/TileDefinition.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/XmlUtilities.hpp"

std::map<std::string, TileDefinition*> TileDefinition::s_definitions;

TileDefinition::~TileDefinition()
{
}

TileDefinition::TileDefinition(const tinyxml2::XMLElement & element)
{
	m_name = ParseXmlAttribute(element, "name", "empty");
	IntVector2 spriteCoords = ParseXmlAttribute(element, "spriteCoords", IntVector2(0, 0));
	m_baseTextureCoords = g_terrainSpriteSheet->GetTexCoordsForSpriteCoords(spriteCoords);
	m_baseSpriteTint = ParseXmlAttribute(element, "spriteTint", Rgba::white);
	m_hasOverlayTexture = element.FindAttribute("overlaySpriteCoords") != 0;
	spriteCoords = ParseXmlAttribute(element, "overlaySpriteCoords", IntVector2(0, 0));
	m_overlayTextureCoords = g_terrainSpriteSheet->GetTexCoordsForSpriteCoords(spriteCoords);
	m_overlaySpriteTint = ParseXmlAttribute(element, "overlaySpriteTint", Rgba::white);
	m_texelColor = ParseXmlAttribute(element, "texelColor", Rgba::white);
	m_allowsSight = ParseXmlAttribute(element, "allowsSight", true);
	m_allowsWalking = ParseXmlAttribute(element, "allowsWalking", true);
	m_allowsFlying = ParseXmlAttribute(element, "allowsFlying", true);
	m_allowsSwimming = ParseXmlAttribute(element, "allowsSwimming", true);
}

void TileDefinition::LoadDefinitions(const std::string & path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* elementToUse = doc.RootElement()->FirstChildElement();
	while (elementToUse != nullptr)
	{
		std::string name = ParseXmlAttribute(*elementToUse, "name", "empty");
		s_definitions[name] = new TileDefinition(*elementToUse);
		elementToUse = elementToUse->NextSiblingElement();
	}
}

TileDefinition* TileDefinition::GetDefinition(const std::string & defName)
{
	std::map<std::string, TileDefinition*>::iterator search = s_definitions.find(defName);
	if (search != s_definitions.end())
	{
		return search->second;
	}

	return nullptr;
}

TileDefinition* TileDefinition::GetDefinitionFromTexel(const Rgba& texel)
{
	std::map<std::string, TileDefinition*>::iterator it;
	for (it = s_definitions.begin(); it != s_definitions.end(); it++)
	{
		Rgba colorTocheck = it->second->m_texelColor;
		if (colorTocheck.r == texel.r && colorTocheck.g == texel.g && colorTocheck.b == texel.b)
			return it->second;
	}

	return nullptr;
}

TileDefinition* ParseXmlAttribute(const tinyxml2::XMLElement & element, const char * attributeName, TileDefinition* defaultValue)
{
	std::string tileName = ParseXmlAttribute(element, attributeName, "empty");

	TileDefinition* output = TileDefinition::GetDefinition(tileName);
	if (output != nullptr)
		return output;

	return defaultValue;
}

#pragma once

#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/ThirdParty/tinyxml2.h"
#include <map>

class TileDefinition
{
public:
	~TileDefinition();
	TileDefinition(const tinyxml2::XMLElement & element);

	static void LoadDefinitions( const std::string& path);
	static TileDefinition* GetDefinition( const std::string& defName);
	static TileDefinition* GetDefinitionFromTexel( const Rgba& texel );

public:
	static std::map<std::string, TileDefinition*> s_definitions;

	std::string m_name;
	AABB2 m_baseTextureCoords;
	Rgba m_baseSpriteTint;
	bool m_hasOverlayTexture = false;
	AABB2 m_overlayTextureCoords;
	Rgba m_overlaySpriteTint;
	Rgba m_texelColor;
	bool m_allowsSight;
	bool m_allowsWalking;
	bool m_allowsFlying;
	bool m_allowsSwimming;
};

TileDefinition* ParseXmlAttribute( const tinyxml2::XMLElement& element, const char* attributeName, TileDefinition* defaultValue );
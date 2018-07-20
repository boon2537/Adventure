#pragma once

#include "Game/EntityDefinition.hpp"

class PortalDefinition : public EntityDefinition
{
public:
	~PortalDefinition();
	PortalDefinition(const tinyxml2::XMLElement & element);

	static void LoadPortalDefinitions( const std::string& path);
	static PortalDefinition* GetPortalDefinition( const std::string& defName);

public:
	static std::map<std::string, PortalDefinition*> s_definitions;
};

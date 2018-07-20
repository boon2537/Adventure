#pragma once

#include "Game/EntityDefinition.hpp"

enum DAMAGE_MODE
{
	DAMAGE_MODE_TARGET_FACTION,
	DAMAGE_MODE_OTHER_FACTION,
	DAMAGE_MODE_ALL
};

class ProjectileDefinition : public EntityDefinition
{
public:
	~ProjectileDefinition();
	ProjectileDefinition(const tinyxml2::XMLElement & element);

	static void LoadProjectileDefinitions( const std::string& path);
	static ProjectileDefinition* GetProjectileDefinition( const std::string& defName);

	static DAMAGE_MODE GetDamageModeForName(std::string name);
	static std::string GetNameForDamageMode(DAMAGE_MODE mode);

public:
	static std::map<std::string, ProjectileDefinition*> s_definitions;

	IntRange m_damageRange;
	float m_liveTime = 100;
	DAMAGE_MODE m_damageMode = DAMAGE_MODE::DAMAGE_MODE_OTHER_FACTION;
	std::string m_targetFaction;
};

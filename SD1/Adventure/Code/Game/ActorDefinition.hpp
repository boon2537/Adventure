#pragma once

#include "Game/EntityDefinition.hpp"
#include "Game/Stats.hpp"
#include "Game/AIBehavior.hpp"
#include "Engine/Core/EngineCommon.hpp"

class ActorDefinition : public EntityDefinition
{
public:
	~ActorDefinition();
	ActorDefinition(const tinyxml2::XMLElement & element);

	static void LoadActorDefinitions( const std::string& path);
	static ActorDefinition* GetActorDefinition( const std::string& defName);

public:
	static std::map<std::string, ActorDefinition*> s_definitions;

	std::string m_faction;
	int m_maxHealth;
	int m_startHealth;
	bool m_enableActorVsActorPhysics = true;
	float m_mass = 1.f;
	bool m_hasAlternateSlot = false;
	Stats m_baseStats;

	//attack for Player
	float m_attackRate;
	Vector2 m_meleeOffset = Vector2(0, 0);
	float m_rangedOffset = 0;
	float m_meleeRadius = 1;
	float m_attackDelay = 0;
	std::string m_projectileName;

	std::vector<std::pair<std::string, float>> m_lootToSpawn;
	
	std::vector<AIBehavior*> m_behaviors;
};

ActorDefinition* ParseXmlAttribute( const tinyxml2::XMLElement& element, const char* attributeName, ActorDefinition* defaultValue );

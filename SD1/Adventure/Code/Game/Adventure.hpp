#pragma once

#include "Game/AdventureDefinition.hpp"

class  Map;
class Player;
class Actor;

class Adventure
{
public:
	~Adventure();
	Adventure(const std::string name, AdventureDefinition* def);
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Render();

	void SpawnPlayerInAdventure(Player* player);
	void SwitchMap(std::string nextMap);
	void CheckKillObjective(Actor* target);

public:
	std::string m_name;
	AdventureDefinition* m_adventureDef = nullptr;
	Map* m_currentMap = nullptr;
	Player* m_player = nullptr;
	std::map<std::string, Map*> m_mapDict;
};
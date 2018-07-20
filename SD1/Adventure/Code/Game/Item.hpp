#pragma once

#include "Game/Entity.hpp"
#include "Game/ItemDefinition.hpp"
#include "Game/Stats.hpp"

class Item : public Entity
{
public:
	~Item();
	Item(const std::string name, ItemDefinition* def, Map* attachedMap);

public:
	ItemDefinition* m_itemDef = nullptr;
	Stats m_stats;
};

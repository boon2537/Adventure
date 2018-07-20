#pragma once

#include "Game/EntityDefinition.hpp"
#include "Game/Stats.hpp"

enum EQUIP_SLOT
{
	EQUIP_SLOT_NONE = -1,
	EQUIP_SLOT_HAND,
	EQUIP_SLOT_HEAD,
	EQUIP_SLOT_HEAD_ALT,
	EQUIP_SLOT_CHEST,
	EQUIP_SLOT_CHEST_ALT,
	NUM_EQUIP_SLOT
};

class ItemDefinition : public EntityDefinition
{
public:
	~ItemDefinition();
	ItemDefinition(const tinyxml2::XMLElement & element);

	static void LoadItemDefinitions(const std::string& path);
	static ItemDefinition* GetItemDefinition(const std::string& defName);
	static void LoadLootTable(const std::string& path);
	static ItemDefinition* GetRandomItemFromLootTable(const std::string& lootName);

public:
	static std::map<std::string, ItemDefinition*> s_definitions;
	static std::map<std::string, std::vector<ItemDefinition*>> s_lootTable;

	EQUIP_SLOT m_equipSlot = EQUIP_SLOT::EQUIP_SLOT_NONE;
	Stats m_minStats;
	Stats m_maxStats;
	AABB2 m_equippedDrawBound;
	bool m_dropOnDeath = false;
};

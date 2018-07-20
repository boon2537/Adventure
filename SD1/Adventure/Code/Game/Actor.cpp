#include "Game/Actor.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Item.hpp"

Actor::~Actor()
{
}

Actor::Actor(const std::string name, ActorDefinition* def, Map* attachedMap)
	: Entity(name, def, attachedMap)
{
	m_actorDef = def;
	m_currentHealth = m_actorDef->m_maxHealth;

	//Deep copy AIBehaviours from def
	for (unsigned int i = 0; i < m_actorDef->m_behaviors.size(); i++)
	{
		m_behaviors.push_back(m_actorDef->m_behaviors[i]->CloneForActor(this));
	}

	//initialize equipped items vector
	for (int i = 0; i < EQUIP_SLOT::NUM_EQUIP_SLOT; i++)
		m_equippedItems[i] = nullptr;

	//Spawn items in inventory
	for (unsigned int i = 0; i < m_actorDef->m_lootToSpawn.size(); i++)
	{
		float percentChance = m_actorDef->m_lootToSpawn[i].second;
		if (CheckRandomChance(percentChance))
		{
			ItemDefinition* itemToSpawn = ItemDefinition::GetRandomItemFromLootTable(m_actorDef->m_lootToSpawn[i].first);
			if (itemToSpawn != nullptr && (itemToSpawn->m_equipSlot == EQUIP_SLOT::EQUIP_SLOT_NONE || m_equippedItems[itemToSpawn->m_equipSlot] == nullptr))
			{
				Item* item = new Item(itemToSpawn->m_name, itemToSpawn, nullptr);
				PickUpAndEquip(item);
			}
		}	
	}

	m_healthBar = SliderBar(Vector2(0, 0.8f), AABB2(0, 0, 0.8f, 0.1f), Rgba(255, 220, 220, 80), Rgba(255, 0, 0, 180));
}

void Actor::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	SelectBehavior();
	if (m_currentBehavior != nullptr)
		m_currentBehavior->Update(deltaTime);

	//reset damage effect
	if (m_timeToResetTint <= m_ageInSeconds)
		m_colorTint = m_entityDef->m_defaultTint;

	m_position += m_velocity * deltaTime;
	
	if (!m_attacking)
		SetMoveDirectionAnimation();
	UpdateEquippedItems(deltaTime);

	Stats myStats = GetCurrentStats();
	m_healthBar.m_fillColor = Stats::GetElementColor(myStats.m_statValues[STAT_ID::STAT_FIRE_DEF], myStats.m_statValues[STAT_ID::STAT_WATER_DEF], myStats.m_statValues[STAT_ID::STAT_EARTH_DEF]);
	m_attackTint = Stats::GetElementColor(myStats.m_statValues[STAT_ID::STAT_FIRE_ATK], myStats.m_statValues[STAT_ID::STAT_WATER_ATK], myStats.m_statValues[STAT_ID::STAT_EARTH_ATK]);
}

void Actor::UpdateEquippedItems(float deltaTime)
{
	for (int i = 0; i < EQUIP_SLOT::NUM_EQUIP_SLOT; i++)
	{
		Item* item = m_equippedItems[i];
		if (item != nullptr)
		{
			item->Update(deltaTime);

			//Set animation
			if (m_spriteAnimSet->GetCurrentAnimName() != item->m_spriteAnimSet->GetCurrentAnimName())
			{
				item->m_spriteAnimSet->StartAnim(m_spriteAnimSet->GetCurrentAnimName());
				item->m_spriteAnimSet->SetElapsedSeconds(m_spriteAnimSet->GetElapsedSeconds());
				//Update animation speed
				item->m_spriteAnimSet->SetCurrentAnimSpeed(m_spriteAnimSet->GetCurrentAnimSpeed());
			}
		}
	}
}

void Actor::Render() const
{
	Entity::Render();

	//Render Equip Items
	AABB2 UVsToUse;
	Item* itemToRender;
	bool isUnderDamageEffect = m_timeToResetTint > m_ageInSeconds;
	Rgba colorToUse;
	g_theRenderer->PushMatrix();
	g_theRenderer->TranslateInMatrix(m_position);

	itemToRender = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD];
	if (itemToRender != nullptr)
	{
		UVsToUse = itemToRender->m_spriteAnimSet->GetCurrentUVs();
		colorToUse = isUnderDamageEffect ? Rgba::red : itemToRender->m_colorTint;
		g_theRenderer->DrawTexturedAABB(itemToRender->m_itemDef->m_equippedDrawBound, itemToRender->m_spriteAnimSet->GetCurrentTexture(), UVsToUse.mins, UVsToUse.maxs, colorToUse);
	}
	itemToRender = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST];
	if (itemToRender != nullptr)
	{
		UVsToUse = itemToRender->m_spriteAnimSet->GetCurrentUVs();
		colorToUse = isUnderDamageEffect ? Rgba::red : itemToRender->m_colorTint;
		g_theRenderer->DrawTexturedAABB(itemToRender->m_itemDef->m_equippedDrawBound, itemToRender->m_spriteAnimSet->GetCurrentTexture(), UVsToUse.mins, UVsToUse.maxs, colorToUse);
	}
	itemToRender = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HAND];
	if (itemToRender != nullptr)
	{
		UVsToUse = itemToRender->m_spriteAnimSet->GetCurrentUVs();
		g_theRenderer->DrawTexturedAABB(itemToRender->m_itemDef->m_equippedDrawBound, itemToRender->m_spriteAnimSet->GetCurrentTexture(), UVsToUse.mins, UVsToUse.maxs, m_attackTint);
	}

	g_theRenderer->PopMatrix();

	RenderWorldUI();
}

void Actor::RenderWorldUI() const
{
	m_healthBar.Render(m_position, Vector2(0.5f, 0.5f));
}

void Actor::OnDestroy()
{
	g_theGame->m_currentAdventure->CheckKillObjective(this);
}

void Actor::AdjustEquippedItemsSpeed(float speed)
{
	for (int i = 0; i < EQUIP_SLOT::NUM_EQUIP_SLOT; i++)
	{
		Item* item = m_equippedItems[i];
		if (item != nullptr)
		{
			item->m_spriteAnimSet->SetCurrentAnimSpeed(speed);
		}
	}
}

void Actor::RemoveItemFromInventory(Item * item)
{
	for (unsigned int i = 0; i < m_inventory.size(); i++)
	{
		if (m_inventory[i] == item)
			m_inventory[i] = nullptr;
	}
}

void Actor::TakeDamage(int baseDamage, Stats shooterStats)
{
	m_currentHealth -= baseDamage;
	m_currentHealth -= Stats::ResolveDamageDealt(shooterStats, GetCurrentStats());
	m_healthBar.m_value = (float) m_currentHealth / m_actorDef->m_maxHealth;
	if (m_currentHealth <= 0)
		Die();

	//show damage effect
	m_colorTint = Rgba::red;
	m_timeToResetTint = m_ageInSeconds + TIME_TO_SHOW_DAMAGE_EFFECT;
}

void Actor::Die()
{
	Destroy();

	for (unsigned int i = 0; i < m_inventory.size(); i++)
	{
		if (m_inventory[i] != nullptr)
		{	
			if (m_inventory[i]->m_itemDef->m_dropOnDeath)
				DropItemToMap(m_inventory[i]);
			else
				delete m_inventory[i];
		}
	}
}

void Actor::SetMeleeAttackDirectionAnimation()
{
	MOVE_DIRECTION direction = GetMoveDirection(m_lastValidVelocity);

	switch (direction)
	{
		case MOVE_DIRECTION::NO_MOVEMENT:
			m_spriteAnimSet->StartAnim("Idle");
			break;
		case MOVE_DIRECTION::NORTH:
			m_spriteAnimSet->StartAnim("SlashNorth");
			break;
		case MOVE_DIRECTION::SOUTH:
			m_spriteAnimSet->StartAnim("SlashSouth");
			break;
		case MOVE_DIRECTION::EAST:
			m_spriteAnimSet->StartAnim("SlashEast");
			break;
		case MOVE_DIRECTION::WEST:
			m_spriteAnimSet->StartAnim("SlashWest");
			break;
	}
}

void Actor::SetCastingDirectionAnimation()
{
	MOVE_DIRECTION direction = GetMoveDirection(m_lastValidVelocity);

	switch (direction)
	{
	case MOVE_DIRECTION::NO_MOVEMENT:
		m_spriteAnimSet->StartAnim("Idle");
		break;
	case MOVE_DIRECTION::NORTH:
		m_spriteAnimSet->StartAnim("CastNorth");
		break;
	case MOVE_DIRECTION::SOUTH:
		m_spriteAnimSet->StartAnim("CastSouth");
		break;
	case MOVE_DIRECTION::EAST:
		m_spriteAnimSet->StartAnim("CastEast");
		break;
	case MOVE_DIRECTION::WEST:
		m_spriteAnimSet->StartAnim("CastWest");
		break;
	}
}

void Actor::PickUpAndEquip(Item* item)
{
	if (item->m_mapAttached != nullptr)
	{
		item->m_mapAttached->RemovePickedUpItem(item);
	}

	//replace and maybe drop item
	item->m_position = m_position;
	if (item->m_itemDef->m_equipSlot != EQUIP_SLOT::EQUIP_SLOT_NONE)
	{
		if (item->m_itemDef->m_equipSlot == EQUIP_SLOT::EQUIP_SLOT_HEAD)
		{
			if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD] == nullptr)
			{
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD] = item;
			}
			else if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD_ALT] == nullptr && m_actorDef->m_hasAlternateSlot)
			{
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD_ALT] = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD];
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD]   = item;
			}
			else
			{
				DropItemToMap(m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD]);
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD] = item;
			}
		}
		else if (item->m_itemDef->m_equipSlot ==EQUIP_SLOT::EQUIP_SLOT_CHEST)
		{
			if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST] == nullptr)
			{
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST] = item;
			}
			else if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST_ALT] == nullptr && m_actorDef->m_hasAlternateSlot)
			{
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST_ALT] = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST];
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST] = item;
			}
			else
			{
				DropItemToMap(m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST]);
				m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST] = item;
			}
		}
		else
			m_equippedItems[item->m_itemDef->m_equipSlot] = item;
	}

	m_inventory.push_back(item);
}

void Actor::DropItemToMap(Item* item)
{
	if (item == nullptr)
		return;

	RemoveItemFromInventory(item);
	m_equippedItems[item->m_itemDef->m_equipSlot] = nullptr;
	item->m_spriteAnimSet->SetCurrentAnim("Idle");
	m_mapAttached->ReceiveItem(item, m_position, m_orientation);
}

void Actor::SelectBehavior()
{
	float maxUtil = 0;
	// help with flip-flops back and forth between two AIBehaviors with similar utility scores
	// another behavior must exceed the current behavior utility by at least x in order to switch behaviors
	float currentBehaviorBonus = 0.1f;

	for (unsigned int i = 0; i < m_behaviors.size(); i++)
	{
		AIBehavior* potential = m_behaviors[i];
		float potentialUtil = potential->CalcUtility();
		if (potential == m_currentBehavior)
		{
			potentialUtil += currentBehaviorBonus;
		}

		if (potentialUtil > maxUtil)
		{
			maxUtil = potentialUtil;
			m_currentBehavior = potential;
		}
	}

}

Stats Actor::GetCurrentStats()
{
	Stats itemStats;
	if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD] != nullptr)
		itemStats = itemStats + m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD]->m_stats;
	if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST] != nullptr)
		itemStats = itemStats + m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST]->m_stats;
	if (m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HAND] != nullptr)
		itemStats = itemStats + m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HAND]->m_stats;

	return m_actorDef->m_baseStats + itemStats;
}

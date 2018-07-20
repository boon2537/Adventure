#include "Game/Player.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Item.hpp"

Player::~Player()
{
}

Player::Player(const std::string name, ActorDefinition * def)
	: Actor(name, def, nullptr)
{
	POTION_HEAL = m_actorDef->m_maxHealth / 2;
}

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);	

	//reset damage effect
	if (m_timeToResetTint <= m_ageInSeconds)
		m_colorTint = Rgba::white;

	//Running
	if (!m_attacking)
	{
		m_spriteAnimSet->SetCurrentAnimSpeed(1);
		AdjustEquippedItemsSpeed(1);
	}
	m_running = false;
	if (!m_attacking && g_xboxController_1->m_LeftTrigger > 0.9f)
	{
		if (m_currentStamina > 0)
		{
			m_running = true;
			m_currentStamina -= STAMINA_CONSUME_RATE * deltaTime;
		}
	}
	else
		m_currentStamina = min(m_currentStamina + (STAMINA_REGEN_RATE * deltaTime), MAX_STAMINA);

	//Move
	if (g_xboxController_1->IsActive())
	{
		if (g_xboxController_1->GetLeftJoyStick().m_magnitude > 0)
		{
			m_orientation = g_xboxController_1->GetLeftJoyStick().m_angleDegrees;

			float velocityFactor = g_xboxController_1->GetLeftJoyStick().m_magnitude;
			if (m_running && !m_attacking)
			{
				m_spriteAnimSet->SetCurrentAnimSpeed(RUN_SPEED_FACTOR);
				AdjustEquippedItemsSpeed(RUN_SPEED_FACTOR);
				velocityFactor *= RUN_SPEED_FACTOR;
			}
			m_velocity.x = CosDegrees(m_orientation) * m_actorDef->m_speed * velocityFactor;
			m_velocity.y = SinDegrees(m_orientation) * m_actorDef->m_speed * velocityFactor;		
			m_lastValidVelocity = m_velocity;
		}
		else
			m_velocity = Vector2(0, 0);
	}

	//Attack
	if (g_theInput->WasKeyJustPressed(KEY_CODE::Z) || g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::X))
	{
		if (m_nextAttackTriggerTime < m_ageInSeconds)
		{
			SetMeleeAttackDirectionAnimation();
			m_attacking = true;
			m_pendingAttack = true;
			m_directionAtAttack = GetMoveDirectionValue(m_lastValidVelocity);
			m_nextAttackTriggerTime = m_ageInSeconds + (1.f / m_actorDef->m_attackRate);
			m_attackTime = m_ageInSeconds + m_actorDef->m_attackDelay;

			m_spriteAnimSet->SetCurrentAnimSpeed(m_spriteAnimSet->GetCurrentAnimDuration() / (1.f / m_actorDef->m_attackRate));
		}
	}

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::A))
	{
		if (m_itemToPickUp != nullptr)
		{
			PickUpAndEquip(m_itemToPickUp);		
			m_itemToPickUp = nullptr;
		}
		else
		{
			DropItemToMap(m_equippedItems[m_selectedEquipSlot]);
		}
	}

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::Y))
	{
		if (m_selectedEquipSlot == EQUIP_SLOT::EQUIP_SLOT_HEAD)
		{
			//swap head item
			Item* temp = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD];
			m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD] = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD_ALT];
			m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD_ALT] = temp;
		}
		m_selectedEquipSlot = EQUIP_SLOT::EQUIP_SLOT_HEAD;
	}

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::B))
	{
		if (m_selectedEquipSlot == EQUIP_SLOT::EQUIP_SLOT_CHEST)
		{
			//swap chest item
			Item* temp = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST];
			m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST] = m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST_ALT];
			m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST_ALT] = temp;
		}
		m_selectedEquipSlot = EQUIP_SLOT::EQUIP_SLOT_CHEST;
	}

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::LEFT_SHOULDER))
	{
		UsePotion();
	}

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::RIGHT_SHOULDER))
	{
		SetItemTransferTarget();
	}

	//Item transfer
	if (m_transferJustPressed == false && g_xboxController_1->m_RightTrigger > 0.9f)
	{
		m_transferJustPressed = true;
		TransferItem(m_itemTransferTarget);
	}
	else
		m_transferJustPressed = false;



	if (m_nextAttackTriggerTime < m_ageInSeconds)
		m_attacking = false;
	if (m_pendingAttack && m_attackTime < m_ageInSeconds)
	{
		m_pendingAttack = false;
		Vector2 attackOffset = Vector2(m_actorDef->m_meleeOffset.x * m_directionAtAttack.x, m_actorDef->m_meleeOffset.y * m_directionAtAttack.y);
		m_mapAttached->SpawnProjectile("PlayerMelee", this, m_position + attackOffset, m_orientation);
	}

	if (!m_attacking)
	{
		m_position.x = m_position.x + (m_velocity.x * deltaTime);
		m_position.y = m_position.y + (m_velocity.y * deltaTime);
		SetMoveDirectionAnimation();
	}
	UpdateEquippedItems(deltaTime);

	Stats myStats = GetCurrentStats();
	m_healthBar.m_fillColor = Stats::GetElementColor(myStats.m_statValues[STAT_ID::STAT_FIRE_DEF], myStats.m_statValues[STAT_ID::STAT_WATER_DEF], myStats.m_statValues[STAT_ID::STAT_EARTH_DEF]);
	m_attackTint = Stats::GetElementColor(myStats.m_statValues[STAT_ID::STAT_FIRE_ATK], myStats.m_statValues[STAT_ID::STAT_WATER_ATK], myStats.m_statValues[STAT_ID::STAT_EARTH_ATK]);
}

void Player::Render() const
{
	if (!IsAlive())
		return;

	Actor::Render();
	//g_theRenderer->DrawDashedCircle(m_position, TRANSFER_TARGET_SEEK_RADIUS, Rgba(255, 255, 0));
}

void Player::RenderWorldUI() const
{
}

void Player::Die()
{
	g_theGame->StartTransitionToState(GAME_STATE::DEFEAT);
}

int Player::GetPotionCount() const
{
	int output = 0;
	for (unsigned int i = 0; i < m_inventory.size(); i++)
	{
		if (m_inventory[i] != nullptr && m_inventory[i]->m_itemDef->m_name == "HealthPotion")
			output++;
	}
	return output;
}

void Player::UsePotion()
{
	if (GetPotionCount() <= 0)
		return;

	for (unsigned int i = 0; i < m_inventory.size(); i++)
	{
		if (m_inventory[i] != nullptr && m_inventory[i]->m_itemDef->m_name == "HealthPotion")	
		{
			m_inventory[i] = nullptr;
			m_currentHealth = min(m_currentHealth + POTION_HEAL, m_actorDef->m_maxHealth);
			break;
		}
	}
}

void Player::SetItemTransferTarget()
{
	//find within angle player is facing
	for (unsigned int i = 0; i < m_mapAttached->m_actors.size(); i++)
	{
		Actor* potential = m_mapAttached->m_actors[i];
		if (potential != nullptr && potential->m_actorDef->m_faction != m_actorDef->m_faction && m_itemTransferTarget != potential)
		{
			float dotProductResult = DotProduct(Vector2(CosDegrees(m_orientation), SinDegrees(m_orientation)).GetNormalized(), (potential->m_position - m_position).GetNormalized());
			if (dotProductResult > SEEK_PRIORITY_DEGREE && GetDistance(m_position, potential->m_position) < TRANSFER_TARGET_SEEK_RADIUS)
			{
				m_itemTransferTarget = potential;
				return;
			}
		}
	}

	//find closest
	float targetDistance = 10000;
	Actor* target = nullptr;
	for (unsigned int i = 0; i < m_mapAttached->m_actors.size(); i++)
	{
		Actor* potential = m_mapAttached->m_actors[i];	
		if (potential != nullptr && potential->m_actorDef->m_faction != m_actorDef->m_faction && m_itemTransferTarget != potential)
		{
			float d = GetDistance(m_position, potential->m_position);
			if (d < TRANSFER_TARGET_SEEK_RADIUS && d < targetDistance)
			{
				targetDistance = d;
				target = potential;
			}		
		}
	}
	m_itemTransferTarget = target;
}

void Player::TransferItem(Actor * target)
{
	if (m_itemTransferTarget == nullptr)
		return;

	if (m_equippedItems[m_selectedEquipSlot] != nullptr)
	{
		target->PickUpAndEquip(m_equippedItems[m_selectedEquipSlot]);
		RemoveItemFromInventory(m_equippedItems[m_selectedEquipSlot]);
		m_equippedItems[m_selectedEquipSlot] = nullptr;
	}
}

void Player::ResetPlayer(const Vector2 & position, Map * map)
{
	m_position = position;
	m_mapAttached = map;
	m_currentHealth = m_actorDef->m_maxHealth;
}

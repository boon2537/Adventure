#pragma once

#include "Game/Entity.hpp"
#include "Game/ActorDefinition.hpp"
#include "Game/ItemDefinition.hpp"
#include "Game/SliderBar.hpp"
#include "Game/Stats.hpp"

class Actor : public Entity
{
public:
	~Actor();
	Actor(const std::string name, ActorDefinition* def, Map* attachedMap);
	virtual void Update(float deltaTime) override;
	virtual void UpdateEquippedItems(float deltaTime);
	virtual void Render() const override;
	virtual void RenderWorldUI() const override;
	virtual void OnDestroy() override;
	virtual void Die();

	void AdjustEquippedItemsSpeed(float speed);
	void RemoveItemFromInventory(Item* item);
	void TakeDamage(int baseDamage, Stats shooterStats);
	void SetMeleeAttackDirectionAnimation();
	void SetCastingDirectionAnimation();
	void PickUpAndEquip(Item* item);
	void DropItemToMap(Item* item);
	void SelectBehavior();
	Stats GetCurrentStats();

public:
	int m_currentHealth;
	ActorDefinition* m_actorDef = nullptr;
	Item* m_equippedItems[EQUIP_SLOT::NUM_EQUIP_SLOT];
	Vector2 m_lastValidVelocity; //to help determine which way the actor was facing before stopping
	Vector2 m_directionAtAttack; //make sure our delayed melee attack is facing in the correct direction
	Rgba m_attackTint = Rgba::red;
	bool m_attacking = false;
	float m_timeToResetTint = 0;
	float TIME_TO_SHOW_DAMAGE_EFFECT = 0.1f;
	SliderBar m_healthBar;

protected:
	float m_nextAttackTriggerTime = 0;
	float m_attackTime = 0;
	bool m_pendingAttack = false;
	AIBehavior* m_currentBehavior;
	std::vector<AIBehavior*> m_behaviors;
};

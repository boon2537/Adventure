#pragma once

#include "Game/Actor.hpp"

class Player : public Actor
{
public:
	~Player();
	Player(const std::string name, ActorDefinition* def);

	virtual void Update(float deltaTime) override;
	virtual void Render() const;
	virtual void RenderWorldUI() const override;
	virtual void Die() override;

	bool IsAlive() const { return m_currentHealth > 0; }
	int GetPotionCount() const;
	void UsePotion();
	void SetItemTransferTarget();
	void TransferItem(Actor* target);
	void ResetPlayer(const Vector2& position, Map* map);

public:
	const float MAX_STAMINA = 100;
	const float STAMINA_CONSUME_RATE = 40.f;
	const float STAMINA_REGEN_RATE = 25.f;
	const float RUN_SPEED_FACTOR = 1.7f;
	const float TRANSFER_TARGET_SEEK_RADIUS = 7.f;
	const float SEEK_PRIORITY_DEGREE = CosDegrees(20);
	int POTION_HEAL = 150;

	bool m_running = false;
	bool m_transferJustPressed = false;
	Actor* m_itemTransferTarget = nullptr;
	Item* m_itemToPickUp = nullptr;
	float m_currentStamina = MAX_STAMINA;
	EQUIP_SLOT m_selectedEquipSlot = EQUIP_SLOT::EQUIP_SLOT_HEAD;
};

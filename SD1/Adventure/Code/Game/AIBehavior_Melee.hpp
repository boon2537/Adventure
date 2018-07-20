#pragma once
#include "Game/AIBehavior.hpp"
#include "Engine/Math/Vector2.hpp"

class AIBehavior_Melee : public AIBehavior
{
public:
	AIBehavior_Melee( const tinyxml2::XMLElement& AIBehaviorXmlElement );

	virtual void Update(float deltaSeconds) override;
	virtual AIBehavior* CloneForActor(Actor* masterActor) override;
	virtual float CalcUtility() override;

public:
	float m_range = 5.f;
	float m_attackRate = 1.f; // shoot per second
	float m_attackDelay = 0.2f;
	Vector2 m_meleeOffset = Vector2(0, 0);
	std::string m_projectileName;

private:
	bool m_pendingAttack = false;
	float m_nextAttackTriggerTime = 0;
	float m_attackTime = 0;
	float m_attackAnimationFinishTime = 0;
};

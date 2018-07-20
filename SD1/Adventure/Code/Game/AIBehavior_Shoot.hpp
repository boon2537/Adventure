#pragma once
#include "Game/AIBehavior.hpp"

class AIBehavior_Shoot : public AIBehavior
{
public:
	AIBehavior_Shoot( const tinyxml2::XMLElement& AIBehaviorXmlElement );

	virtual void Update(float deltaSeconds) override;
	virtual AIBehavior* CloneForActor(Actor* masterActor) override;
	virtual float CalcUtility() override;

public:
	float m_range = 5.f;
	float m_attackRate = 1.f; // shoot per second
	float m_attackDelay = 0.2f;
	std::string m_projectileName;

private:
	bool m_pendingAttack = false;
	float m_nextAttackTriggerTime = 0;
	float m_attackTime = 0;
	float m_attackAnimationFinishTime = 0;
};

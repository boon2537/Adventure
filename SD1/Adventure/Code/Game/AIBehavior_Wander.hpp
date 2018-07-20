#pragma once

#include "Game/AIBehavior.hpp"
#include "Engine/ThirdParty/tinyxml2.h"
#include "Engine/Math/Vector2.hpp"

class AIBehavior_Wander : public AIBehavior
{
public:
	AIBehavior_Wander( const tinyxml2::XMLElement& AIBehaviorXmlElement );

	virtual void Update(float deltaSeconds) override;
	virtual AIBehavior* CloneForActor(Actor* masterActor) override;
	virtual float CalcUtility() override;

public:
	Vector2 m_currentWanderGoal;
	float m_nextPickWanderTime = 0;
	float m_pickDelay = 3.f;
};
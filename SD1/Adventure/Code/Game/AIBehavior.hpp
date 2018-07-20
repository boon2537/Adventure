#pragma once

#include "Engine/ThirdParty/tinyxml2.h"
#include <string>

class Actor;

class AIBehavior
{
public:
	virtual ~AIBehavior() {};
	AIBehavior( const tinyxml2::XMLElement& AIBehaviorXmlElement );
	virtual void Update(float deltaSeconds) { m_timeElapsed += deltaSeconds; };
	virtual AIBehavior* CloneForActor(Actor* masterActor) = 0;
	virtual float CalcUtility() = 0;

	static AIBehavior* CreateAIBehavior( const tinyxml2::XMLElement& AIBehaviorXmlElement );

public:
	float m_basePriority = 0.f;
	Actor* m_actorAttached = nullptr;

protected:
	std::string	m_name;
	float m_timeElapsed = 0;
};
#pragma once

#include "Game/AIBehavior.hpp"
#include "Engine/ThirdParty/tinyxml2.h"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/HeatMap.hpp"

class AIBehavior_Follow : public AIBehavior
{
public:
	AIBehavior_Follow( const tinyxml2::XMLElement& AIBehaviorXmlElement );

	virtual void Update(float deltaSeconds) override;
	virtual AIBehavior* CloneForActor(Actor* masterActor) override;
	virtual float CalcUtility() override;

	void SelectFollowTarget();
	Vector2 GetDestinationFromHeatMap(const HeatMap& heatMap, const IntVector2& center);

public:
	std::string m_followTargetName;
	float m_distanceToFollow;
	float m_distanceMin = 0;
	Actor* m_currentTarget = nullptr;
	HeatMap m_distanceMapToTarget;
	std::vector<IntVector2> m_targetCoords;
	float m_currentHeatToTarget = 10000000;
	Vector2 m_moveTarget = Vector2(0, 0);
};
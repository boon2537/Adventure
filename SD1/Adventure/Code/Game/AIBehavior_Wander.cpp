#include "Game/AIBehavior_Wander.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

AIBehavior_Wander::AIBehavior_Wander(const tinyxml2::XMLElement & AIBehaviorXmlElement)
	: AIBehavior(AIBehaviorXmlElement)
{
	m_pickDelay = ParseXmlAttribute(AIBehaviorXmlElement, "pickDelay", m_pickDelay);
}

void AIBehavior_Wander::Update(float deltaSeconds)
{
	AIBehavior::Update(deltaSeconds);

	if (m_nextPickWanderTime < m_timeElapsed)
	{
		m_currentWanderGoal = Vector2(GetRandomFloatInRange(0, static_cast<float>(m_actorAttached->m_mapAttached->m_mapWidth)), GetRandomFloatInRange(0, static_cast<float>(m_actorAttached->m_mapAttached->m_mapHeight)));
		m_actorAttached->m_orientation = Atan2Degress(m_currentWanderGoal.y - m_actorAttached->m_position.y, m_currentWanderGoal.x - m_actorAttached->m_position.x);
		m_nextPickWanderTime = m_timeElapsed + m_pickDelay;

		m_actorAttached->m_velocity = Vector2(CosDegrees(m_actorAttached->m_orientation), SinDegrees(m_actorAttached->m_orientation)) * m_actorAttached->m_entityDef->m_speed;
		m_actorAttached->m_lastValidVelocity = m_actorAttached->m_velocity;
	}
}

AIBehavior* AIBehavior_Wander::CloneForActor(Actor* masterActor)
{
	m_actorAttached = masterActor;
	return new AIBehavior_Wander(*this);
}

float AIBehavior_Wander::CalcUtility()
{
	return m_basePriority;
}

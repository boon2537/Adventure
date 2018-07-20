#include "Game/AIBehavior_Follow.hpp"
#include "Game/Map.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/XmlUtilities.hpp"

AIBehavior_Follow::AIBehavior_Follow(const tinyxml2::XMLElement & AIBehaviorXmlElement)
	: AIBehavior(AIBehaviorXmlElement)
{
	m_distanceToFollow = ParseXmlAttribute(AIBehaviorXmlElement, "distanceToFollow", 7.f);
	m_followTargetName = ParseXmlAttribute(AIBehaviorXmlElement, "target", "");
	m_distanceMin = ParseXmlAttribute(AIBehaviorXmlElement, "distanceMin", m_distanceMin);
	m_targetCoords.push_back(IntVector2(0, 0));
}

void AIBehavior_Follow::Update(float deltaSeconds)
{
	AIBehavior::Update(deltaSeconds);

	//use shared heat map from player to save time/space
	if (m_currentTarget == g_theGame->m_player)
	{
		m_moveTarget = GetDestinationFromHeatMap(m_actorAttached->m_mapAttached->m_playerDistanceField, m_actorAttached->m_mapAttached->GetClosetTileFromPosition(m_actorAttached->m_position).m_tileCoords);
	}
	else
	{
		m_targetCoords[0] = m_actorAttached->m_mapAttached->GetClosetTileFromPosition(m_currentTarget->m_position).m_tileCoords;
		m_actorAttached->m_mapAttached->UpdateDistanceField(m_distanceMapToTarget, m_targetCoords);
		m_moveTarget = GetDestinationFromHeatMap(m_distanceMapToTarget, m_actorAttached->m_mapAttached->GetClosetTileFromPosition(m_actorAttached->m_position).m_tileCoords);
	}

	m_actorAttached->m_orientation = Atan2Degress(m_moveTarget.y - m_actorAttached->m_position.y, m_moveTarget.x - m_actorAttached->m_position.x);
	m_actorAttached->m_velocity = Vector2(CosDegrees(m_actorAttached->m_orientation), SinDegrees(m_actorAttached->m_orientation)) * m_actorAttached->m_entityDef->m_speed;
	m_actorAttached->m_lastValidVelocity = m_actorAttached->m_velocity;
}

AIBehavior* AIBehavior_Follow::CloneForActor(Actor* masterActor)
{
	m_actorAttached = masterActor;
	return new AIBehavior_Follow(*this);
}

float AIBehavior_Follow::CalcUtility()
{
	if (m_followTargetName == "")
	{
		return 0;
	}

	SelectFollowTarget();

	if (m_currentTarget == nullptr)
		return 0;

	float d = GetDistance(m_actorAttached->m_position, m_currentTarget->m_position);
	if (d < m_distanceToFollow && d > m_distanceMin)
		return m_basePriority;
	else
		return 0;
}

void AIBehavior_Follow::SelectFollowTarget()
{
	//pick the closet target to follow
	Actor* previousTarget = m_currentTarget;
	float minDistance = 100000.f;
	for (unsigned int i = 0; i < m_actorAttached->m_mapAttached->m_actors.size(); i++)
	{
		Actor* potential = m_actorAttached->m_mapAttached->m_actors[i];
		if (potential != nullptr && potential->m_actorDef->m_name == m_followTargetName)
		{
			float d = GetDistance(m_actorAttached->m_position, potential->m_position);
			if (d < minDistance)
			{
				minDistance = d;
				m_currentTarget = potential;
			}
		}
	}

	if (m_currentTarget != nullptr && previousTarget != m_currentTarget)
	{
		m_distanceMapToTarget = HeatMap(IntVector2(m_currentTarget->m_mapAttached->m_mapWidth, m_currentTarget->m_mapAttached->m_mapHeight), m_currentTarget->m_mapAttached->HEAT_MAP_FILL_VALUE);
	}
}

Vector2 AIBehavior_Follow::GetDestinationFromHeatMap(const HeatMap& heatMap, const IntVector2& center)
{
	float north = heatMap.GetHeat(IntVector2(center.x, center.y + 1));
	float south = heatMap.GetHeat(IntVector2(center.x, center.y - 1));
	float east = heatMap.GetHeat(IntVector2(center.x + 1, center.y));
	float west = heatMap.GetHeat(IntVector2(center.x - 1, center.y));

	float selected = min(min(north, south), min(east, west));

	if (selected == m_currentHeatToTarget) //so we don't flip flop
		return m_moveTarget;
	
	m_currentHeatToTarget = selected;
	if (selected == north)	return Vector2((float)center.x + 0.5f, (float)center.y + 1.5f);
	if (selected == south)	return Vector2((float)center.x + 0.5f, (float)center.y - 0.5f);
	if (selected == east)	return Vector2((float)center.x + 1.5f, (float)center.y + 0.5f);
	if (selected == west)	return Vector2((float)center.x - 0.5f, (float)center.y + 0.5f);

	return m_moveTarget;
}
